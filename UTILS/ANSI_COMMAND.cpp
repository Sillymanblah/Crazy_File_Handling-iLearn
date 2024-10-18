#include "ANSI_COMMAND.hpp"

namespace ANSI
{
#define VALUE_STEP 51 // = 255 / 5
    BYTE BYTE_COLOR::rgb_code( BYTE __red, BYTE __green, BYTE __blue )
    { return ( ( __red / VALUE_STEP ) * RGB::RED_INC ) + ( ( __green / VALUE_STEP ) * RGB::GREEN_INC ) + ( ( __blue / VALUE_STEP ) * RGB::BLUE_INC ); }
    
    COMMAND::COMMAND( EFFECT __effect ) : codes( { (BYTE) __effect } ), closing_tag( CODE::END::TEXT ) {}
    COMMAND::COMMAND( LAYER __layer, BASIC_COLOR __color ) : closing_tag( CODE::END::TEXT )
    {
        if ( __layer == LAYER::UNDERLINE && __color != BASIC_COLOR::DEFAULT )
            throw std::invalid_argument( "COMMAND( LAYER, BASIC_COLOR ) can only take the argument of BASIC_COLOR::DEFAULT when the layer is LAYER::UNDERLINE!" );
        codes.push_back( (BYTE)__layer + (BYTE)__color );
    }
    COMMAND::COMMAND( LAYER __layer, BYTE_COLOR __color ) : codes( { (BYTE)__layer, CODE::BYTE_FORMAT, __color._code } ), closing_tag( CODE::END::TEXT ) {}
    COMMAND::COMMAND( LAYER __layer, RGB_COLOR __color ) : codes( { (BYTE)__layer, CODE::RGB_FORMAT, __color._red, __color._green, __color._blue } ), closing_tag( CODE::END::TEXT ) {}

    std::ostream& operator << ( std::ostream& output, const ANSI::COMMAND& command )
    {
        output << ANSI::COMMAND::CODE::BEGIN::ESCAPE << ANSI::COMMAND::CODE::BEGIN::SEQUENCE_INTRO;
        
        size_t num_commands = command.codes.size();
        for ( size_t index = 0; index < num_commands; ++index )
        {
            output << (int)command.codes[index];
            if ( index < num_commands - 1 ) output << ANSI::COMMAND::CODE::DELIMITER;
        }

        return output << command.closing_tag;
    }
    std::ostream& operator << ( std::ostream& output, ANSI::COMMAND&& command )
    {
        output << ANSI::COMMAND::CODE::BEGIN_CODE;
        
        size_t num_commands = command.codes.size();
        for ( size_t index = 0; index < num_commands; ++index )
        {
            output << (int)command.codes[index];
            if ( index < num_commands - 1 ) output << ANSI::COMMAND::CODE::DELIMITER;
        }
        
        return output << command.closing_tag;
    }
    
    std::ostream& operator << ( std::ostream& output, const COMMANDS& commands )
    {
        for ( const COMMAND& command : commands ) output << command;
        return output;
    }
    std::ostream& operator << ( std::ostream& output, COMMANDS&& commands )
    {
        for ( COMMAND& command : commands ) output << command;
        return output;
    }
};
// namespace ANSI