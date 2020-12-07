#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace Avaruuspeli
{
	class GameState : public State
	{
	public:
		GameState(GameDataRef data);

		void Init();

		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;
		sf::Sprite _player;
		sf::Sprite _enemy[ENEMIES_PER_WAVE];
		sf::Sprite _playerHealth;
		sf::Sprite _enemyHealth[ENEMIES_PER_WAVE];
		sf::Sprite _playerProjectile[PLAYER_PROJECTILE_COUNT];
		sf::Sprite _enemyProjectile[ENEMY_PROJECTILE_COUNT];

		int score;
		float playerBulletCD;
		bool playerBulletOnCD;
		float enemyBulletCD[ENEMIES_PER_WAVE];
		int playerBullet;
		int enemyBullet;
		int enemyDir[ENEMIES_PER_WAVE];
		float playerHealth;
		float enemyHealth[ENEMIES_PER_WAVE];
		int deleteEnemy[ENEMIES_PER_WAVE];
		float scoreCounter;
		bool advanceWave;
	};
}
