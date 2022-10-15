/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 生成order阶的Sierpinski三角形
 */
int drawSierpinskiTriangleRec(GWindow& window, GPoint one, GPoint two, GPoint three, int order, int num) {
    /* TODO: Implement this function. */
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return num + 1;
    } else {
        GPoint midone((one.x + two.x) / 2, (one.y + two.y) / 2);
        GPoint midtwo((two.x + three.x) / 2, (two.y + three.y) / 2);
        GPoint midthree((three.x + one.x) / 2, (three.y + one.y) / 2);
        //firstTriangle
        num += drawSierpinskiTriangleRec(window, midone, midtwo, two, order - 1, num);
        //secondTriangle
        num += drawSierpinskiTriangleRec(window, midtwo, midthree, three, order - 1, num);
        //thirdTriangle
        num += drawSierpinskiTriangleRec(window, midthree, midone, one, order - 1, num);
    }
    return num;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 * 生成order阶的Sierpinski三角形
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    int num = 0;
    drawSierpinskiTriangleRec(window, one, two, three, order, num);
    return num;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

