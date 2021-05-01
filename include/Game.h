#ifndef GAME_H
#define GAME_H

#include <memory>
#include <list>
#include "SFML/Graphics.hpp"
#include "Singleton.h"

class Game : public Singleton<Game>
{
	friend class Singleton<Game>;

public:
	~Game();
	void Exec();

private:
	Game();

	void ProcessInput();
	void Update();
	void Render();

	std::unique_ptr<sf::RenderWindow> m_pWindow;

	static const sf::Time g_refreshTime;
	//std::list<GameObject*> m_objects;
};


#endif // !GAME_H