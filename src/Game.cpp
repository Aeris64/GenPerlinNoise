#include "../include/Game.h"
#include "../include/MapGenerator.h"
#include "../include/MapDisplay.h"

#include <iostream>

static const int image_width = 1080;
static const int image_height = 720;

const sf::Time Game::g_refreshTime = sf::seconds(1.f / 60.f);
static const auto g_videoMode = sf::VideoMode(image_width, image_height);
static const std::string g_windowTitle = "SFML M1";

static const float scale = 400;
static const int octaves = 7;
static const float persistance = 0.5F;
static const float lacunarity = 2.F;
static const int seed = 300;

Game::~Game()
{
}

void Game::Exec()
{
	const sf::Clock clock;
	auto previous = clock.getElapsedTime().asMilliseconds();
	auto lag = 0.0;

	static const auto refreshTime = g_refreshTime.asMilliseconds();

	MapGenerator mg(image_width, image_height, scale, octaves, persistance, lacunarity, seed);
	mg.GenerateMap();

	while (m_pWindow->isOpen())
	{
		const auto current = clock.getElapsedTime().asMilliseconds();
		const auto elapsed = current - previous;
		previous = current;
		lag += elapsed;

		m_pWindow->clear(sf::Color::Black);
		
		ProcessInput();

		while(refreshTime > 0.0 && lag >= refreshTime)
		{
			Update();
			lag -= refreshTime;
		}

		m_pWindow->draw(*mg.sprite);
		// mg.OnDraw(m_pWindow);

		Render();
	}
}

Game::Game()
	: m_pWindow(std::make_unique<sf::RenderWindow>(g_videoMode, g_windowTitle))
{
	/*auto* pPlayer = m_factory->Create("Player");
	m_objects.push_back(pPlayer);*/
}

void Game::ProcessInput()
{
	sf::Event event;
	while (m_pWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_pWindow->close();
	}
}

void Game::Update()
{
	/*for(auto& object : m_objects)
	{
		if(object != nullptr)
			object.Update();
	}*/
}

void Game::Render()
{
	/*for(auto& object : m_objects)
	{
		if(object != nullptr)
			object.Render();
	}*/

	m_pWindow->display();
}
