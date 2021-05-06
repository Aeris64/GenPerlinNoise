#include <iostream>
#include "../include/Game.h"

int main()
{
	auto* pGame = Game::GetInstance();
	pGame->Exec();

    return 0;
}