/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 二路归并
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
#define version_judgeError

#ifdef version_judgeError
    //result尾部的值
    int lastResult = 0;
    //循环，直到两个队列都为空
    while (!(a.isEmpty() && b.isEmpty())) {
        int minVal = 0;
        //如果有一个队列为空，最小值取另一个队列最前元素
        if (a.isEmpty()) {
            minVal = b.dequeue();
        } else if (b.isEmpty()) {
            minVal = a.dequeue();
        } else {
            //如果两个队列全不为空，比较两个队列最前的元素，找出最小值
            minVal = min(a.peek(), b.peek());
            //找到该最小元素属于哪个队列并让其出队
            if (a.peek() == minVal) {
                a.dequeue();
            } else {
                b.dequeue();
            }
        }
        //判断，如果当前最小值小于result，报错
        if (minVal < lastResult && (!result.isEmpty())) {
            error("队列顺序错误，已结束！");
        }
        //将最小值放入result
        result.enqueue(minVal);
        lastResult = minVal;
    }
#endif

#ifdef version_noJudgeError
    //循环，直到两个队列都为空
    while (!(a.isEmpty() && b.isEmpty())) {
        int minVal = 0;
        //如果有一个队列为空，最小值取另一个队列最前元素
        if (a.isEmpty()) {
            minVal = b.dequeue();
        } else if (b.isEmpty()) {
            minVal = a.dequeue();
        } else {
            //如果两个队列全不为空，比较两个队列最前的元素，找出最小值
            minVal = min(a.peek(), b.peek());
            //找到该最小元素属于哪个队列并让其出队
            if (a.peek() == minVal) {
                a.dequeue();
            } else {
                b.dequeue();
            }
        }
        //将最小值放入result
        result.enqueue(minVal);
    }
 #endif
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

void recMultiMergeRec(const Vector<Queue<int>>& all,
                      int start,
                      int end,
                      Queue<int> &result) {
    /* TODO: Implement this function. */
    //base case
    if (end - start == 1) {
        result = all.get(start);
        return;
    } else {
        int cutLoc = (start + end) / 2;
        Queue<int> left, right;
        //inclusion
        recMultiMergeRec(all, start, cutLoc, left);
        //exclusion
        recMultiMergeRec(all, cutLoc, end, right);
        result = binaryMerge(left, right);
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 递归策略实现更有效的多路合并
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */
    recMultiMergeRec(all, 0, all.size(), result);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("Boundary testing of binaryMerge") {
    EXPECT_EQUAL(binaryMerge({}, {}), {});
    EXPECT_EQUAL(binaryMerge({1}, {}), {1});
    EXPECT_EQUAL(binaryMerge({}, {1,1}), {1,1});
    EXPECT_EQUAL(binaryMerge({-1,0}, {-1,1,1}), {-1, -1, 0, 1, 1});
    EXPECT_ERROR(binaryMerge({}, {1, -1}));
    EXPECT_ERROR(binaryMerge({-55, 656, 0}, {1, 1}));
    EXPECT_ERROR(binaryMerge({1, 2, 5, 4}, {1, 3, 4, 5, 6}));
    EXPECT_ERROR(binaryMerge({1, 6, 5}, {}));
}

STUDENT_TEST("Stress testing of binaryMerge") {
    for (int i = 1; i < 6; i++) {
        Queue<int> one;
        Queue<int> two;
        for (int j = 0; j < i * 4000; j++) {
            one.enqueue(j);
            two.enqueue(j);
        }
        TIME_OPERATION(one.size() + two.size(), binaryMerge(one, two));
    }
}

STUDENT_TEST("Boundary testing of naiveMultiMerge") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    all = {{1, 2, 3}, {}, {}, {}};
    expected = {1, 2, 3};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    all = {{1, 2, 3}, {-3}, {}, {25}};
    expected = {-3, 1, 2, 3, 25};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
    all = {{1, -2, 3}, {-3}, {}, {25}};
    EXPECT_ERROR(naiveMultiMerge(all));
}

STUDENT_TEST("Stress testing of naiveMultiMerge") {
    Vector<Queue<int>> all;
    for (int i = 1; i < 8; i++) {
        Queue<int> one;
        int size = 0;
        for (int j = 0; j < 10000; j++) {
            one.enqueue(j);
        }
        all.add(one);
        for (const Queue<int> &item : all) {
            size += item.size();
        }
        TIME_OPERATION(size, naiveMultiMerge(all));
    }
}

STUDENT_TEST("Stress testing of recMultiMerge") {
    Vector<Queue<int>> all;
    for (int i = 1; i < 8; i++) {
        Queue<int> one;
        int size = 0;
        for (int j = 0; j < 10000; j++) {
            one.enqueue(j);
        }
        all.add(one);
        for (const Queue<int> &item : all) {
            size += item.size();
        }
        TIME_OPERATION(size, recMultiMerge(all));
    }
}
