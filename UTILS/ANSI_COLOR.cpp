#include "ANSI_COLOR.hpp"

[[nodiscard]] std::string ANSI::COLOR::basic_color_tag( ANSI::COLOR::BASIC color )
{
    switch (color)
    {
        COLOR_CASE(BLACK);
        COLOR_CASE(RED);
        COLOR_CASE(GREEN);
        COLOR_CASE(YELLOW);
        COLOR_CASE(BLUE);
        COLOR_CASE(MAGENTA);
        COLOR_CASE(CYAN);
        COLOR_CASE(WHITE);

        default: throw std::invalid_argument( "Color does not match the allowed values of the enumeration..." ); // Should never reach this, but if you do, fuck you.
    }
}

[[nodiscard]] std::string ANSI::COLOR::get_layer_code( ANSI::LAYER layer )
{ return ( layer == LAYER::FOREGROUND ) ? CODE::LAYER::FOREGROUND : CODE::LAYER::BACKGROUND; }

[[nodiscard]] std::string ANSI::COLOR::get_base_code( ANSI::LAYER layer )
{ return CODE::CONTROL::BEGIN + get_layer_code(layer); }

[[nodiscard]] std::string ANSI::COLOR::get_base( ANSI::LAYER layer )
{ return get_base_code(layer) + CODE::SEPARATOR; }

[[nodiscard]] std::string ANSI::COLOR::get_default( ANSI::LAYER layer )
{ return get_base_code(layer) + CODE::CONTROL::END; }

[[nodiscard]] std::string ANSI::COLOR::build_color_tag( ANSI::COLOR_BYTE red, ANSI::COLOR_BYTE green, ANSI::COLOR_BYTE blue )
{ return std::to_string(red) + CODE::SEPARATOR + std::to_string(green) + CODE::SEPARATOR + std::to_string(blue); }

[[nodiscard]] ANSI::COLOR ANSI::get_basic_color( ANSI::COLOR::BASIC color, LAYER level )
{ return ANSI::COLOR::get_base(level) + ANSI::COLOR::basic_color_tag(color) + CODE::CONTROL::END; }

[[nodiscard]] ANSI::COLOR ANSI::build_color( ANSI::COLOR_BYTE red, ANSI::COLOR_BYTE green, ANSI::COLOR_BYTE blue, LAYER level )
{ return ANSI::COLOR::get_base(level) + ANSI::COLOR::build_color_tag(red, green, blue) + CODE::CONTROL::END; }