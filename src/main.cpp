#include <iostream>
#include "../include/Game.h"
#include "../include/MapGenerator.h"

int main()
{
	auto* pGame = Game::GetInstance();
	pGame->Exec();

    return 0;
}