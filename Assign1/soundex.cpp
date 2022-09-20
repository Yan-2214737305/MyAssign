/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* This function is used to build the soundex rules between
 * the characters and numbers.
 */
int codeRules(char sonStr) {
    string transRules[] = {
        "AEIOUHWY",
        "BFPV",
        "CGJKQSXZ",
        "DT",
        "L",
        "MN",
        "R",
    };
    int digit = -1;
    for (int i = 0; i < 7; i++) {
        if(stringContains(transRules[i],charToString(sonStr))) {
            digit = i;
            break;
        }
    }
    return digit;
}

/* This function is used to re-encode the input string
 * according to soundex rules.
 */
string soundexRecode(string s) {
    s = toUpperCase(s);
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        result += integerToString(codeRules(s[i]));
    }
    return result;
}

/* This function is used to merge adjacent repeating numbers.
 */
string mergeNum(string num) {
    int charPos = 0;
    if(num.empty()) return num;
    while(charPos < num.length() - 1) {
        if (num[charPos] == num[charPos + 1]) {
            num.erase(num.begin() + charPos);
            charPos = 0;
        }
        else{
            charPos++;
        }
    }
    return num;
}

/* This function is used to replace the first number
 * with the capital letter.
 */
string replaceCapital(string capitalLetter, string num) {
    if(!num.empty()) {
        num.erase(num.begin());
    }
    return (toUpperCase(capitalLetter) + num);
}

/* This function is used to filter out a character
 * in the string.
 */
string filterChar(string s, char drop) {
    if(s.empty()) return s;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == drop) {
            s.erase(s.begin() + i);
            i--;
        }
    }
    return s;
}

/* This function is used to complete the soundexCode.
 */
string completeCode(string s, int remainLen) {
    if (s.length() < remainLen) {
        s.append("0");
        s = completeCode(s, remainLen);
    }
    else {
        s.erase(s.begin() + remainLen, s.end());
    }
    return s;
}

/* Calculating the soundex code.
 */
string soundex(string s) {
    if (s.empty()) return s;
    string soundexCode = "";
    s = removeNonLetters(s);
    soundexCode = mergeNum(soundexRecode(s));
    soundexCode = replaceCapital(charToString(s[0]),soundexCode);
    soundexCode = filterChar(soundexCode, integerToChar(0));
    soundexCode = completeCode(soundexCode, 4);
    return soundexCode;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    string surName = "";
    string soundexCode = "";

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    while(true) {
        Vector<string> sameNames;

        surName = getLine("Enter a surname (RETURN to quit):");
        if (surName.empty()) {
            cout << "All done!" << endl;
            break;
        }
        soundexCode = soundex(surName);
        cout << "Soundex code is " << soundexCode << endl;
        for(int i = 0; i < databaseNames.size(); i++) {
            if (soundexCode == soundex(databaseNames[i])) {
                sameNames.add(databaseNames[i]);
            }
        }
        sameNames.sort();
        cout << "Matches from database: ";
        cout << sameNames << endl;
        cout << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


//PROVIDED_TEST("Sample inputs from handout") {
//    EXPECT_EQUAL(soundex("Curie"), "C600");
//    EXPECT_EQUAL(soundex("O'Conner"), "O256");
//}

//PROVIDED_TEST("hanrahan is in lowercase") {
//    EXPECT_EQUAL(soundex("hanrahan"), "H565");
//}

//PROVIDED_TEST("DRELL is in uppercase") {
//    EXPECT_EQUAL(soundex("DRELL"), "D640");
//}

//PROVIDED_TEST("Liu has to be padded with zeros") {
//    EXPECT_EQUAL(soundex("Liu"), "L000");
//}

//PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
//    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
//}

//PROVIDED_TEST("Au consists of only vowels") {
//    EXPECT_EQUAL(soundex("Au"), "A000");
//}

//PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
//    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
//}

//PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
//    EXPECT_EQUAL(soundex("Jackson"), "J250");
//}

//PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
//    EXPECT_EQUAL(soundex("Schwarz"), "S620");
//}

//PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
//    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
//}

//PROVIDED_TEST("Wharton begins with Wh") {
//    EXPECT_EQUAL(soundex("Wharton"), "W635");
//}

//PROVIDED_TEST("Ashcraft is not a special case") {
//    // Some versions of Soundex make special case for consecutive codes split by hw
//    // We do not make this special case, just treat same as codes split by vowel
//    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
//}

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "#$'156";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Confirm codeRules get the digit of 'A','Z','R','Y'") {
    EXPECT_EQUAL(codeRules('A'),0);
    EXPECT_EQUAL(codeRules('Z'),2);
    EXPECT_EQUAL(codeRules('R'),6);
    EXPECT_EQUAL(codeRules('Y'),0);
}

STUDENT_TEST("Confirm the encoding of the string") {
    EXPECT_EQUAL(soundexRecode("Schwarz"),"2200062");
    EXPECT_EQUAL(soundexRecode("rdTlMoP"),"6334501");
    EXPECT_EQUAL(soundexRecode(""),"");
}

STUDENT_TEST("Confirm the merging of the string") {
    EXPECT_EQUAL(mergeNum("2230006"),"2306");
    EXPECT_EQUAL(mergeNum("953000"),"9530");
    EXPECT_EQUAL(mergeNum("222222"),"2");
    EXPECT_EQUAL(mergeNum(""),"");
}

STUDENT_TEST("Replacing the first number with the capital letter") {
    EXPECT_EQUAL(replaceCapital("A","2306"),"A306");
    EXPECT_EQUAL(replaceCapital("S","953"),"S53");
    EXPECT_EQUAL(replaceCapital("","953"),"53");
    EXPECT_EQUAL(replaceCapital("S",""),"S");
}

STUDENT_TEST("Filtering out a character in the string") {
    EXPECT_EQUAL(filterChar("2301600",'0'),"2316");
    EXPECT_EQUAL(filterChar("",'1'),"");
    EXPECT_EQUAL(filterChar("S0SC4",'S'),"0C4");
}

STUDENT_TEST("Complete the soundexCode") {
    EXPECT_EQUAL(completeCode("A1653216",4),"A165");
    EXPECT_EQUAL(completeCode("fsdljlk",0),"");
    EXPECT_EQUAL(completeCode("JK7",6),"JK7000");
}

STUDENT_TEST("Calculating the soundex code") {
    EXPECT_EQUAL(soundex("Curie"),"C600");
    EXPECT_EQUAL(soundex(""),"");
    EXPECT_EQUAL(soundex("O'Conner"),"O256");
    EXPECT_EQUAL(soundex("+*dAd5 &*64"),"D300");
    EXPECT_EQUAL(soundex("Angelou"),"A524");
}
