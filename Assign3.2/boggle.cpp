/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * 计算传入单词的分数
 */
int points(string str) {
    int len = str.length();
    if (len > 3) {
        return len - 3;
    }
    return 0;
}

/* 寻找当前位置的所有相邻位置
 */
Set<GridLocation> generateNeighbors(const Grid<char>& board, const GridLocation &cur) {
    Set<GridLocation> neighbors;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            GridLocation next = cur;
            next.row += i;
            next.col += j;
            if (board.inBounds(next)) {
                neighbors.add(next);
            }
        }
    }
    return neighbors;
}

// 返回Grid中所有元素的坐标
Set<GridLocation> findLocation (const Grid<char> &board) {
    Set<GridLocation> locSet;
    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numRows(); j++) {
            GridLocation loc = {i, j};
            locSet.add(loc);
        }
    }
    return locSet;
}

/*
 * 通过递归探索拼字盘来查找所有单词并计算点数
 */
void scoreBoardRec(const Grid<char>& board,
                  const Lexicon& lex,
                  Set<GridLocation>& currentGrid,
                  Set<GridLocation>& visited,
                  string word,
                  Set<string> &result) {
    //Base case
    if (currentGrid.difference(visited).isEmpty()) {
        return;
    } else {
        for (const GridLocation &item : currentGrid) {
            //choose
            word += board[item.row][item.col];
            visited.add(item);
            auto next = generateNeighbors(board, item).difference(visited);
            //explore
            if (lex.containsPrefix(word)){
                if (lex.contains(word)) {
                    result.add(word);
                }
                scoreBoardRec(board, lex, next, visited, word, result);
            }
            //unchoose
            word.erase(word.end() - 1);
            visited.remove(item);
        }
    }
    return;
}

/*
 * 探索拼字盘来查找所有单词并计算点数
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int scores = 0;
    string word;
    Set<string> result;
    Set<GridLocation> iniGrid = findLocation(board);
    Set<GridLocation> visited;
    scoreBoardRec(board, lex, iniGrid, visited, word, result);
    for (const string &item : result) {
        scores += points(item);
    }
    return scores;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Boundary testing of generateNeighbors") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};
    EXPECT_EQUAL(generateNeighbors(board, {0,0}), {{0,1},{1,1},{1,0}});
}

STUDENT_TEST("Boundary testing of scoreBoard") {
    Grid<char> board = {};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);

    board = {{'Z'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);

    board = {{'a'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);

    board = {{'a', 's'},
             {'H', 'a'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}
