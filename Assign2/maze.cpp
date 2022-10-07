// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* 寻找当前位置的所有相邻位置
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    //南北方向移动
    for (int i = -1; i <= 1; i++) {
        //东西方向移动
        for (int j = -1; j <= 1; j++) {
            if (abs(i) + abs(j) != 1) {
                continue;
            }
            GridLocation next = cur;
            next.row = next.row + i;
            next.col = next.col + j;
            //边界判断
            if (next.row > (maze.numRows() - 1) || next.row < 0) {
                continue;
            }
            if (next.col > (maze.numCols() - 1) || next.col < 0) {
                continue;
            }
            //通道判断
            else if (maze[next]) {
                neighbors.add(next);
            }
        }
    }
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Set<GridLocation> visited;

    if (path.peek() != mazeExit) {
        error("路径终点不是迷宫出口!");
    }
    /* TODO: Fill in the remainder of this function. */
    //循环，直到栈空
    while (true) {
        //提取栈顶元素
        GridLocation back = path.pop();
        if (path.isEmpty()) {
            if (back.row != 0 || back.col != 0) {
                error("路径起点不是迷宫入口!");
            }
            else break;
        }
        if (!generateValidMoves(maze, path.peek()).contains(back)) {
            error("移动路径不符合规则!");
        }
        if (visited.contains(back)) {
            error("路径出现了循环!");
        }
        visited.add(back);
    }
}

/**
 * @brief visitedWords - 由于栈无法遍历查询，转换为集合类型
 * @param currentladder
 * @return 以集合的形式返回栈中包含的位置
 */
Set<GridLocation> visitedLoc(Stack<GridLocation> currentPath) {
    Set<GridLocation> visited;
    while (!currentPath.isEmpty()) {
        visited.add(currentPath.pop());
    }
    return visited;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    //画出迷宫地图
    MazeGraphics::drawGrid(maze);
    //创建空队列
    Queue<Stack<GridLocation>> triedPath;
    //创建仅包含入口的路径，添加到队列
    path.push({0,0});
    triedPath.enqueue(path);

    //循环，直到队列为空
    while (!triedPath.isEmpty()) {
        //取出当前路径
        Stack currentPath = triedPath.dequeue();
        GridLocation currentLoc = currentPath.peek();
        //标记当前路径
        MazeGraphics::highlightPath(currentPath, "green", 5);
        //到达出口，结束
        if (currentLoc.row == maze.numRows() - 1 && currentLoc.col == maze.numCols() - 1) {
            return currentPath;
        }
        //根据路径末端，找到所有可能的位置
        Set<GridLocation> nextLoc = generateValidMoves(maze, currentLoc);
        //不走回头路，将经过的位置放入集合
        auto visited = visitedLoc(currentPath);
        for (GridLocation item : nextLoc) {
            //如果之前没走过，将位置添加入副本，并添加到队列
            if (!visited.contains(item)) {
                Stack copyPath = currentPath;
                copyPath.push(item);
                triedPath.enqueue(copyPath);
            }
        }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves在无墙1x1迷宫的边界测试") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};
    EXPECT(generateValidMoves(maze, center).isEmpty());
}

STUDENT_TEST("generateValidMoves在有墙1x1迷宫的边界测试") {
    Grid<bool> maze = {{false}};
    GridLocation center = {0, 0};
    EXPECT(generateValidMoves(maze, center).isEmpty());
}

STUDENT_TEST("generateValidMoves在有墙3x3迷宫的边界测试") {
    Grid<bool> maze = {{true,false,true},
                       {false,true,false},
                       {true,false,true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {};
    EXPECT(generateValidMoves(maze, center).isEmpty());
}

STUDENT_TEST("generateValidMoves在有墙3x3迷宫的功能测试") {
    Grid<bool> maze = {{true,false,true},
                       {true,true,false},
                       {false,true,true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{1, 0}, {2, 1}};
    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves在nxn迷宫的压力测试") {
    GridLocation side = {1, 1};
    for (int i = 10; i < 3000; i += 1000) {
        Grid<bool> maze(i,i,true);
        TIME_OPERATION(i, generateValidMoves(maze, side));
    }
}

STUDENT_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/17x37.maze", maze);
    readSolutionFile("res/17x37.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, false},
                       {false, true, true}};
    Stack<GridLocation> not_end_at_exit = { {0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 0} };
    Stack<GridLocation> not_begin_at_entry = { {0, 1}, {0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2} };
    Stack<GridLocation> go_through_wall = { {0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2} };
    Stack<GridLocation> teleport = { {0, 0}, {1, 0}, {1, 1}, {2, 2} };
    Stack<GridLocation> revisit = { {0, 0}, {1, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

STUDENT_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    Stack<GridLocation> realSoln;
    readSolutionFile("res/17x37.soln", realSoln);

    EXPECT_EQUAL(soln, realSoln);
}
