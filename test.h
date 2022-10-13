#include <gtest/gtest.h>
#include "ioc.h"

std::shared_ptr<UObject> spaceship;

TEST( test1 , test )
{
    Ioc move;

    move.Register( "move" , [  ]( std::string argv[] )->ICommand*
    {

        position xy{ std::stoi( argv[ 0 ] ) , std::stoi( argv[ 1 ] ) };
        velocity vel{ std::stoi( argv[ 2 ] ) , std::stoi( argv[ 3 ] ) };
        int fuel = std::stoi( argv[ 4 ] );

        return new macroCommandMove( spaceship , xy , vel , fuel );
    } );

    std::string argv[ 10 ];

    argv[ 0 ] = "1";
    argv[ 1 ] = "2";
    argv[ 2 ] = "3";
    argv[ 3 ] = "4";
    argv[ 4 ] = "5";

    ICommand* movecmd = move.Resolve( "move" , argv );
    movecmd->execute();

    //  smth tests
}

TEST( test2 , test )
{
     Ioc rotate;

     ExceptionHandler exceptionhandler;
     ExceptionStaus ex_status = ExceptionStaus::NOTPROCESSED;

    try
    {
        exceptionhandler.Setup( "Rotation",
                      ExceptionType::ArgumentException,
                      [](){ std::cout<<"Exception Rotation" <<  std::endl; }
                    );

        exceptionhandler.Setup( "CommandException",
                      ExceptionType::TYPE2Exception,
                      [](){ std::cout<<"CommandException" <<  std::endl; }
                    );

        rotate.Register( "rotate" , [  ]( std::string argv[] )->ICommand*
        {
            int angle = std::stoi( argv[ 0 ] );
            int fuel = std::stoi( argv[ 1 ] );
            velocity vel{ std::stoi( argv[ 2 ] ) , std::stoi( argv[ 3 ] ) };

            return new macroCommandRotate( spaceship , angle , fuel , vel );
        } );

        std::string argv[ 10 ];

        argv[ 0 ] = "45";
        argv[ 1 ] = "100";
        argv[ 2 ] = "4";
        argv[ 3 ] = "5";

        ICommand* rotatecmd = rotate.Resolve( "rotate"  , argv );

        rotatecmd->execute();
    }
    catch ( Exception ex )
    {
        ex_status = exceptionhandler.Handler( ex.cmd_type , ex.ex_type );
    }

    EXPECT_EQ( (int)ex_status , ( int ) ExceptionStaus::PROCESSED );
}
