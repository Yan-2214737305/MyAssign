/**
 *  斐波那契数列的兔子背景：一对兔子成年后每月生下一对兔子（一公一母）
 * 这对兔子一个月后成熟，之后也每个月生一对兔子
 * 若成熟兔子永远不死，则为斐波那契数列（1 1 2 3 5 8）
 * 若假设一对兔子寿命为k,即生下k-1对小兔子后死去，该如何得到数列？
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "vector.h"
using namespace std;

void recursiveSolver(Vector<int> &sequence, int n, int lifeSpan) {
    if (n == 0) {
        return;
    }
    else {
        int month = sequence.size() - 1;
        int num = sequence.get(month) + sequence.get(month - 1);
        if (month == lifeSpan) {
            num -= 1;
        }
        else if (month > lifeSpan) {
            num -= sequence.get(month - lifeSpan);
        }
        sequence.add(num);
        recursiveSolver(sequence, n - 1, lifeSpan);
    }
}

//包装函数：初始化、后处理数列
Vector<int> Fibonacci (int n, int lifeSpan) {
    Vector<int> sequence = {0, 1};
    recursiveSolver(sequence, n, lifeSpan);
    sequence.remove(0);
    return sequence;
}

//主函数
int main () {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    //繁衍月数
    int n = 20;
    //兔子寿命（月）
    int lifeSpan = 100;
    cout << "寿命为" << lifeSpan << "个月的兔子，" ;
    cout << "繁衍了" << n << "个月，其种群数量变化趋势：" << endl;
    cout << Fibonacci(n, lifeSpan) << endl;
    return 0;
}
