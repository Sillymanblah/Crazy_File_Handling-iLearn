#ifndef ANSI_COLORS_HPP
#define ANSI_COLORS_HPP

#include <string>
#include <stdexcept>

typedef unsigned char BYTE;

namespace ANSI
{
    enum class LAYER : BYTE
    {
        FOREGROUND  = 38,
        BACKGROUND  = 48,
        UNDERLINE   = 58, // Can not be used in conjuction with any BASIC_COLORs except DEFAULT
    };
    // enum class LAYER

    enum class EFFECT : BYTE
    {
        BOLD = 1,
        DIM,
        ITALIC,
        UNDERLINE,
        BLINKING,
        INVERSE,
        HIDDEN,
        STRIKETHROUGH,
        DOUBLE_UNDERLINE = 21,
        OVERLINE = 53,
        SUPERSCRIPT = 73,
        SUBSCRIPT = 74,
        
        RESET_ALL = 0,
        RESET_BOLD = 22, // Resetting bold or dim colors is the same code, so they are both set to 22.
        RESET_DIM = 22, // Resetting bold or dim colors is the same code, so they are both set to 22.
        RESET_ITALIC,
        RESET_UNDERLINE, // Turns off underlines, including single and double, plus any colored underlines
        RESET_BLINKING,
        RESET_INVERSE,
        RESET_HIDDEN,
        RESET_STRIKETHROUGH,
        RESET_OVERLINE = 55,
        RESET_SCRIPT = 75, // Turns off subscript AND superscript

        FRAME_OPEN = 51,
        FRAME_CLOSE = 54,
    };

    // For basic colors, these are stored as an offset integer from the forground/background color mode.
    enum class BASIC_COLOR : int
    {
        BLACK = -8,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        
        // We skip over the enumeration value of 0, because that code is not a color and is reserved for setting the color (using a byte or 3 byte RGB).

        DEFAULT = 1, // Default color is a reset of color to whatever the terminal default is.

        // Furthermore, there are bright versions of all the basic colors found further down below:

        BRIGHT_BLACK = 52,
        BRIGHT_RED,
        BRIGHT_GREEN,
        BRIGHT_YELLOW,
        BRIGHT_BLUE,
        BRIGHT_MAGENTA,
        BRIGHT_CYAN,
        BRIGHT_WHITE,
    };

    class BYTE_COLOR
    {
    public:
        BYTE_COLOR( BYTE __value );
        BYTE_COLOR( BYTE_COLOR&& ) = default;
        BYTE_COLOR( const BYTE_COLOR& ) = default;

    private:
        BYTE _code;
    };
    
    class RGB_COLOR
    {
    public:
        RGB_COLOR( BYTE __red, BYTE __green, BYTE __blue ) : _red(__red), _green(__green), _blue(__blue) {}
        RGB_COLOR( RGB_COLOR&& ) = default;
        RGB_COLOR( const RGB_COLOR& ) = default;

    private:
        BYTE _red;
        BYTE _green;
        BYTE _blue;
    };

    class COMMAND
    {
    private:
        // The below section cannot be a namespace anymore because they are stored inside of a class.
        struct CODE
        {
            static inline const char DELIMITER              = ';';
            static inline const unsigned char RGB_FORMAT    = 2;
            static inline const unsigned char BYTE_FORMAT   = 5;
            
            enum class BEGIN : char
            {
                ESCAPE          = 27, // Escape key to start a command or sequence.
                SEQUENCE_INTRO  = '[', // Control Sequence Introducer - almost always follows ESCAPE in ANSI codes.
            };

            static inline const char BEGIN_CODE[2] = { (char)BEGIN::ESCAPE, (char)BEGIN::SEQUENCE_INTRO };
            
            enum class END : char
            {
                CURSOR_UP           = 'A',
                CURSOR_DOWN         = 'B',
                CURSOR_RIGHT        = 'C',
                CURSOR_LEFT         = 'D',
                CURSOR_START_DOWN   = 'E',
                CURSOR_START_UP     = 'F',
                CURSOR_TO_COLUMN    = 'G',
                CURSOR_TO_POS       = 'H',
                SCREEN_ERASE        = 'J',
                LINE_ERASE          = 'K',
                COLOR               = 'm',
                SET_MODE            = 'h',
                UNSET_MODE          = 'l',
                GET_CURSOR_POS      = 'n',
                KEY_STRING          = 'p',
            };
            // static inline enum class CONTROL
        };
        // struct CODE

    private:
        static std::string get_layer_code( LAYER layer );

        static std::string get_base_code( LAYER layer );
        static std::string get_base( LAYER layer );

        static std::string get_default( LAYER layer );


    public:
        COMMAND() = delete;
        COMMAND( EFFECT );
        COMMAND( LAYER, BASIC_COLOR );
        COMMAND( LAYER, BYTE_COLOR );
        COMMAND( LAYER, RGB_COLOR );
        COMMAND( const COMMAND& ) = default;
        COMMAND( COMMAND&& ) = default;

        friend std::ostream& operator << ( std::ostream& output, const COMMAND& command );
        friend std::ostream& operator << ( std::ostream& output, COMMAND&& command );

    private:
        BYTE* commands;
    };
    // class COMMAND
}
// namespace ANSI

#endif // ANSI_COLORS_HPP