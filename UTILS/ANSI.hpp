#pragma once
#ifndef ANSI_HPP
#define ANSI_HPP

#include "ANSI_COMMAND.hpp"

namespace ANSI
{
    namespace TEXT
    {
        static inline COMMAND DEFAULT           = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::DEFAULT );
        static inline COMMAND BLACK             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BLACK );
        static inline COMMAND RED               = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::RED );
        static inline COMMAND GREEN             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::GREEN );
        static inline COMMAND BLUE              = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BLUE );
        static inline COMMAND YELLOW            = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::YELLOW );
        static inline COMMAND MAGENTA           = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::MAGENTA );
        static inline COMMAND CYAN              = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::CYAN );
        static inline COMMAND WHITE             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::WHITE );
        static inline COMMAND BRIGHT_BLACK      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_BLACK );
        static inline COMMAND BRIGHT_RED        = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_RED );
        static inline COMMAND BRIGHT_GREEN      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_GREEN );
        static inline COMMAND BRIGHT_BLUE       = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_BLUE );
        static inline COMMAND BRIGHT_YELLOW     = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_YELLOW );
        static inline COMMAND BRIGHT_MAGENTA    = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_MAGENTA );
        static inline COMMAND BRIGHT_CYAN       = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_CYAN );
        static inline COMMAND BRIGHT_WHITE      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_WHITE );

        static inline COMMAND& RESET            = DEFAULT;
        
        namespace EFFECTS
        {
            static inline COMMAND RESET         = COMMAND( EFFECT::RESET_ALL );

            static inline COMMAND UNDERLINE_ON  = COMMAND( EFFECT::UNDERLINE );
            static inline COMMAND UNDERLINE_X2  = COMMAND( EFFECT::DOUBLE_UNDERLINE );
            static inline COMMAND UNDERLINE_OFF = COMMAND( EFFECT::RESET_UNDERLINE );
            
            static inline COMMAND OVERLINE_ON     = COMMAND( EFFECT::OVERLINE );
            static inline COMMAND OVERLINE_OFF    = COMMAND( EFFECT::RESET_OVERLINE );

            static inline COMMAND BLINKING_ON   = COMMAND( EFFECT::BLINKING );
            static inline COMMAND BLINKING_OFF  = COMMAND( EFFECT::RESET_BLINKING );

            static inline COMMAND BOLD_ON       = COMMAND( EFFECT::BOLD );
            static inline COMMAND BOLD_OFF      = COMMAND( EFFECT::RESET_BOLD ); // Same thing as RESET_THIN

            static inline COMMAND DIM_ON       = COMMAND( EFFECT::DIM );
            static inline COMMAND DIM_OFF      = COMMAND( EFFECT::RESET_DIM ); // Same thing as RESET_BOLD

            static inline COMMAND ITALIC_ON     = COMMAND( EFFECT::ITALIC );
            static inline COMMAND ITALIC_OFF    = COMMAND( EFFECT::RESET_ITALIC );

            static inline COMMAND OPEN_FRAME    = COMMAND( EFFECT::FRAME_OPEN );
            static inline COMMAND CLOSE_FRAME   = COMMAND( EFFECT::FRAME_CLOSE );

            static inline COMMAND HIDE          = COMMAND( EFFECT::HIDDEN );
            static inline COMMAND UNHIDE        = COMMAND( EFFECT::RESET_HIDDEN );
            
            static inline COMMAND INVERT_ON     = COMMAND( EFFECT::INVERSE );
            static inline COMMAND INVERT_OFF    = COMMAND( EFFECT::RESET_INVERSE );
            
            static inline COMMAND STRIKE_ON     = COMMAND( EFFECT::STRIKETHROUGH );
            static inline COMMAND STRIKE_OFF    = COMMAND( EFFECT::RESET_STRIKETHROUGH );
            
            static inline COMMAND SUB_ON        = COMMAND( EFFECT::SUBSCRIPT );
            static inline COMMAND SUPER_ON      = COMMAND( EFFECT::SUPERSCRIPT );
            static inline COMMAND SCRIPTS_OFF   = COMMAND( EFFECT::RESET_SCRIPT );
        }
        // namespace EFFECTS
    }
    // namespace TEXT

    namespace BACKGROUND
    {
        static inline COMMAND DEFAULT           = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::DEFAULT );
        static inline COMMAND BLACK             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BLACK );
        static inline COMMAND RED               = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::RED );
        static inline COMMAND GREEN             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::GREEN );
        static inline COMMAND BLUE              = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BLUE );
        static inline COMMAND YELLOW            = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::YELLOW );
        static inline COMMAND MAGENTA           = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::MAGENTA );
        static inline COMMAND CYAN              = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::CYAN );
        static inline COMMAND WHITE             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::WHITE );
        static inline COMMAND BRIGHT_BLACK      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_BLACK );
        static inline COMMAND BRIGHT_RED        = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_RED );
        static inline COMMAND BRIGHT_GREEN      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_GREEN );
        static inline COMMAND BRIGHT_BLUE       = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_BLUE );
        static inline COMMAND BRIGHT_YELLOW     = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_YELLOW );
        static inline COMMAND BRIGHT_MAGENTA    = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_MAGENTA );
        static inline COMMAND BRIGHT_CYAN       = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_CYAN );
        static inline COMMAND BRIGHT_WHITE      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_WHITE );
        
        static inline COMMAND& RESET            = DEFAULT;
    }
    // namespace BACKGROUND

    static inline std::ostream& FULL_RESET( std::ostream& output )
    { return output << TEXT::RESET << TEXT::EFFECTS::RESET << BACKGROUND::RESET; }
}
// namespace ANSI


#endif // ANSI_HPP