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

#define version_b

#ifdef version_b
void recursiveSolver(Vector<int> &rabbit,
                     Vector<int> &sequence,
                     int order,
                     int lifeSpan) {
    //Base Case
    if (order == 0) {
        return;
    } else {
        //生长/生育过程
        int rabbitSize = rabbit.size();
        for (int i = 0; i < rabbitSize; i++) {
            int current = rabbit[i];
            //寿命结束，兔子死亡
            if (current == lifeSpan) {
                rabbit.remove(i);
                rabbitSize--;
                i--;
                continue;
            }
            //成熟期兔子，生育
            if (current != 0) {
                rabbit.add(0);
            }
            //兔子年纪增大一个月
            rabbit[i]++;
        }
        sequence.add(rabbitSize);
        recursiveSolver(rabbit, sequence, order - 1, lifeSpan);
    }
    return;
}

//包装函数
Vector<int> Fibonacci (int order, int lifeSpan) {
    Vector<int> sequence = {};
    Vector<int> rabbit = {0};
    recursiveSolver(rabbit, sequence, order + 1, lifeSpan);
    return sequence;
}
#endif

#ifdef version_a
void recursiveSolver(Vector<int> &sequence, int order, int lifeSpan) {
    if (order == 0) {
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
        recursiveSolver(sequence, order - 1, lifeSpan);
    }
}

//包装函数：初始化、后处理数列
Vector<int> Fibonacci (int order, int lifeSpan) {
    Vector<int> sequence = {0, 1};
    recursiveSolver(sequence, order, lifeSpan);
    sequence.remove(0);
    return sequence;
}
#endif

//主函数
int main () {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    //繁衍月数
    int order = 5;
    //兔子寿命（月）
    int lifeSpan = 3;
    cout << "寿命为" << lifeSpan << "个月的兔子，" ;
    cout << "繁衍了" << order << "个月，其种群数量变化趋势：" << endl;
    cout << Fibonacci(order, lifeSpan) << endl;
    return 0;
}
