#include "controller.h"

Controller::Controller(std::istream& stream):
    mStream(stream) {
}

bool Controller::isGameOver() {
    return mStream.eof();
}

char Controller::getNextChar() {
    return mStream.get();
}

std::string Controller::getNextWord() {
    if (isGameOver()) {
        return "";
    }
    
    std::string nextWord;

    mStream >> nextWord;

    return nextWord;
}
