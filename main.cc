#include <iostream>

#include "view.h"
#include "controller.h"
#include "model.h"

int main() {

    View v (std::cout);
    Controller c (std::cin);
    Model m (v, c);

    m.start();
    
    std::cout << "Thanks for playing!" << std::endl;

    return 0;
}