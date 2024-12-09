#pragma once
#pragma once
#include "Menu.h"
#include "entity.h"
#include"Turtle.h"
#include "Menu.h"
#include"Spikey.h"
#include"flyTur.h"
#include"Plant1.h"
#include"Plant2.h"


#include<Windows.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900


class Game
{
private:
	int curState = 0;
public:
	enum class GameState
	{
		MainMenu,
		ChooseCharacter,
		Scoreboard,
		HelpMenu,
		Exit,
		ChooseLevel,
		PauseMenu,
		AskRestart,
		PlayingGame
	};

	
	//std::string entityName[11] = { "cheep","coin","flame","goombas","koopas","levelUp","plant1","plant2","plant3","qblock","star" };
	std::string entityName[2] = { "goombas","koopas" };

	sf::RenderWindow* window;
	sf::View view;
	
	//Menu
	MainMenu mainMenu;
	ChooseCharacterMenu chooseCharacterMenu;
	ChooseLevelMenu chooseLevel;
	PauseMenu pauseMenu;
	AskRestart askRestart;
	sf::Event sfEvent;

	//Mobs
	std::vector<Entity> mobs;

	
	Game() : mainMenu(5, {"START GAME", "RESTART", "SCOREBOARD", "HELP", "EXIT"}), 
		chooseCharacterMenu(2, {"MARIO", "LUGI"}),
		chooseLevel(3, {"EASY","MEDIUM","HARD"}),
		pauseMenu(3,{"CONTINUE", "RESTART","BACK TO MAIN MENU"}),
		askRestart(2,{"YES","NO"}),
		sfEvent()
	{
		//loadMobs();
		mainMenu.loadResultsToArray();
		this->window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario");
		view.reset(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
	}

	virtual ~Game();
	void run();
	
	void handleMainMenu();
	void handleChooseCharacter();
	void handleScoreboard();
	void handleHelpMenu();
	void handlePlayingGame();
	void handleChooseLevel();
	void handlePauseMenu();
	void handleAskRestart();


	//Mobs
	void loadMobs();
	void drawMobs();

	std::vector<sf::Texture>  loadFrame(std::string folderPath);
	//void loadFrame(std::string folderPath, sf::RenderWindow& window);
	
};
