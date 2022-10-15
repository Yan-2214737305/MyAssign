/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/* 递归，从字符串中提取括号 */
void operatorsFromRec(const string &str, const Set<char> &bracket, int index, string &result) {
    /* TODO: Implement this function. */
    //base case
    if (index == str.length()) {
        return;
    } else {
        //recursive case
        //若为括号，添加到result
        if (bracket.contains(str.at(index))) {
            result.append(charToString(str.at(index)));
        }
        operatorsFromRec(str, bracket, index + 1, result);
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 递归，从字符串中提取括号（包装函数）
 */
string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    Set<char> bracket = {'(', ')', '[', ']', '{', '}'};
    string result;
    operatorsFromRec(str, bracket, 0, result);
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 判断只包含括号字符的字符串是否平衡
 */
bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    //base case
    if (ops.empty()) {
        return true;
    } else {
        //recursive case
        //"()"exist, remove it and check the substring
        if (stringContains(ops, "()")) {
            ops.erase(stringIndexOf(ops, "()"), 2);
            return operatorsAreMatched(ops);
        } else if (stringContains(ops, "[]")) {
            ops.erase(stringIndexOf(ops, "[]"), 2);
            return operatorsAreMatched(ops);
        } else if (stringContains(ops, "{}")) {
            ops.erase(stringIndexOf(ops, "{}"), 2);
            return operatorsAreMatched(ops);
        }
        return false;
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("Boundary testing of operatorsFrom") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("jsalhfjhH"), "");
    EXPECT_EQUAL(operatorsFrom("<(>)}{[]"), "()}{[]");
}

STUDENT_TEST("Stress testing of operatorsFrom") {
    string str = "(a)  []}}{";
    for (int i = 0; i < 10; i++) {
        str += str;
        TIME_OPERATION(str.length(), operatorsFrom(str));
    }
}

STUDENT_TEST("Boundary testing of operatorsAreMatched") {
    EXPECT(operatorsAreMatched(""));
    EXPECT(!operatorsAreMatched("[])({}"));
    EXPECT(!operatorsAreMatched("()[{}(])"));
    EXPECT(!operatorsAreMatched("()[{  }]"));
}

STUDENT_TEST("Stress testing of operatorsAreMatched") {
    string str = "{[{}()]()[]}{[{}()]()[]}";
    for (int i = 0; i < 8; i++) {
        str += str;
        TIME_OPERATION(str.length(), operatorsAreMatched(str));
    }
}
