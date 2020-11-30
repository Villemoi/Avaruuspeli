#include <sstream>
#include "MainMenuState.hpp"
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include <iostream>

namespace Avaruuspeli
{
	MainMenuState::MainMenuState(GameDataRef data) : _data(data)
	{

	}
	void MainMenuState::Init()
	{
		//load textures
		this->_data->assets.LoadTexture("Background", MAIN_MENU_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Play Button", MAIN_MENU_PLAY_FILEPATH);
		this->_data->assets.LoadTexture("Highscore Button", MAIN_MENU_HIGHSCORE_FILEPATH);
		this->_data->assets.LoadTexture("Exit Button", MAIN_MENU_EXIT_FILEPATH);
		this->_data->assets.LoadTexture("Title", MAIN_MENU_TITLE_FILEPATH);
		//set textures to sprites
		this->_background.setTexture(this->_data->assets.GetTexture("Background"));
		this->_playButton.setTexture(this->_data->assets.GetTexture("Play Button"));
		this->_highscoreButton.setTexture(this->_data->assets.GetTexture("Highscore Button"));
		this->_exitButton.setTexture(this->_data->assets.GetTexture("Exit Button"));
		this->_title.setTexture(this->_data->assets.GetTexture("Title"));

		//play button position
		this->_playButton.setPosition((SCREEN_WIDTH / 2) - (this->_playButton.getGlobalBounds().width / 2),
			(SCREEN_HEIGHT / 10) * 4 - (this->_playButton.getGlobalBounds().height / 2));
		//highscore button position
		this->_highscoreButton.setPosition((SCREEN_WIDTH / 2) - (this->_highscoreButton.getGlobalBounds().width / 2),
			(SCREEN_HEIGHT / 10) * 5 - (this->_highscoreButton.getGlobalBounds().height / 2));
		//exit button position
		this->_exitButton.setPosition((SCREEN_WIDTH / 2) - (this->_exitButton.getGlobalBounds().width / 2),
			(SCREEN_HEIGHT / 10) * 6 - (this->_exitButton.getGlobalBounds().height / 2));
		//title position
		this->_title.setPosition((SCREEN_WIDTH / 2) - (this->_title.getGlobalBounds().width / 2),
			this->_title.getGlobalBounds().height * 0.1);
	}

	//Inputs
	void MainMenuState::HandleInput()
	{
		sf::Event event;
			while (this->_data->window.pollEvent(event))
			{
				//closing the window
				if (sf::Event::Closed == event.type) 
				{
					this->_data->window.close();
				}
				//pressing the play button
				if (this->_data->input.IsSpriteClicked(_playButton, sf::Mouse::Left, this->_data->window))
				{
					std::cout << "Go to Game Screen" << std::endl;
					this->_data->machine.AddState(StateRef(new GameState(_data)), true);
				}
				//pressing the highscore button
				if (this->_data->input.IsSpriteClicked(_highscoreButton, sf::Mouse::Left, this->_data->window))
				{
					std::cout << "Go to Highscore Screen" << std::endl;
				}
				//pressing the exit button
				if (this->_data->input.IsSpriteClicked(_exitButton, sf::Mouse::Left, this->_data->window))
				{
					this->_data->window.close();
				}
			}
	}

	void MainMenuState::Update(float dt) 
	{

	}

	void MainMenuState::Draw(float dt)
	{
		this->_data->window.clear();
		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_title);
		this->_data->window.draw(this->_playButton);
		this->_data->window.draw(this->_highscoreButton);
		this->_data->window.draw(this->_exitButton);
		this->_data->window.display();
	}
}