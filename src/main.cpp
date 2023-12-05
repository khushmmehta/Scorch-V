#include <iostream>
#include "ScorchVkEngine/ScorchV.h"

int main() {
    ScorchV app;

    try { app.run(); }
    catch (const std::exception& e) { std::cerr << e.what() << std::endl; return EXIT_FAILURE; }

    std::cin.get();

    return EXIT_SUCCESS;
}
