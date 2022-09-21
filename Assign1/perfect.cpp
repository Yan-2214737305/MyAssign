/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find pairs of divisors
 * a loop iterates over all numbers from 1 to sqrt(n), testing for a
 * zero remainder from the division.
 */
long smarterSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor <= int(sqrt(n)); divisor++){
        if (n == 1) continue;
        else if (divisor * divisor == n || divisor == 1) {
            total += divisor;
        }
        else if (n % divisor == 0) {
            total += divisor + n / divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function finds perfect numbers by using Euclidean optimization.
 */
long findNthPerfectEuclid(long n) {
    long count = 1, num = 0;
    long long perfNum = 0;
    while(true){
        if (smarterSum(pow(2, count) - 1) == 1){
            perfNum = pow(2, count-1) * (pow(2, count) - 1);
            cout << "Found perfect number: " << perfNum << endl;
            num++;
        }
        if (num == n)break;
        count++;
    }
    return perfNum;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(30000, findPerfects(30000));
    TIME_OPERATION(60000, findPerfects(60000));
    TIME_OPERATION(120000, findPerfects(120000));
    TIME_OPERATION(240000, findPerfects(240000));
}

STUDENT_TEST("Confirm nagative value are false") {
    EXPECT_EQUAL(isPerfect(-1),false);
    EXPECT_EQUAL(isPerfect(-68),false);
    EXPECT_EQUAL(isPerfect(-100),false);
    EXPECT_EQUAL(isPerfect(-200046412),false);
}

STUDENT_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(25), 6);
}

STUDENT_TEST("Time trials of findPerfectsSmarter on doubling input sizes") {
    TIME_OPERATION(1000000, findPerfectsSmarter(1000000));
    TIME_OPERATION(2000000, findPerfectsSmarter(2000000));
    TIME_OPERATION(4000000, findPerfectsSmarter(4000000));
    TIME_OPERATION(8000000, findPerfectsSmarter(8000000));
}

STUDENT_TEST("The 1st, 4th, 5th, 7th, 8th are perfect numbers") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(1)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(4)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(5)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(7)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(8)));
}

STUDENT_TEST("Time trials of findNthPerfectEuclid on doubling input sizes") {
    TIME_OPERATION(5, findNthPerfectEuclid(5));
    TIME_OPERATION(7, findNthPerfectEuclid(7));
    TIME_OPERATION(8, findNthPerfectEuclid(8));
}
