#include "model.h"

#include <cstdlib>
#include <ctime>

#include <string>
#include <fstream>

static const int NUMBER_ATTEMPTS = 6; 

Model::Model(View& view, Controller& controller) :
    mView(view),
    mController(controller),
    mCommonWords(generateWords("./commonWords.txt")),
    mUncommonWords(generateWords("./uncommonWords.txt")) {}


void Model::start() {

    srand(time(nullptr));
    
    while (!mController.isGameOver()) {
        playLevel();

        
        char c = '?';
        while (c != 'y' && c != 'n' && !mController.isGameOver()) {
            mView.askToPlayAgain();
            c = mController.getNextChar();
        }

        if (c == 'n' || mController.isGameOver()) {
            break;
        }   
    }

    mView.reset();
}

void Model::playLevel() {
    mView.startLevel();

    const std::string word = getRandomWord();

    std::vector<View::Letter> viewLine (5);

    std::string guess;
    for (int guessNumber = 0; guessNumber < NUMBER_ATTEMPTS; ++guessNumber) {

        mView.askForNextWord();

        if (mController.isGameOver()) {
            return;
        }

        guess = mController.getNextWord();
        if (!isValidWord(guess)) {
            mView.setMessage("Invalid word!");
            --guessNumber;
            continue;
        }
        mView.setMessage("");

        std::string word_copy = word;

        bool correctWord = true;
        for (int i = 0; i < 5; ++i) {
            viewLine[i].letter = guess[i];
            viewLine[i].colour = View::Colour::GRAY;

            if (word[i] == guess[i]) {
                viewLine[i].colour = View::Colour::GREEN;
                word_copy[i] = '_';
            } else {
                correctWord = false;
            }
        }

        for (int i = 0; i < 5; ++i) {

            if (viewLine[i].colour == View::Colour::GREEN) {
                continue;
            }

            for (int j = 0; j < 5; ++j) {
                if (guess[i] == word_copy[j]) {
                    viewLine[i].colour = View::Colour::YELLOW;
                    word_copy[j] = '_';
                    break;
                }
            }
        }

        mView.setGuess(guessNumber, viewLine);

        if (correctWord) {
            mView.setMessage("That's correct!");
            return;
        }
    }

    mView.setMessage("The word was \'" + word + "\'");
}

std::string Model::getRandomWord() {
    long randomIndex = rand() % mCommonWords.size();
    auto pntr = mCommonWords.begin();

    for (long i = 0; i < randomIndex; ++i) {
        pntr++;
    }
    return *pntr;
}

bool Model::isValidWord(const std::string& word) {
    if (word.length() != 5) {
        return false;
    }

    return mCommonWords.find(word) != mCommonWords.end() ||
            mUncommonWords.find(word) != mUncommonWords.end();
}

std::unordered_set<std::string> Model::generateWords(const std::string& filename) {
    std::ifstream fileStream (filename);

    std::unordered_set<std::string> words;

    words.clear();
    
    std::string line;
    while (!fileStream.eof()) {
        getline(fileStream, line);

        if (line.length() != 5) {
            throw std::runtime_error("Word dictionary is invalid");
        }

        words.insert(line);
    }

    return words;
}
