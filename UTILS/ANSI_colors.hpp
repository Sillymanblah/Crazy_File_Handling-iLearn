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
        [[nodiscard]] static const char* basic_color_tag( BASIC color )
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

        [[nodiscard]] static std::string get_layer_code( LAYER layer )
        { return ( layer == LAYER::FOREGROUND ) ? CODE::LAYER::FOREGROUND : CODE::LAYER::BACKGROUND; }

        [[nodiscard]] static std::string get_base( LAYER layer )
        { return CODE::CONTROL::BEGIN + get_layer_code(layer) + CODE::SEPARATOR; }

        [[nodiscard]] static std::string build_color_tag( COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue )
        { return std::to_string(red) + CODE::SEPARATOR + std::to_string(green) + CODE::SEPARATOR + std::to_string(blue); }

        // Constructor from string to COLOR in private for usage in the build and get color functions.
        COLOR( const std::string& color_code ) : color(color_code) {}
        COLOR( std::string&& color_code ) : color(color_code) {}

    public:
        COLOR() : color()
        {
            color += std::string( CODE::CONTROL::BEGIN ) + CODE::LAYER::FOREGROUND + CODE::CONTROL::END; // default foreground
            color += std::string( CODE::CONTROL::BEGIN ) + CODE::LAYER::BACKGROUND + CODE::CONTROL::END; // default background
        }
        COLOR( const COLOR& ) = default;
        COLOR( COLOR&& ) = default;

        [[nodiscard]] friend COLOR get_basic_color( COLOR::BASIC color, LAYER level = ANSI::LAYER::FOREGROUND )
        { return get_base(level) + basic_color_tag(color) + CODE::CONTROL::END; }

        [[nodiscard]] friend COLOR build_color( COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue, LAYER level = ANSI::LAYER::FOREGROUND )
        { return get_base(level) + build_color_tag(red, green, blue) + CODE::CONTROL::END; }
    };

} // namespace ANSI

#endif // ANSI_COLORS_HPP