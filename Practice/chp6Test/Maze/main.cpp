#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

int main()
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    solveMaze(maze);

    cout << endl << "All done!" << endl;
    return 0;
}


// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    Grid<bool> g;
    GridLocation loc;
    Set<GridLocation> set;
    Stack<GridLocation> path;
    string str;

    set = generateValidMoves(g, loc);
    validatePath(g, path);
    readMazeFile(str, g);
    readSolutionFile(str, path);
    path = solveMaze(g);
}
