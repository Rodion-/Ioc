#ifndef IOC_H
#define IOC_H

#include <unordered_map>
#include <iostream>
#include "command.h"

class Ioc
{
    std::unordered_map< std::string , ICommand*(*)( std::string argv[] ) > m_types;

    public:

    Ioc(){}
    ~Ioc(){}

    ICommand* Resolve( std::string type , std::string argv[] )
    {
        auto result = m_types.find( type );

        if( result != m_types.end() )
        {
            result->second( argv );
        }
    }

    void Register( std::string type , ICommand*(*create)( std::string argv[] ) )
    {
        m_types.insert( std::make_pair( type , create ) );
    }
};


#endif // IOC_H
