// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 去除字符串首尾的标点，并将所有字母转换为小写。
 * 检查字符串是否全为非字母字符，如果都是非字母，
 * 则丢弃该字符串并返回一个空字符串。
 */
string cleanToken(string s)
{
    s = toLowerCase(s);
    int count = s.length();
    //确认至少包含一个字符
    for (int i = s.length() - 1; i >= 0; i--) {
        if (!isalpha(s.at(i))) {
            count--;
        }
        else break;
    }
    //没有字符通过测试，返回空字符串
    if (!count) {
        return "";
    }
    //否则，删除字符串首尾的标点
    else {
        while (!s.empty()) {
            if (ispunct(s.at(0))) {
                s.erase(s.begin());
            }
            else break;
        }
        while (!s.empty()) {
            if (ispunct(s.at(s.length() - 1))) {
                s.erase(s.length() - 1);
            }
            else break;
        }
        return s;
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/* 从正文文本中提取出所有不重复的字符串，并以集合的形式返回
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    //以空格为分隔符，拆分文本
    Vector<string> son = stringSplit(text, " ");
    //循环，处理各个子字符串，并放入集合
    for (string item : son) {
        string pure = cleanToken(item);
        if (!pure.empty()) {
            tokens.add(pure);
        }
    }
    return tokens;
}

// 从数据库文件读取内容，并将其处理为反向索引的形式
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    Map<string, Set<string>> forwardIndex;
    //按行读入文件，存入Vector
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    Vector<string> lines;
    readEntireFile(in, lines);

    //初始化
    Set<string> allTokens;
    //构建正向索引
    for (int i = 0; i < lines.size(); i += 2) {
        Set<string> tokens = gatherTokens(lines.get(i + 1));
        forwardIndex.put(lines.get(i), tokens);
        allTokens += tokens;
    }
    //构建反向索引
    for (const string &elem : allTokens) {
        Set<string> reverseUrl;
        for (const string &item : forwardIndex) {
            Set<string> reverseTokens = forwardIndex.get(item);
            if (reverseTokens.contains(elem)) {
                reverseUrl.add(item);
            }
        }
        index.put(elem, reverseUrl);
    }

    return forwardIndex.size();
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    //以空格为分隔符，拆分文本
    Vector<string> son = stringSplit(query, " ");
    //循环，处理各个子字符串
    result = index.get(son.get(0));
    for (const string &item : son) {
        if (item == son.get(0)) continue;
        Set<string> website = index.get(cleanToken(item));
        switch (item.at(0)) {
        case '+':
            result.intersect(website);
            break;
        case '-':
            result.difference(website);
            break;
        default:
            result.unionWith(website);
        }
    }
    return result;
}

// 构建简易搜索引擎
void searchEngine(string dbfile)
{
    // TODO: your code here
    //构建反向索引
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index;
    int npage = buildIndex(dbfile, index);
    //打印网页数量和包含单词数
    cout << "Indexed " << npage << " pages ";
    cout << "contains " << index.size() << " unique terms" << endl;
    cout << endl;
    //循环，直到用户输入为空
    while (true) {
        string line = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (line.empty()) {
            break;
        }
        Set<string> urls = findQueryMatches(index, line);
        cout << "Found " << urls.size() << " matching pages" << endl;
        cout << urls << endl << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleaanToken的边界测试") {
    EXPECT_EQUAL(cleanToken("&^%#(("), "");
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("&J)*"), "j");
}

STUDENT_TEST("cleaanToken的真值测试") {
    EXPECT_EQUAL(cleanToken("%&Hkh^&*JJK9="), "hkh^&*jjk9");
}

STUDENT_TEST("cleaanToken的压力测试") {
    string s = "";
    for (int i = 0; i < 2000; i++) {
        s += "*";
    }
    for (int i = 0; i < 7; i++) {
        TIME_OPERATION(s.length(), cleanToken(s));
        s += s;
    }
}

STUDENT_TEST("gatherTokens的真值测试") {
    Set<string> expected = {"he", "is", "a", "professor"};
    EXPECT_EQUAL(gatherTokens("He is a **PROFESSOR** !"), expected);
}

STUDENT_TEST("gatherTokens的边界测试") {
    Set<string> expected = {};
    EXPECT_EQUAL(gatherTokens(""), expected);
}

STUDENT_TEST("gatherTokens的边界测试") {
    Set<string> expected = {};
    EXPECT_EQUAL(gatherTokens("? *"), expected);
}

STUDENT_TEST("gatherTokens的功能测试") {
    Set<string> tokens = gatherTokens("One *code two **CODE~ +tHree coDE- CO*de");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("code"));
    EXPECT(tokens.contains("co*de"));
}

STUDENT_TEST("gatherTokens的压力测试") {
    string s = "";
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 10; j++) {
            s += "a";
        }
        s += " ";
    }
    for (int i = 0; i < 5; i++) {
        TIME_OPERATION(s.length(), gatherTokens(s));
        s += s;
    }
}

STUDENT_TEST("buildIndex的边界测试") {
    Map<string, Set<string>> index;
    EXPECT_ERROR(buildIndex("web.txt", index));
}

STUDENT_TEST("buildIndex的压力测试") {
    Map<string, Set<string>> index;
    int npage = buildIndex("res/website.txt", index);
    TIME_OPERATION(npage, buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches的功能测试") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matches = findQueryMatches(index, "red fish +Blue -*gReen*");
    EXPECT_EQUAL(matches.size(),1);
}

STUDENT_TEST("findQueryMatches的压力测试") {
    Map<string, Set<string>> index;
    buildIndex("res/website.txt", index);
    string keyWords = "CS106B ";
    for (int i = 0; i < 10; i++) {
        keyWords += keyWords;
    }
    for (int i = 0; i < 5; i++) {
        TIME_OPERATION(keyWords.length(), findQueryMatches(index, keyWords));
        keyWords += keyWords;
    }
}
