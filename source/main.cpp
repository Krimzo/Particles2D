#include "simulator.h"


int main( int argc, char** argv )
{
    Simulator simulator{};
    while ( simulator.update() );
    return 0;
}
