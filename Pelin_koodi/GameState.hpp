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
		sf::Sprite _enemy;
		sf::Sprite _playerProjectile;
		sf::Sprite _enemyProjectile;

		int score;
	};
}
