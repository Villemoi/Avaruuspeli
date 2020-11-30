#include <sstream>
#include "GameState.hpp"
#include "DEFINITIONS.hpp"

#include <iostream>

namespace Avaruuspeli
{
	GameState::GameState(GameDataRef data) : _data(data)
	{

	}
	void GameState::Init()
	{
		score = 0;

		//load textures
		this->_data->assets.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Player", PLAYER_FILEPATH);
		this->_data->assets.LoadTexture("Enemy", ENEMY_FILEPATH);
		this->_data->assets.LoadTexture("Player Projectile", PLAYER_PROJECTILE_FILEPATH);
		this->_data->assets.LoadTexture("Enemy Projectile", ENEMY_PROJECTILE_FILEPATH);
		//set textures to sprites
		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_player.setTexture(this->_data->assets.GetTexture("Player"));
		this->_enemy.setTexture(this->_data->assets.GetTexture("Enemy"));
		this->_playerProjectile.setTexture(this->_data->assets.GetTexture("Player Projectile"));
		this->_enemyProjectile.setTexture(this->_data->assets.GetTexture("Enemy Projectile"));

		//player start position
		this->_player.setPosition((SCREEN_WIDTH / 2) - (this->_player.getGlobalBounds().width / 2),
			SCREEN_HEIGHT - (this->_player.getGlobalBounds().width * 1.5f));

	}

	//Inputs
	void GameState::HandleInput()
	{
		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			//closing the window
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				// Escape key is pressed: Pause
				std::cout << "Pause The Game" << std::endl;
			}
		}
	}

	void GameState::Update(float dt)
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// left key is pressed: move our character
			this->_player.move(-1 * PLAYER_SPEED, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// right key is pressed: move our character
			this->_player.move(1 * PLAYER_SPEED, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			// up key is pressed: move our character
			this->_player.move(0, -1 * PLAYER_SPEED);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			// down key is pressed: move our character
			this->_player.move(0, 1 * PLAYER_SPEED);
		}

	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear();
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_player);
		this->_data->window.draw(this->_enemy);
		this->_data->window.draw(this->_playerProjectile);
		this->_data->window.draw(this->_enemyProjectile);
		this->_data->window.display();
	}
}