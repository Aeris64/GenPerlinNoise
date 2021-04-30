#include <iostream>
#include "Game.h"
#include "MapGenerator.h"

int main()
{
	auto* pGame = Game::GetInstance();
	pGame->Exec();

    return 0;
}