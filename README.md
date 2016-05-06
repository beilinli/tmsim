Turing Machine arithmetic simulator: includes machines for simulating addition, subtraction, multiplication, exponentiation by a TM.

Usage:
- Compile: `g++ -o tmsim tmsim.cpp`
- Run: `./tmsim [-add|-sub|-mult|-exp] [arg1] [arg2]`

Notation:
- prints "INITIAL CONFIGURATION" followed by initial state and initial tape content (usually input).
- prints "TRANSITION" for each transition, followed by
    - state and characters at tape positions before transition
    - state and characters at tape position after transition, and change in tape position(s)
    - new tape contents, with characters at new position(s) marked
- prints "RESULT" and final contents of result tape, as well as integer translation
