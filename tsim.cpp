#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
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

int binToDec(string bin) {
    int len = bin.size();
    int n = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (bin[i] == ' ') continue;
        n = (n << 1) | (bin[i] - '0');
    }
    return n;
}

/*
 * printTape - prints contents of tape marked at tape position
 */
void printTape(string tape, int pos) {
    int len = tape.size();
    if (pos < 0) {
        cout << "[ ]" + tape;
    } else if (pos >= len) {
        cout << tape + "[ ]";
    } else {
        for (int i = 0; i < len; i++) {
            if (i == pos) cout << "[";
            cout << tape[i];
            if (i == pos) cout << "]";
        }
    }
    cout << "\n";
}

string itos(int n) {
    stringstream ss;
    ss << n;
    return ss.str();
}

/*
 * simAdd - simulates addition on a 3-tape TM and prints trace
 */
void simAdd(string x, string y) {
    int tapePos = 0; // common tape position in this algorithm
    int curState = 0; // represents carry status
    int finalState = 2;

    // allocate blank symbol display at ends of tapes
    int len = max(x.size(), y.size()) + 1;
    string res(len, ' ');
    string xs(len - x.size(), ' ');
    string ys(len - y.size(), ' ');
    x += xs, y += ys;

    // print initial configuration (state, marked tapes)
    cout << "INITIAL CONFIGURATION\n";
    cout << "state 0\n";
    cout << "Tape 1: ";
    printTape(x, tapePos);
    cout << "Tape 2: ";
    printTape(y, tapePos);
    cout << "Tape 3: [ ]\n\n";

    while (curState != finalState) {
        // transition function
        char xc = x[tapePos];
        char yc = y[tapePos];

        cout << "TRANSITION\n";
        // old state, read
        cout << "State " << curState << ", (" << xc << "," << yc << ", ) ==> ";

        if (xc == ' ' && yc == ' ') {
            res[tapePos] = curState ? '1' : ' ';
            curState = finalState;
        } else {
            int b1 = (xc == ' ') ? 0 : (xc - '0');
            int b2 = (yc == ' ') ? 0 : (yc - '0');
            int b3 = (curState + b1 + b2) & 1;
            res[tapePos] = b3 + '0';

            curState = (curState + b1 + b2 > 1);
        }

        // new state, write
        cout << "State " << curState << ", (" << xc << "," << yc << ","
                << res[tapePos] << "), R\n";

        tapePos++;
        // print tape
        cout << "Tape 1: ";
        printTape(x, tapePos);
        cout << "Tape 2: ";
        printTape(y, tapePos);
        cout << "Tape 3: ";
        printTape(res, tapePos);
        cout << "\n";
    }

    cout << "RESULT\n";
    cout << "Tape: " << res << "\n";
    cout << "Result: " << binToDec(res) << "\n";
}

void simSub(string x, string y) {
    int tapePos = 0; // common tape position in this algorithm
    int curState = 0; // represents carry status
    int finalState = 2;

    // allocate blank symbol display at ends of tapes
    int len = max(x.size(), y.size()) + 1;
    string res(len, ' ');
    string xs(len - x.size(), ' ');
    string ys(len - y.size(), ' ');
    x += xs, y += ys;

    // print initial configuration (state, marked tapes)
    cout << "INITIAL CONFIGURATION\n";
    cout << "state 0\n";
    cout << "Tape 1: ";
    printTape(x, tapePos);
    cout << "Tape 2: ";
    printTape(y, tapePos);
    cout << "Tape 3: [ ]\n\n";

    while (curState != finalState) {
        // transition function
        char xc = x[tapePos];
        char yc = y[tapePos];

        cout << "TRANSITION\n";
        // old state, read
        cout << "State " << curState << ", (" << xc << "," << yc << ", ) ==> ";

        if (xc == ' ' && yc == ' ') {
            res[tapePos] = ' ';
            curState = finalState;
        } else {
            int b1 = (xc == ' ') ? 0 : (xc - '0');
            int b2 = (yc == ' ') ? 0 : (yc - '0');
            int b3 = (b1 - b2 - curState) & 1;
            res[tapePos] = b3 + '0';

            curState = (b1 - b2 - curState < 0);
        }

        // new state, write
        cout << "State " << curState << ", (" << xc << "," << yc << ","
                << res[tapePos] << "), R\n";

        tapePos++;
        // print tape
        cout << "Tape 1: ";
        printTape(x, tapePos);
        cout << "Tape 2: ";
        printTape(y, tapePos);
        cout << "Tape 3: ";
        printTape(res, tapePos);
        cout << "\n";
    }

    cout << "RESULT\n";
    cout << "Tape: " << res << "\n";
    cout << "Result: " << binToDec(res) << "\n";
    cout << "Not yet implemented.\n";
}

void simMult(string x, string y) {
    cout << "Not yet implemented.\n";
}

void simExp(string x, string y) {
    cout << "Not yet implemented.\n";
}
