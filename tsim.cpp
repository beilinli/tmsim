#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

string revBin(int);
void simAdd(string, string);
void simSub(string, string);
void simMult(string, string);
void simExp(string, string);

const char *usgMsg = "Usage: ./tsim [-add|-sub|-mult|-exp] [x] [y]";

/*
 * main - main routine for the simulator
 * argv: desired operation and two nonnegative operands
 */
int main(int argc, char **argv) {
    if (argc != 4) {
        cout << usgMsg << '\n';
        return 1;
    }

    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    string xVec = revBin(x);
    string yVec = revBin(y);

    cout << xVec << '\n';
    cout << yVec << '\n';

    if (!strcmp(argv[1], "-add")) {
        simAdd(xVec, yVec);
    } else if (!strcmp(argv[1], "-sub")) {
        simSub(xVec, yVec);
    } else if (!strcmp(argv[1], "-mult")) {
        simMult(xVec, yVec);
    } else if (!strcmp(argv[1], "-exp")) {
        simExp(xVec, yVec);
    } else {
        cout << usgMsg << '\n';
    }

    return 0;
}

/*
 * revBin - returns reverse binary representation of n.
 * requires: n >= 0
 */
string revBin(int n) {
    if (n == 0) return "0";

    string res;
    while (n > 0) {
        res += ((n & 1) + '0');
        n >>= 1;
    }

    return res;
}

void simAdd(string x, string y) {
    cout << "Not yet implemented.\n";
}

void simSub(string x, string y) {
    cout << "Not yet implemented.\n";
}

void simMult(string x, string y) {
    cout << "Not yet implemented.\n";
}

void simExp(string x, string y) {
    cout << "Not yet implemented.\n";
}
