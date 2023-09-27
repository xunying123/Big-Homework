/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

int processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            int k = processLine(input, program, state);
            if (k == -1) return 0;
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

int processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.addWordCharacters("-");
    scanner.setInput(line);
    std::string token = scanner.nextToken();
    if (token[0] < '0' || token[0] > '9') {
        if (scanner.hasMoreTokens()) {
            if (token == "LET") {
                std::string a = scanner.nextToken();
                token = scanner.nextToken();
                token = scanner.nextToken();
                Expression *exp = parseExp(scanner);
                int value = exp->eval(state);
                state.setValue(a, value);
                return 0;
            }
            if (token == "PRINT") {
                Expression *exp = parseExp(scanner);
                int value = exp->eval(state);
                std::cout << value << std::endl;
                return 0;
            }
            if (token == "INPUT") {
                std::string in;
                std::string a = scanner.nextToken();
                if (state.isDefined(a)) {
                    getline(std::cin, in);
                    try {
                        TokenScanner scanner123;
                        scanner123.ignoreWhitespace();
                        scanner123.scanNumbers();
                        scanner123.addWordCharacters("-");
                        scanner123.setInput(in);
                        Expression *exp1 = parseExp(scanner123);
                        int value = exp1->eval(state);
                        state.setValue(a, value);
                    } catch (...) {
                        std::cout << "INVALID NUMBER" << std::endl;
                    }
                } else {
                    error("VARIABLE NOT DEFINED");
                }

            }
        } else {
            if (token == "CLEAR") {
                program.clear();
                state.Clear();
                return 0;
            }
            if (token == "QUIT") {
                program.clear();
                state.Clear();
                exit(0);
                return -1;
            }
            if (token == "LIST") {
                program.Print();
            }
            if (token == "RUN") {
                auto itea = program.order.begin();
                (itea->second).execute(state, program);
            }
        }
    } else {
        if (scanner.hasMoreTokens()) {
            int k = stringToInteger(token);
            program.addSourceLine(k, line);
        } else {
            return 0;
        }
    }
}

