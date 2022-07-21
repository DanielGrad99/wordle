#include "view.h"

#include <string>

#include "constants.h"

#define TITLE_ROW 1

#define FIRST_GUESS_ROW 3
#define DIST_BETWEEN_ROWS 1
#define DIST_BETWEEN_LETTERS 2

#define MSG_ROW FIRST_GUESS_ROW + DIST_BETWEEN_ROWS * (MAX_NUM_GUESSES + 1)
#define INPUT_ROW MSG_ROW + 1

View::View(std::ostream& stream):
        mStream(stream),
        mCurColour(View::Colour::WHITE) {
    
    reset();
}

void View::reset() {
    clearScreen();
    goToPos(1, 1);
}

void View::startLevel() {
    reset();
    switchToColour(View::Colour::RESET);

    goToPos(TITLE_ROW, 4);
    mStream << "Guess The Word!";

    std::vector<Letter> empty = {
        {'_', View::Colour::RESET},
        {'_', View::Colour::RESET},
        {'_', View::Colour::RESET},
        {'_', View::Colour::RESET},
        {'_', View::Colour::RESET}
    };
    
    for (int i = 0 ; i < MAX_NUM_GUESSES; ++i) {
        setGuess(i, empty);
    }
}

void View::setGuess(int guessNumber, std::vector<Letter>& word) {
    goToPos(FIRST_GUESS_ROW + guessNumber * DIST_BETWEEN_ROWS, 8);
    
    for (size_t i = 0; i < word.size(); ++i) {
        if (i != 0) {
            for (int i = 1; i < DIST_BETWEEN_LETTERS; ++i) {
                mStream << " ";
            }
        }

        switchToColour(word[i].colour);
        mStream << word[i].letter;
    }
}

void View::setMessage(std::string msg) {
    goToPos(MSG_ROW, 1);
    switchToColour(View::Colour::RESET);
    mStream << msg << "\033[K";
}

void View::askForNextWord() {
    goToPos(INPUT_ROW, 1);
    switchToColour(View::Colour::RESET);
    mStream << "Next Guess: " << "\033[J";
}

void View::askToPlayAgain() {
    goToPos(INPUT_ROW, 1);
    switchToColour(View::Colour::RESET);
    mStream << "Play Again? (y/n)" << "\033[J";   
}

void View::clearScreen() {
    mStream << "\033[2J";
}

void View::goToPos(int r, int c) {
    std::string row = std::to_string(r);
    std::string column = std::to_string(c);

    mStream << "\033[" << row << ";" << column << "H";
}

void View::switchToColour(const Colour& c) {
    if (mCurColour == c) {
        return;
    }
    mCurColour = c;

    mStream << "\033[";

    switch (c) {
    case View::Colour::GREEN:
        mStream << "32";
        break;
    case View::Colour::YELLOW:
        mStream << "33";
        break;
    case View::Colour::GRAY:
        mStream << "90";
        break;
    case View::Colour::WHITE:
        mStream << "37";
        break;
    case View::Colour::RESET:
        mStream << "0";
        break;
    }
    mStream << "m";
}
