#ifndef ANSI_COLORS_HPP
#define ANSI_COLORS_HPP

#include <string>
#include <stdexcept>

#define COLOR_CASE(color) case BASIC::color : return CODE::COLOR::color;

namespace ANSI
{
    typedef unsigned char COLOR_BYTE;

    namespace CODE
    {
        namespace CONTROL
        {
            constexpr const char* BEGIN         = "\x1b[";
            constexpr const char* END           = "m";
        } // namespace CONTROL

        namespace LAYER
        {
            constexpr const char* FOREGROUND    = "38;2";
            constexpr const char* BACKGROUND    = "48;2";
        } // namespace LAYER

        namespace COLOR
        {
            constexpr const char* BLACK         = "0;0;0";
            constexpr const char* WHITE         = "255;255;255";
            constexpr const char* RED           = "255;0;0";
            constexpr const char* GREEN         = "0;255;0";
            constexpr const char* BLUE          = "0;0;255";
            constexpr const char* YELLOW        = "255;255;0";
            constexpr const char* CYAN          = "0;255;255";
            constexpr const char* MAGENTA       = "255;0;255";
        } // namespace COLOR
        
        constexpr const char SEPARATOR          = ';';
    } // namespace CODE

    enum class LAYER : bool
    {
        FOREGROUND = 0,
        BACKGROUND = 1,
    };

    class COLOR
    {
    public:
        enum class BASIC : unsigned char
        {
            BLACK,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            WHITE,
        };
        
        std::string color;

    private:
        static std::string get_layer_code( LAYER layer );

        static std::string get_base_code( LAYER layer );
        static std::string get_base( LAYER layer );

        static std::string get_default( LAYER layer );

        static std::string basic_color_code( BASIC color );
        static std::string build_color_code( COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue );

        // Constructor from string to COLOR in private for usage in the build and get color functions.
        COLOR( const std::string& color_code ) : color(color_code) {}
        COLOR( std::string&& color_code ) : color(color_code) {}

    public:
        COLOR() : color( get_default( LAYER::BACKGROUND ) + get_default( LAYER::FOREGROUND ) ) {}
        COLOR( const COLOR& ) = default;
        COLOR( COLOR&& ) = default;

        friend COLOR get_basic_color( COLOR::BASIC color, LAYER level = ANSI::LAYER::FOREGROUND );

        friend COLOR build_color( COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue, LAYER level = ANSI::LAYER::FOREGROUND );
    };

} // namespace ANSI

#endif // ANSI_COLORS_HPP