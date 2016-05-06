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

#define MODE_PLUS 1
#define MODE_MINUS 0

const char *usgMsg = "Usage: ./tsim [-add|-sub|-mult|-exp] [x] [y]";

/*
 * main - simulator main routine reads arguments and calls appropriate function.
 * argv: desired operation and two nonnegative operands.
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

/*
 * binToDec - returns integer whose reverse binary representation is bin.
 */
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
 * printTape - prints contents of tape marked at tape position.
 * requires: 0 <= pos < tape.size()
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
 * simPlus - inner computation for simAdd, simSub. Written to avoid
 *  code duplication for addition/subtraction.
 *
 * Invariants for machine:
 *  common tape position increments rightward until final
 *
 * Param:
 *  x, y: operands
 *  mode: 1 for addition, 0 for subtraction
 */
void simPlus(string x, string y, int mode) {
    int tapePos = 0; // common tape position
    int curState = 0; // carry state for operation on current digits
    int finalState = 2;

    // allocate blank symbol buffer at end of both tapes
    int len = max(x.size(), y.size()) + 1;
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

    /*
     * Encode transition function.
     * Current state is 0 if and only if no carry required to add
     * current digits.
     */
    while (curState != finalState) {
        cout << "TRANSITION\n";
        // print old state
        cout << "State " << curState << ", (" << x[tapePos] << ","
            << y[tapePos] << ") ==> ";

        if (x[tapePos] == ' ' && y[tapePos] == ' ') {
            /*
             * Subtraction: reached end of computation. See below for defn of
             *  subtraction.
             * Addition: new digit 0+0=0 if no carry, 0+0+1=1 if carry.
             */
            y[tapePos] = (mode * curState) ? '1' : ' ';
            curState = finalState;
        } else {
            // new digit is modulo 2 sum of old digits and carry status
            int b1 = (x[tapePos] == ' ') ? 0 : (x[tapePos] - '0');
            int b2 = (y[tapePos] == ' ') ? 0 : (y[tapePos] - '0');

            // determine if carry with next digit
            int digitSum = mode ? (curState + b1 + b2) : (b2 + curState - b1);
            curState = (digitSum > mode);

            int b3 = digitSum & 1;
            y[tapePos] = b3 + '0';
        }

        // write new state configuration
        cout << "State " << curState << ", (" << x[tapePos] << ","
            << y[tapePos] << "), R\n";

        tapePos++;
        // print tape
        cout << "Tape 1: ";
        printTape(x, tapePos);
        cout << "Tape 2: ";
        printTape(y, tapePos);
        cout << "\n";
    }

    // print final contents
    cout << "RESULT\n";
    cout << "Tape: " << y << "\n";
    cout << "Result: " << binToDec(y) << "\n";
}

/*
 * simAdd - computes x + y.
 *
 * Param:
 *  x, y: reverse binary representation of operands
 *
 * Requires:
 *  x, y >= 0
 */
void simAdd(string x, string y) {
    simPlus(x, y, MODE_PLUS);
}

/*
 * simSub - computes x - y if x > y;  (x - y) modulo 2^(|y|) otherwise.
 *
 * Param:
 *  x, y: reverse binary representation of operands
 *
 * Requires: x, y >= 0
 */
void simSub(string x, string y) {
    simPlus(x, y, MODE_MINUS);
}

/*
 * posChange - maps direction of tape shift to numerical position change.
 * Used in simMult.
 *
 * Param:
 *  dir - 'L', 'R', or 'S'
 */
int posChange(char dir) {
    if (dir == 'L') return -1;
    if (dir == 'R') return 1;
    return 0;
}

/*
 * simMult - computes x * y. Until x is the 0 string, TM decrements x, adds y
 *  to tape to eventually store product. See documentation for details.
 *
 * Param:
 *  x, y: reverse binary representation of operands
 *
 * Requires:
 *  x, y >= 0 of reasonable length (e.g. representation of 64-bit integers)
 *
 * Invariants:
 *  tape positions return to 0 after each step
 *  tapes y, prod [see below] always at same position
 */
