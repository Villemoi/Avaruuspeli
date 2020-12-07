#include <sstream>
#include "GameState.hpp"
#include "DEFINITIONS.hpp"
#include <stdlib.h> 

#include <iostream>

namespace Avaruuspeli
{
	GameState::GameState(GameDataRef data) : _data(data)
	{

	}
	void GameState::Init()
	{
		this->score = 0;

		//load textures
		this->_data->assets.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Player", PLAYER_FILEPATH);
		this->_data->assets.LoadTexture("Enemy", ENEMY_FILEPATH);
		this->_data->assets.LoadTexture("Player Projectile", PLAYER_PROJECTILE_FILEPATH);
		this->_data->assets.LoadTexture("Enemy Projectile", ENEMY_PROJECTILE_FILEPATH);
		this->_data->assets.LoadTexture("Player Health", PLAYER_HEALTH_FILEPATH);
		this->_data->assets.LoadTexture("Enemy Health", ENEMY_HEALTH_FILEPATH);
		//set textures to sprites
		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_player.setTexture(this->_data->assets.GetTexture("Player"));
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			this->_enemy[i].setTexture(this->_data->assets.GetTexture("Enemy"));
		}
		this->_playerHealth.setTexture(this->_data->assets.GetTexture("Player Health"));
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			this->_enemyHealth[i].setTexture(this->_data->assets.GetTexture("Enemy Health"));
		}
		for (int i = 0; i < ENEMY_PROJECTILE_COUNT; i++) {
			this->_enemyProjectile[i].setTexture(this->_data->assets.GetTexture("Enemy Projectile"));
			//set bullet positions
			this->_enemyProjectile[i].setPosition(-20,0);
		}
		for (int i = 0; i < PLAYER_PROJECTILE_COUNT; i++) {
			this->_playerProjectile[i].setTexture(this->_data->assets.GetTexture("Player Projectile"));
			//set bullet positions
			this->_playerProjectile[i].setPosition(-20, 0);
		}

		//player start position
		this->_player.setPosition((SCREEN_WIDTH / 2) - (this->_player.getGlobalBounds().width / 2),
			SCREEN_HEIGHT - (this->_player.getGlobalBounds().width * 1.5f));

		//enemy start location
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) 
		{
			this->_enemy[i].setPosition(rand() % SCREEN_WIDTH + 1, rand() % (SCREEN_HEIGHT - (int)this->_enemy[i].getGlobalBounds().height) / 2 + 1);
		}

		//set cooldowns
		this->playerBulletCD = 0;
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			this->enemyBulletCD[i] = 0.0f;
		}

		//set Healths
		this->playerHealth = PLAYER_MAX_HEALTH;
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			this->enemyHealth[i] = ENEMY_MAX_HEALTH;
		}
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
		//player movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			// left key is pressed: move our character
			this->_player.move(-1 * PLAYER_SPEED, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			// right key is pressed: move our character
			this->_player.move(1 * PLAYER_SPEED, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			// up key is pressed: move our character
			this->_player.move(0, -1 * PLAYER_SPEED);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			// down key is pressed: move our character
			this->_player.move(0, 1 * PLAYER_SPEED);
		}

		//screen width boundaries
		if (this->_player.getPosition().x < 0)
		{
			this->_player.setPosition(0, this->_player.getPosition().y);
		}
		else if (this->_player.getPosition().x + this->_player.getGlobalBounds().width > SCREEN_WIDTH)
		{
			this->_player.setPosition(SCREEN_WIDTH - this->_player.getGlobalBounds().width, this->_player.getPosition().y);
		}

		//screen height boundaries
		if (this->_player.getPosition().y < 0)
		{
			this->_player.setPosition(this->_player.getPosition().x, 0);
		}
		else if (this->_player.getPosition().y + this->_player.getGlobalBounds().height > SCREEN_HEIGHT)
		{
			this->_player.setPosition(this->_player.getPosition().x, SCREEN_HEIGHT - this->_player.getGlobalBounds().height);
		}

		//player bullet cooldown
		if (playerBulletCD / FPS_CAP >= PLAYER_SHOOTING_RATE)
		{
			playerBulletOnCD = false;
		}
		else
		{
			playerBulletCD++;
		}
		//player shooting
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			if (playerBulletOnCD == false) {	// Space key is pressed: try shooting
				this->_playerProjectile[playerBullet].setPosition(this->_player.getPosition().x + this->_player.getGlobalBounds().width / 2, this->_player.getPosition().y);
				playerBullet++;
				if (playerBullet > PLAYER_PROJECTILE_COUNT - 1)
				{
					playerBullet = 0;
				}
				playerBulletCD = 0;
				playerBulletOnCD = true;
			}
		}


		//enemy movement
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			if (this->deleteEnemy[i] == 0) {
				if (this->enemyDir[i] == 1)
				{
					this->_enemy[i].move(1 * ENEMY_SPEED, 0);
				}
				if (this->enemyDir[i] == 0)
				{
					this->_enemy[i].move(-1 * ENEMY_SPEED, 0);
				}
				//enemy boundaries and direction change
				if (this->_enemy[i].getPosition().x < 0)
				{
					this->_enemy[i].setPosition(0, this->_enemy[i].getPosition().y);
					this->enemyDir[i] = true;
				}
				else if (this->_enemy[i].getPosition().x + this->_enemy[i].getGlobalBounds().width > SCREEN_WIDTH)
				{
					this->_enemy[i].setPosition(SCREEN_WIDTH - this->_enemy[i].getGlobalBounds().width, this->_enemy[i].getPosition().y);
					this->enemyDir[i] = false;
				}
				//enemy shooting
				if (enemyBulletCD[i] / FPS_CAP >= ENEMY_SHOOTING_RATE)
				{
					this->_enemyProjectile[enemyBullet].setPosition(this->_enemy[i].getPosition().x + this->_enemy[i].getGlobalBounds().width / 2, this->_enemy[i].getPosition().y + this->_enemy[i].getGlobalBounds().height);
					enemyBullet++;
					if (enemyBullet > ENEMY_PROJECTILE_COUNT - 1)
					{
						enemyBullet = 0;
					}
					enemyBulletCD[i] = 0;
				}
				else
				{
					enemyBulletCD[i]++;
				}
			}
		}
		//Bullet movement
		for (int i = 0; i < ENEMY_PROJECTILE_COUNT; i++) {
			this->_enemyProjectile[i].move(0, 1 * ENEMY_PROJECTILE_SPEED);
		}
		for (int i = 0; i < PLAYER_PROJECTILE_COUNT; i++) {
			this->_playerProjectile[i].move(0, -1 * PLAYER_PROJECTILE_SPEED);
		}


		//detect hits
		for (int i = 0; i < ENEMY_PROJECTILE_COUNT; i++)
		{
			if (this->_enemyProjectile[i].getPosition().x < this->_player.getPosition().x + this->_player.getGlobalBounds().width &&
				this->_enemyProjectile[i].getPosition().x > this->_player.getPosition().x &&
				this->_enemyProjectile[i].getPosition().y < this->_player.getPosition().y + this->_player.getGlobalBounds().height &&
				this->_enemyProjectile[i].getPosition().y > this->_player.getPosition().y)
			{
				playerHealth = playerHealth - ENEMY_DAMAGE;
				this->_enemyProjectile[i].setPosition(-20, 0);
			}

			else if (this->_enemyProjectile[i].getPosition().x + this->_enemyProjectile[i].getGlobalBounds().width < this->_player.getPosition().x + this->_player.getGlobalBounds().width &&
				this->_enemyProjectile[i].getPosition().x + this->_enemyProjectile[i].getGlobalBounds().width > this->_player.getPosition().x &&
				this->_enemyProjectile[i].getPosition().y < this->_player.getPosition().y + this->_player.getGlobalBounds().height &&
				this->_enemyProjectile[i].getPosition().y > this->_player.getPosition().y)
			{
				playerHealth = playerHealth - ENEMY_DAMAGE;
				this->_enemyProjectile[i].setPosition(-20, 0);
			}

			else if (this->_enemyProjectile[i].getPosition().x < this->_player.getPosition().x + this->_player.getGlobalBounds().width &&
				this->_enemyProjectile[i].getPosition().x > this->_player.getPosition().x &&
				this->_enemyProjectile[i].getPosition().y + this->_enemyProjectile[i].getGlobalBounds().height < this->_player.getPosition().y + this->_player.getGlobalBounds().height &&
				this->_enemyProjectile[i].getPosition().y + this->_enemyProjectile[i].getGlobalBounds().height > this->_player.getPosition().y)
			{
				playerHealth = playerHealth - ENEMY_DAMAGE;
				this->_enemyProjectile[i].setPosition(-20, 0);
			}

			else if (this->_enemyProjectile[i].getPosition().x + this->_enemyProjectile[i].getGlobalBounds().width < this->_player.getPosition().x + this->_player.getGlobalBounds().width &&
				this->_enemyProjectile[i].getPosition().x + this->_enemyProjectile[i].getGlobalBounds().width > this->_player.getPosition().x &&
				this->_enemyProjectile[i].getPosition().y + this->_enemyProjectile[i].getGlobalBounds().height < this->_player.getPosition().y + this->_player.getGlobalBounds().height &&
				this->_enemyProjectile[i].getPosition().y + this->_enemyProjectile[i].getGlobalBounds().height > this->_player.getPosition().y)
			{
				playerHealth = playerHealth - ENEMY_DAMAGE;
				this->_enemyProjectile[i].setPosition(-20, 0);
			}
		}
		for (int i = 0; i < PLAYER_PROJECTILE_COUNT; i++)
		{
			for (int x = 0; x < ENEMIES_PER_WAVE; x++) {
				if (this->_playerProjectile[i].getPosition().x < this->_enemy[x].getPosition().x + this->_enemy[x].getGlobalBounds().width &&
					this->_playerProjectile[i].getPosition().x > this->_enemy[x].getPosition().x &&
					this->_playerProjectile[i].getPosition().y < this->_enemy[x].getPosition().y + this->_enemy[x].getGlobalBounds().height &&
					this->_playerProjectile[i].getPosition().y > this->_enemy[x].getPosition().y)
				{
					enemyHealth[x] = enemyHealth[x] - PLAYER_DAMAGE;
					this->_playerProjectile[i].setPosition(-20, 0);
				}

				else if (this->_playerProjectile[i].getPosition().x + this->_playerProjectile[i].getGlobalBounds().width < this->_enemy[x].getPosition().x + this->_enemy[x].getGlobalBounds().width &&
					this->_playerProjectile[i].getPosition().x + this->_playerProjectile[i].getGlobalBounds().width > this->_enemy[x].getPosition().x &&
					this->_playerProjectile[i].getPosition().y < this->_enemy[x].getPosition().y + this->_enemy[x].getGlobalBounds().height &&
					this->_playerProjectile[i].getPosition().y > this->_enemy[x].getPosition().y)
				{
					enemyHealth[x] = enemyHealth[x] - PLAYER_DAMAGE;
					this->_playerProjectile[i].setPosition(-20, 0);
				}

				else if (this->_playerProjectile[i].getPosition().x < this->_enemy[x].getPosition().x + this->_enemy[x].getGlobalBounds().width &&
					this->_playerProjectile[i].getPosition().x > this->_enemy[x].getPosition().x &&
					this->_playerProjectile[i].getPosition().y + this->_playerProjectile[i].getGlobalBounds().height < this->_enemy[x].getPosition().y + this->_enemy[x].getGlobalBounds().height &&
					this->_playerProjectile[i].getPosition().y + this->_playerProjectile[i].getGlobalBounds().height > this->_enemy[x].getPosition().y)
				{
					enemyHealth[x] = enemyHealth[x] - PLAYER_DAMAGE;
					this->_playerProjectile[i].setPosition(-20, 0);
				}
				else if (this->_playerProjectile[i].getPosition().x + this->_playerProjectile[i].getGlobalBounds().width < this->_enemy[x].getPosition().x + this->_enemy[x].getGlobalBounds().width &&
					this->_playerProjectile[i].getPosition().x + this->_playerProjectile[i].getGlobalBounds().width > this->_enemy[x].getPosition().x &&
					this->_playerProjectile[i].getPosition().y + this->_playerProjectile[i].getGlobalBounds().height < this->_enemy[x].getPosition().y + this->_enemy[x].getGlobalBounds().height &&
					this->_playerProjectile[i].getPosition().y + this->_playerProjectile[i].getGlobalBounds().height > this->_enemy[x].getPosition().y)
				{
					enemyHealth[x] = enemyHealth[x] - PLAYER_DAMAGE;
					this->_playerProjectile[i].setPosition(-20, 0);
				}
			}
		}

		//HP bars
		if (this->playerHealth >= 0) {
			this->_playerHealth.setScale(this->playerHealth / 100, 1);
		}
		this->_playerHealth.setPosition(this->_player.getPosition().x - 
			(this->_playerHealth.getGlobalBounds().width - this->_player.getGlobalBounds().width)/2, 
			this->_player.getPosition().y + this->_player.getGlobalBounds().height);
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			if (this->enemyHealth[i] >= 0) {
				this->_enemyHealth[i].setScale(this->enemyHealth[i] / 100, 1);
			}
			this->_enemyHealth[i].setPosition(this->_enemy[i].getPosition().x - 
				(this->_enemyHealth[i].getGlobalBounds().width - this->_enemy[i].getGlobalBounds().width) / 2,
				this->_enemy[i].getPosition().y - this->_enemyHealth[i].getGlobalBounds().height);
		}

		//Enemy death
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			if (this->enemyHealth[i] <= 0)
			{
				this->enemyHealth[i] = ENEMY_MAX_HEALTH;
				this->deleteEnemy[i] = 1;
				this->_enemy[i].setPosition(-20, 0);
				this->score = this->score + SCORE_FROM_ENEMY;
			}
		}

		//Point counting
		if (this->scoreCounter / FPS_CAP >= SCORE_INCOME_RATE)
		{
			this->score = this->score + SCORE_INCOME_AMMOUNT;
		}
		this->scoreCounter++;

		//reset wave
		int sum = 1;
		for (int i = 0; i < ENEMIES_PER_WAVE; i++)
		{
			sum = sum * this->deleteEnemy[i];
		}
		if (sum != 0) {
			for (int i = 0; i < ENEMIES_PER_WAVE; i++)
			{
				this->_enemy[i].setPosition(rand() % SCREEN_WIDTH + 1, rand() % SCREEN_HEIGHT / 2 + 1);
				this->deleteEnemy[i] = 0;
			}
		}

		//Game over
		if (this->playerHealth < 1)
		{
			std::cout << "Game over" << std::endl;
		}

	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear();
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_player);
		this->_data->window.draw(this->_playerHealth);
		for (int i = 0; i < ENEMIES_PER_WAVE; i++) {
			if (this->deleteEnemy[i] == 0) {
				this->_data->window.draw(this->_enemy[i]);
				this->_data->window.draw(this->_enemyHealth[i]);
			}
		}
		for (int i = 0; i < ENEMY_PROJECTILE_COUNT; i++) {
			if (this->_enemyProjectile[i].getPosition().y < SCREEN_HEIGHT) {
				this->_data->window.draw(this->_enemyProjectile[i]);
			}
		}
		for (int i = 0; i < PLAYER_PROJECTILE_COUNT; i++) {
			if (this->_playerProjectile[i].getPosition().y > 0) {
				this->_data->window.draw(this->_playerProjectile[i]);
			}
		}
		this->_data->window.display();
	}
}