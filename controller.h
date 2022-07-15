#pragma once

#include <iostream>
#include <string>

class Controller {
public:
    Controller(std::istream& stream);

    bool isGameOver();

    char getNextChar();
    std::string getNextWord();

private:
    std::istream& mStream;
};
