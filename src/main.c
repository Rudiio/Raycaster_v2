#include <time.h>
#include "Game.h"

int main (int argc, char **argv)
{
    srand(time(NULL));

    mainloop();

    return 0;
}