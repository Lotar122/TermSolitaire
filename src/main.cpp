#include <new>

#include "Classes/Game/Game.hpp"

using byte = unsigned char;

int main()
{
    #ifdef Windows
        system("cls");
    #else
        system("clear");
    #endif

    alignas(Game) byte gameMemory[sizeof(Game)];
    Game* game = new (gameMemory) Game();

    while(!game->stop)
    {
        int exitCode = game->update();
        if (exitCode == 1)
        {
            game->~Game();
            game = new (gameMemory) Game();
        }
    }

    return 0;
}