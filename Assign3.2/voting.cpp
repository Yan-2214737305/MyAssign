// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

//判断目标区块与子集组成联盟时，其是否为关键选票
bool isKeyBallot (int sumCoalition, int target, int sumBallot) {
    if (sumCoalition <= sumBallot / 2 && sumCoalition + target >= sumBallot / 2 + 1) {
        return true;
    }
    return false;
}

// 递归找到除该block外，其他blocks的子集
int computePowerIndexesRec(const Vector<int>& blocks,
                           int sumCoalition,
                           int sumBallot,
                            int index,
                            int target) {
    int keyBallot = 0;
    if (index == blocks.size()) {
        int targetVal = blocks.get(target);
        if (isKeyBallot(sumCoalition, targetVal, sumBallot)) {
            return keyBallot + 1;
        }
    } else {
        if (sumCoalition < sumBallot / 2 + 1) {
            if (index == target) {
                keyBallot += computePowerIndexesRec(blocks, sumCoalition, sumBallot, index + 1, target);
            } else {
                //inclusion
                sumCoalition += blocks.get(index);
                keyBallot += computePowerIndexesRec(blocks, sumCoalition, sumBallot, index + 1, target);
                //exclusion
                sumCoalition -= blocks.get(index);
                keyBallot += computePowerIndexesRec(blocks, sumCoalition, sumBallot, index + 1, target);
            }
        }
    }
    return keyBallot;
}

// 该函数计算关键选票，以确定每个区块的投票权
Vector<int> computePowerIndexes(Vector<int>& blocks) {
    Vector<int> result;
    Vector<int> keyBallot(blocks.size(), 0);
    int sum = 0;
    int sumBallot = 0;
    for (const int &item : blocks) {
        sumBallot += item;
    }
    //取出目标区块
    for (int i = 0; i < blocks.size(); i++) {
        //递归探索不包含该区块的子集
        keyBallot[i] = computePowerIndexesRec(blocks, 0, sumBallot, 0, i);
        sum += keyBallot[i];
    }
    //循环计算Banzhaf影响力
    for (const int &item : keyBallot) {
        result.add(item * 100 / sum);
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 6; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

STUDENT_TEST("Boundary testing of isKeyBallot") {
    EXPECT(isKeyBallot(0, 1, 1));
    EXPECT(isKeyBallot(1, 1, 2));
    EXPECT(!isKeyBallot(1, 0, 2));
    EXPECT(isKeyBallot(50, 1, 100));
}

STUDENT_TEST("Stress testing of computePowerIndexes") {
    Vector<int> blocks;
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < 4; i++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}



