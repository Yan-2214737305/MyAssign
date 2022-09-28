#include <iostream>
#include <fstream>
#include "console.h"
#include "testing/SimpleTest.h"

/* 回文识别程序 */
bool isPalindrome(std::string &s, int start, int end) {
    //边界条件
    if(end - start <= 1) {
        return true;
    }
    //递归判断
    if(s[start] == s[end-1]) {
        start++;
        end--;
        return isPalindrome(s,start,end);
    }
    else {
        return false;
    }
}

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    //文件IO
    int count = 0;
    std::string line;
    std::ifstream fin("res/EnglishWords.txt");
    std::ofstream fout("res/Palindrome.txt");
    while (getline(fin, line)) {
        if (isPalindrome(line, 0, line.length())) {
            std::cout << "find: " << ++count << std::endl;
            fout << line << std::endl;
        }
    }
    std::cout << "Find success!" << std::endl;

    //命令行IO
//    std::string s = "level";
//    std::cout << isPalindrome(s,0,s.length()) << std::endl;

    return 0;
}

STUDENT_TEST("回文识别函数真值测试") {
    std::string s = "l";
    EXPECT(isPalindrome(s,0,s.length()));
    s = "level";
    EXPECT(isPalindrome(s,0,s.length()));
    s = "non";
    EXPECT(isPalindrome(s,0,s.length()));
}

STUDENT_TEST("回文识别函数压力测试") {
    std::string s = "l";
    for (int i = 5; i>0 ; i--) {
        for (int j = 0; j<5000; j++) {
            s.append("ll");
        }
        TIME_OPERATION(s.length(), isPalindrome(s,0,s.length()));
    }
}
