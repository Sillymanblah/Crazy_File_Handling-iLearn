#ifndef ANSI_COLORS_HPP
#define ANSI_COLORS_HPP

#include <string>
#include <stdexcept>

#define COLOR_CASE(color) case BASIC::color : return color;

namespace ANSI
{
    typedef unsigned char COLOR_BYTE;

    constexpr const char* BEGIN         = "\x1b[";
    constexpr const char* END           = "m";

    constexpr const char* FOREGROUND    = "38;2;";
    constexpr const char* BACKGROUND    = "48;2;";

    constexpr const char* BLACK         = "0;0;0";
    constexpr const char* WHITE         = "255;255;255";
    constexpr const char* RED           = "255;0;0";
    constexpr const char* GREEN         = "0;255;0";
    constexpr const char* BLUE          = "0;0;255";
    constexpr const char* YELLOW        = "255;255;0";
    constexpr const char* CYAN          = "0;255;255";
    constexpr const char* MAGENTA       = "255;0;255";

    enum class LAYER : bool
    {
        FOREGROUND = 0,
        BACKGROUND = 1,
    };

    class COLOR
    {
    public:
        enum class BASIC : COLOR_BYTE
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
        [[nodiscard]] static const char* get_basic_color(BASIC color)
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

                default: throw std::invalid_argument("Color does not fit the enumeration..."); // Should never reach this, but if you do, fuck you.
            }
        }
        
        [[nodiscard]] static std::string get_base(LAYER level)
        {
            std::string color_command(BEGIN);
            color_command += level == LAYER::FOREGROUND ? FOREGROUND : BACKGROUND;
            return color_command;
        }
        [[nodiscard]] static std::string create_color(COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue)
        {
            return ';' + std::to_string(red) + ';' + std::to_string(green) + ';' + std::to_string(blue);
        }

        COLOR(const std::string& color_code) : color(color_code) {}
        COLOR(std::string&& color_code) : color(color_code) {}

    public:
        COLOR() : color()
        {
            color += std::string(BEGIN) + "39;2" + END; // default foreground
            color += std::string(BEGIN) + "49;2" + END; // default background
        }
        COLOR(const COLOR&) = default;
        COLOR(COLOR&&) = default;

        [[nodiscard]] friend COLOR get_basic_color(COLOR::BASIC color, LAYER level = ANSI::LAYER::FOREGROUND)
        {
            return get_base(level) + COLOR::get_basic_color(color) + END;
        }
        [[nodiscard]] friend COLOR build_color(COLOR_BYTE red, COLOR_BYTE green, COLOR_BYTE blue, LAYER level = ANSI::LAYER::FOREGROUND)
        {
            return get_base(level) + create_color(red, green, blue) + END;
        }
    };
} // namespace ANSI

#endif // ANSI_COLORS_HPP