
#include "ANSI.hpp"

#include <iostream>

int main()
{
    std::cout << ANSI::TEXT::GREEN << "Hello World!\n";
    std::cout << ANSI::TEXT::RED << "REDDDDDDD RUMMM\n";

    std::cout << ANSI::BACKGROUND::CYAN << ANSI::TEXT::EFFECTS::BOLD_ON << ANSI::TEXT::BRIGHT_YELLOW << "Who let the dogs out?\n";

    std::cout << ANSI::FULL_RESET << "I am back to normal!\n";

    std::cout << ANSI::COMMAND( ANSI::LAYER::BACKGROUND, ANSI::RGB_COLOR( 50, 80, 10 ) ) << "Some kind of gross green, maybe...\n";

    std::cout << ANSI::TEXT::MAGENTA << "Magenta is my favorite, but this looks like PINK!\n" << ANSI::FULL_RESET;

    return 1;
}