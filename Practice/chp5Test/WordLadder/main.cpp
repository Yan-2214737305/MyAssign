/**
 * 词梯（Word Ladder）是刘易斯·卡罗尔发明的一种单词游戏。
 * 游戏会给出一个开始单词和结束单词。玩家需要更改开始单词中的某个字母，
 * 获得一个新词，计作一步。然后玩家需要再次更改所得新词中的某个字母，
 * 获得另一个新词。最终获得结束单词，游戏结束。
 */

#include "main.h"
#include <fstream>
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;


///**
// * @brief findWordLadder - 实现一个单词阶梯查找算法(基于薛老师，不太严谨，对于十几万的英文单词，理论上有非常多)
// *
// * 例如：开始单词 cold 和结束单词 warm 可以得到如下结果。
// *
// * COLD -> CORD -> CARD -> WARD -> WARM
// *
// * @param start
// * @param end
// * @return 将计算出的单词阶梯以队列的形式保存
// */
//Queue<Stack<string>> findWordLadder(string start, string end) {
//    // TODO: 删除以下两行代码，实现这个函数。
//    //定义队列代表总词梯，栈代表分支
//    Queue<Stack<string>> trunk;
//    Stack<string> branch;
//    branch.push(start);
//    trunk.enqueue(branch);
//    //存放找到的词梯
//    Queue<Stack<string>> saveLadder;
//    //遍历过的结点
//    Set<string> visited;
//    int floor = 1;
//    //循环，直到队列为空
//    while (!trunk.isEmpty()) {
//        //提取队列中的分支和结点
//        Stack currentBranch = trunk.dequeue();
//        string currentNode = currentBranch.peek();
//        //当前分支已经找到最终结果
//        if (currentNode == end) {
//            saveLadder.enqueue(currentBranch);
//        }
//        if( currentBranch.size() > floor) {
//            floor++;
//        }
//        //寻找当前结点的关联结点
//        Set<string> nextNodes = nextWords(currentNode);
//        //禁忌
//        visited = visitedWords(currentBranch);
//        //将新的子结点存入
//        for (const string &node : nextNodes) {
//            if (!visited.contains(node)) {
//                Stack nextBranch = currentBranch;
//                nextBranch.push(node);
//                trunk.enqueue(nextBranch);
//            }
//        }
//    }
//    return saveLadder;

//}


/**
 * @brief findWordLadder - 实现一个单词阶梯查找算法（想法来源于禁忌算法，生成路径最短的所有词梯，严谨，解的个数有限）
 *
 *
 * 例如：开始单词 cold 和结束单词 warm 可以得到如下结果。
 *
 * COLD -> CORD -> CARD -> WARD -> WARM
 *
 * @param start
 * @param end
 * @return 将计算出的单词阶梯以队列的形式保存
 */
Queue<Stack<string>> findWordLadder(string start, string end) {
    // TODO: 删除以下两行代码，实现这个函数。
    //定义队列代表总词梯，栈代表分支
    Queue<Stack<string>> trunk;
    Stack<string> branch;
    branch.push(start);
    trunk.enqueue(branch);
    //存放找到的词梯
    Queue<Stack<string>> saveLadder;
    //遍历过的结点，防止走回头路
    Set<string> taboo;
    Set<string> visited;
    //禁忌层数
    int floor = 1;
    //循环，直到队列为空
    while (!trunk.isEmpty()) {
        //提取队列中的分支和结点
        Stack currentBranch = trunk.dequeue();
        string currentNode = currentBranch.peek();
        //当前分支已经找到最终结果
        if (currentNode == end) {
            saveLadder.enqueue(currentBranch);
        }
        //寻找当前结点的关联结点
        Set<string> nextNodes = nextWords(currentNode);
        //禁忌
        if (currentBranch.size() > floor) {
            taboo += visited;
            floor++;
        }
        taboo.add(currentNode);
        //将新的子结点存入
        for (const string &node : nextNodes) {
            if (!taboo.contains(node)) {
                Stack nextBranch = currentBranch;
                nextBranch.push(node);
                trunk.enqueue(nextBranch);
                visited.add(node);
            }
        }
    }
    return saveLadder;

}

// ******************* 无需修改以下代码 **********************

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    // 初始化单词表
    ifstream fin("res/EnglishWords.txt");
    string word;
    while (fin >> word) {
        dictionary.add(word);
    }

    // 程序交互部分
    while (true) {
        string start = getLine("输入开始单词: ");
        if (!start.size())
            break;
        string end = getLine("输入结束单词: ");
        if (!end.size())
            break;

        Queue<Stack<string>> wordLadder = findWordLadder(toLowerCase(start), toLowerCase(end));

        if (wordLadder.isEmpty()) {
            cout << "未找到单词阶梯！" << endl;
            continue;
        }
        else {
            cout << "以下是从 " << start << " 到 " << end << " 生成的词梯：" << endl;
            while (!wordLadder.isEmpty()) {
                cout << wordLadder.dequeue() << endl;
            }
            cout << "游戏结束！" << endl;
        }
    }

    cout << "未获取输入，游戏结束！" << endl;
    return 0;
}

/**
 * @brief visitedWords - 由于栈无法遍历查询，转换为集合类型
 * @param currentladder
 * @return 以集合的形式返回栈中包含的所有单词
 */
Set<string> visitedWords(Stack<string> currentLadder) {
    Set<string> visited;
    while (!currentLadder.isEmpty()) {
        visited.add(currentLadder.pop());
    }
    return visited;
}

/**
 * @brief nextWords - 根据提供的 currentWord 找出与其仅一字之差的单词。
 *
 * 例如：cat 对应的单词有 bat、eat、cot、cab、car 等等。
 *
 * @param currentWord
 * @return 将所有找出的单词以集合的形式返回
 */
Set<string> nextWords(string currentWord) {
    Set<string> neighboringWords;
    for (int i = 0; i < currentWord.length(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            string newWord = currentWord;
            newWord[i] = ch;
            if (dictionary.contains(newWord)) {
                neighboringWords.add(newWord);
            }
        }
    }
    return neighboringWords;
}