void simMult(string x, string y) {
    int tapePos[] = {1, 1, 1};
    int curState = 0;
    int finalState = 5;

    // allocate blank symbol display at ends of tapes
    int len = x.size() * y.size() + 2;
    string xs(len - x.size(), ' ');
    string ys(len - y.size(), ' ');
    x = " " + x + xs, y = " " + y + ys;

    string prod(len, ' '); // tape storing value of x * y

    // print initial configuration (state, marked tapes)
    cout << "INITIAL CONFIGURATION\n";
    cout << "state 0\n";
    cout << "Tape 1: ";
    printTape(x, tapePos[0]);
    cout << "Tape 2: ";
    printTape(y, tapePos[1]);
    cout << "Tape 3: ";
    printTape(prod, tapePos[2]);

    /*
     * Encode transition function.
     * State key:
     *  0 - decrement x
     *  1 - move x tape position left to beginning
     *  2, 3 - add y to prod [see below]. Identical to carry states in addition
     *      algorithm.
     *  4 - move y, prod tape positions left
     *  5 - final
     */
    while (curState != finalState) {
        char dir[3] = {'S', 'S', 'S'}; // direction of shift for each tape

        cout << "TRANSITION\n";
        // old state, read
        cout << "State " << curState << ", (" << x[tapePos[0]] << ","
            << y[tapePos[1]] << ", " << prod[tapePos[2]] << ") ==> ";

        if (curState == 0) {
            if (prod[tapePos[2]] == ' ') {
                prod[tapePos[2]] = '0';
            } else if (x[tapePos[0]] == '0') {
                x[tapePos[0]] = '1';
                dir[0] = 'R';
            } else if (x[tapePos[0]] == '1') {
                x[tapePos[0]] = '0';
                curState = 1;
                dir[0] = 'L';
            } else if (x[tapePos[0]] == ' ') {
                curState = finalState;
            }
        } else if (curState == 1) {
            if (x[tapePos[0]] == ' ') {
                curState = 2;
                dir[0] = 'R';
            } else {
                dir[0] = 'L';
            }
        } else if (curState == 2 || curState == 3) {
            if (y[tapePos[1]] == ' ' && prod[tapePos[2]] == ' ') {
                prod[tapePos[2]] = (curState == 3) ? '1' : ' ';
                curState = 4;
                dir[1] = dir[2] = 'L';
            } else {
                int b1 = (y[tapePos[1]] == ' ') ? 0 : (y[tapePos[1]] - '0');
                int b2 = (prod[tapePos[2]] == ' ') ? 0 : (prod[tapePos[2]] - '0');

                // determine if carry with next digit
                int digitSum = (curState - 2) + b1 + b2;
                curState = (digitSum > 1) + 2;
                prod[tapePos[2]] = (digitSum & 1) + '0';

                dir[1] = dir[2] = 'R';
            }
        } else if (curState == 4) {
            if (y[tapePos[1]] == ' ' && prod[tapePos[2]] == ' ') {
                curState = 0;
                dir[1] = dir[2] = 'R';
            } else {
                dir[1] = dir[2] = 'L';
            }
        }

        // write new configuration
        cout << "State " << curState << ", (" << x[tapePos[0]] << ","
                << y[tapePos[1]] << "," << prod[tapePos[2]] << "), ("
                << dir[0] <<", " << dir[1] << ", " << dir[2] << ")\n";

        // calculate print new tape
        for (int i = 0; i < 3; i++)
            tapePos[i] += posChange(dir[i]);

        cout << "Tape 1: ";
        printTape(x, tapePos[0]);
        cout << "Tape 2: ";
        printTape(y, tapePos[1]);
        cout << "Tape 3: ";
        printTape(prod, tapePos[2]);
        cout << "\n";
    }

    cout << "RESULT\n";
    cout << "Tape: " << prod << "\n";
    cout << "Result: " << binToDec(prod) << "\n";
}

void simExp(string x, string y) {
    cout << "Not yet implemented.\n";
}
