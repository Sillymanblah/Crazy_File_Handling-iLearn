#ifndef ANSI_COLORS_HPP
#define ANSI_COLORS_HPP

#include <string>
#include <stdexcept>

typedef unsigned char BYTE;

namespace ANSI
{
    enum class LAYER : BYTE
    {
        FOREGROUND = 38,
        BACKGROUND = 48,
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

        // Resetting bold or dim colors is the same code, so that is why they are both set to 22, the rest all increment from that.
        
        RESET_ALL = 0,
        RESET_BOLD = 22,
        RESET_DIM = 22,
        RESET_ITALIC,
        RESET_UNDERLINE,
        RESET_BLINKING,
        RESET_INVERSE,
        RESET_HIDDEN,
        RESET_STRIKETHROUGH,
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
            
            enum class CONTROL : char
            {
                // This is the only sequence begin code, the rest all mark the end of the sequence
                ESCAPE              = '\x1b',
                // Control Sequence Introducer - always follows ESCAPE in any code
                SEQUENCE_INTRO      = '[',

                COLOR               = 'm',
                SET_MODE            = 'h',
                UNSET_MODE          = 'l',
                KEY_STRING          = 'p',
                SCREEN_ERASE        = 'J',
                LINE_ERASE          = 'K',
                CURSOR_UP           = 'A',
                CURSOR_DOWN         = 'B',
                CURSOR_RIGHT        = 'C',
                CURSOR_LEFT         = 'D',
                CURSOR_START_DOWN   = 'E',
                CURSOR_START_UP     = 'F',
                CURSOR_TO_COLUMN    = 'G',
                CURSOR_TO_POS       = 'H',
                GET_CURSOR_POS      = 'n',
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