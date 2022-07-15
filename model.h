#pragma once

#include <unordered_set>

#include "view.h"
#include "controller.h"

class Model {
public:
    Model(View& view, Controller& controller);

    void start();

private:

    void playLevel();
    
    std::string getRandomWord();
    bool isValidWord(const std::string& word);

    static std::unordered_set<std::string> generateWords(const std::string& filename);

    View& mView;
    Controller& mController;

    std::unordered_set<std::string> mCommonWords;
    std::unordered_set<std::string> mUncommonWords;

};
