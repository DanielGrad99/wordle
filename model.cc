#include "model.h"

#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

#include "constants.h"

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

    std::vector<View::Letter> viewLine (WORD_LENGTH);

    std::string guess;
    for (int guessNumber = 0; guessNumber < MAX_NUM_GUESSES; ++guessNumber) {

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
        for (int i = 0; i < WORD_LENGTH; ++i) {
            viewLine[i].letter = guess[i];
            viewLine[i].colour = View::Colour::GRAY;

            if (word[i] == guess[i]) {
                viewLine[i].colour = View::Colour::GREEN;
                word_copy[i] = '_';
            } else {
                correctWord = false;
            }
        }

        for (int i = 0; i < WORD_LENGTH; ++i) {

            if (viewLine[i].colour == View::Colour::GREEN) {
                continue;
            }

            for (int j = 0; j < WORD_LENGTH; ++j) {
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
    if (word.length() != WORD_LENGTH) {
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

        if (line.length() != WORD_LENGTH) {
            throw std::runtime_error("Word dictionary is invalid");
        }

        words.insert(line);
    }

    return words;
}
