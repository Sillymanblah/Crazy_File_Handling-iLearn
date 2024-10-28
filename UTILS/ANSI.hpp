#pragma once
#ifndef ANSI_HPP
#define ANSI_HPP

#include "ANSI_COMMAND.hpp"

namespace ANSI
{
    namespace TEXT
    {
        const inline COMMAND DEFAULT           = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::DEFAULT );
        const inline COMMAND BLACK             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BLACK );
        const inline COMMAND RED               = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::RED );
        const inline COMMAND GREEN             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::GREEN );
        const inline COMMAND BLUE              = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BLUE );
        const inline COMMAND YELLOW            = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::YELLOW );
        const inline COMMAND MAGENTA           = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::MAGENTA );
        const inline COMMAND CYAN              = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::CYAN );
        const inline COMMAND WHITE             = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::WHITE );
        const inline COMMAND BRIGHT_BLACK      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_BLACK );
        const inline COMMAND BRIGHT_RED        = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_RED );
        const inline COMMAND BRIGHT_GREEN      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_GREEN );
        const inline COMMAND BRIGHT_BLUE       = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_BLUE );
        const inline COMMAND BRIGHT_YELLOW     = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_YELLOW );
        const inline COMMAND BRIGHT_MAGENTA    = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_MAGENTA );
        const inline COMMAND BRIGHT_CYAN       = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_CYAN );
        const inline COMMAND BRIGHT_WHITE      = COMMAND( LAYER::FOREGROUND, BASIC_COLOR::BRIGHT_WHITE );

        const inline COMMAND& RESET            = DEFAULT;
        
        namespace EFFECTS
        {
            const inline COMMAND RESET         = COMMAND( EFFECT::RESET_ALL );

            const inline COMMAND UNDERLINE_ON  = COMMAND( EFFECT::UNDERLINE );
            const inline COMMAND UNDERLINE_X2  = COMMAND( EFFECT::DOUBLE_UNDERLINE );
            const inline COMMAND UNDERLINE_OFF = COMMAND( EFFECT::RESET_UNDERLINE );
            
            const inline COMMAND OVERLINE_ON     = COMMAND( EFFECT::OVERLINE );
            const inline COMMAND OVERLINE_OFF    = COMMAND( EFFECT::RESET_OVERLINE );

            const inline COMMAND BLINKING_ON   = COMMAND( EFFECT::BLINKING );
            const inline COMMAND BLINKING_OFF  = COMMAND( EFFECT::RESET_BLINKING );

            const inline COMMAND BOLD_ON       = COMMAND( EFFECT::BOLD );
            const inline COMMAND BOLD_OFF      = COMMAND( EFFECT::RESET_BOLD ); // Same thing as RESET_THIN

            const inline COMMAND DIM_ON       = COMMAND( EFFECT::DIM );
            const inline COMMAND DIM_OFF      = COMMAND( EFFECT::RESET_DIM ); // Same thing as RESET_BOLD

            const inline COMMAND ITALIC_ON     = COMMAND( EFFECT::ITALIC );
            const inline COMMAND ITALIC_OFF    = COMMAND( EFFECT::RESET_ITALIC );

            const inline COMMAND OPEN_FRAME    = COMMAND( EFFECT::FRAME_OPEN );
            const inline COMMAND CLOSE_FRAME   = COMMAND( EFFECT::FRAME_CLOSE );

            const inline COMMAND HIDE          = COMMAND( EFFECT::HIDDEN );
            const inline COMMAND UNHIDE        = COMMAND( EFFECT::RESET_HIDDEN );
            
            const inline COMMAND INVERT_ON     = COMMAND( EFFECT::INVERSE );
            const inline COMMAND INVERT_OFF    = COMMAND( EFFECT::RESET_INVERSE );
            
            const inline COMMAND STRIKE_ON     = COMMAND( EFFECT::STRIKETHROUGH );
            const inline COMMAND STRIKE_OFF    = COMMAND( EFFECT::RESET_STRIKETHROUGH );
            
            const inline COMMAND SUB_ON        = COMMAND( EFFECT::SUBSCRIPT );
            const inline COMMAND SUPER_ON      = COMMAND( EFFECT::SUPERSCRIPT );
            const inline COMMAND SCRIPTS_OFF   = COMMAND( EFFECT::RESET_SCRIPT );

            const inline COMMAND& DEFAULT      = RESET;
        }
        // namespace EFFECTS
    }
    // namespace TEXT

    namespace BACKGROUND
    {
        const inline COMMAND DEFAULT           = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::DEFAULT );
        const inline COMMAND BLACK             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BLACK );
        const inline COMMAND RED               = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::RED );
        const inline COMMAND GREEN             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::GREEN );
        const inline COMMAND BLUE              = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BLUE );
        const inline COMMAND YELLOW            = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::YELLOW );
        const inline COMMAND MAGENTA           = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::MAGENTA );
        const inline COMMAND CYAN              = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::CYAN );
        const inline COMMAND WHITE             = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::WHITE );
        const inline COMMAND BRIGHT_BLACK      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_BLACK );
        const inline COMMAND BRIGHT_RED        = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_RED );
        const inline COMMAND BRIGHT_GREEN      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_GREEN );
        const inline COMMAND BRIGHT_BLUE       = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_BLUE );
        const inline COMMAND BRIGHT_YELLOW     = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_YELLOW );
        const inline COMMAND BRIGHT_MAGENTA    = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_MAGENTA );
        const inline COMMAND BRIGHT_CYAN       = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_CYAN );
        const inline COMMAND BRIGHT_WHITE      = COMMAND( LAYER::BACKGROUND, BASIC_COLOR::BRIGHT_WHITE );
        
        const inline COMMAND& RESET            = DEFAULT;
    }
    // namespace BACKGROUND

    /// @deprecated Instead use either @c ANSI::DEFAULT or @c ANSI::RESET.
    const inline std::ostream& FULL_RESET( std::ostream& output )
    { return output << TEXT::RESET << TEXT::EFFECTS::RESET << BACKGROUND::RESET; }

    const inline COMMANDS DEFAULT  = { TEXT::DEFAULT, TEXT::EFFECTS::DEFAULT, BACKGROUND::DEFAULT };
    const inline COMMANDS& RESET   = DEFAULT;
}
// namespace ANSI


#endif // ANSI_HPP