/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement(std::string s) {
    line = s;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.addWordCharacters("-");
    scanner.setInput(s);
    std::string token = scanner.nextToken();
    token = scanner.nextToken();
    op = token;
}

void Statement::execute(EvalState &state, Program &program) {
    if (op == "INPUT") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        token = scanner.nextToken();
        token = scanner.nextToken();
        std::string in;
        getline(std::cin, in);
        try {
            TokenScanner scanner123;
            scanner123.ignoreWhitespace();
            scanner123.scanNumbers();
            scanner123.addWordCharacters("-");
            scanner123.setInput(in);
            Expression *exp1 = parseExp(scanner123);
            int value = exp1->eval(state);
            state.setValue(token, value);
            int k = program.getNextLineNumber(m);
            program.order[k].execute(state, program);
        } catch (...) {
            std::cout << "INVALID NUMBER" << std::endl;
        }
    }
    if (op == "REM") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        int k = program.getNextLineNumber(m);
        program.order[k].execute(state, program);
    }

    if (op == "LET") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        token = scanner.nextToken();
        std::string a = scanner.nextToken();
        if (state.isDefined(a)) {
            token = scanner.nextToken();
            token = scanner.nextToken();
            Expression *exp1 = parseExp(scanner);
            int value = exp1->eval(state);
            state.setValue(a, value);
            int k = program.getNextLineNumber(m);
            program.order[k].execute(state, program);
        } else
            error("VARIABLE NOT DEFINED");
    }
    if (op == "PRINT") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        token = scanner.nextToken();
        token = scanner.nextToken();
        Expression *exp1 = parseExp(scanner);
        int value = exp1->eval(state);
        std::cout << value << std::endl;
        int k = program.getNextLineNumber(m);
        program.order[k].execute(state, program);
    }
    if (op == "IF") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int mm = stringToInteger(token);
        token = scanner.nextToken();
        std::string a = scanner.nextToken();
        a += " ";
        token = scanner.nextToken();
        while (token != "<" || token != "=" || token != ">") {
            a = a + token + " ";
            token = scanner.nextToken();
        }
        char oop = token[0];
        token = scanner.nextToken();
        TokenScanner scanner1, scanner2;
        scanner1.ignoreWhitespace();
        scanner1.scanNumbers();
        scanner1.addWordCharacters("-");
        scanner1.setInput(a);
        Expression *exp1 = parseExp(scanner1);
        int value1 = exp1->eval(state);
        scanner2.ignoreWhitespace();
        scanner2.scanNumbers();
        scanner2.addWordCharacters("-");
        a = token;
        a += " ";
        while (token != "THEN") {
            a = a + token + " ";
            token = scanner.nextToken();
        }
        scanner2.setInput(a);
        Expression *exp2 = parseExp(scanner1);
        int value2 = exp2->eval(state);
        if (((oop == '=') && (value1 == value2)) || ((oop == '<') && (value1 < value2)) ||
            ((oop == '>') && (value1 > value2))) {
            token = scanner.nextToken();
            int m = stringToInteger(token);
            program.order[m].execute(state, program);
        } else {
            int k = program.getNextLineNumber(mm);
            program.order[k].execute(state, program);
        }
    }
    if (op == "GOTO") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        token = scanner.nextToken();
        token = scanner.nextToken();
        int m = stringToInteger(token);
        program.order[m].execute(state, program);
    }
    if (op == "END") {
        return;
    }
}

Statement::Statement() {
    op = "";
    line = "";
}
