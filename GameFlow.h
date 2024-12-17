#pragma once
#include "Menu.h"
#include "entity.h"
#include "Game.h"
#include "Resources.h"



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900


class GameFlow
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
	
	//clock 

	/*static sf::Clock clock;
	static sf::Time pausedTime;
	static bool isPaused;
	*/

	sf::Clock gameClock;
	sf::Clock clock;
	sf::Time pausedTime;
	bool isPaused = true;

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
	std::vector<std::unique_ptr<Entity>> entities;

	//Main Game:
	Game* game = nullptr;

	//Resource
	Resources resources;

	//Renderer
	Renderer* renderer;
	

	GameFlow(Game* game) : mainMenu(5, { "START GAME", "RESTART", "SCOREBOARD", "HELP", "EXIT" }),
		chooseCharacterMenu(2, { "MARIO", "LUGI" }),
		chooseLevel(3, { "EASY","MEDIUM","HARD" }),
		pauseMenu(3, { "CONTINUE", "RESTART","BACK TO MAIN MENU" }),
		askRestart(2, { "YES","NO" }),
		sfEvent(),
		game(game)
	{
		loadPausedTimeToFile();
		loadMobs();
		mainMenu.loadResultsToArray();
		game->loadEntities(entities);
		this->window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario");
		this->window->setFramerateLimit(60);
		this->renderer = new Renderer(*this->window);
		view.reset(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
		resources.loadResource();
	}

	virtual ~GameFlow();
	void run();

	void handleMainMenu();
	void handleHelpMenu();
	void handlePauseMenu();
	void handleChooseCharacter();
	void handleScoreboard();
	void handlePlayingGame();
	void handleChooseLevel();
	void handleAskRestart();
	void handleEntity();
	void handleClosed();

	//Mobs
	void loadMobs();
	//void drawMobs();

	std::vector<sf::Texture>  loadFrame(std::string folderPath);
	//void loadFrame(std::string folderPath, sf::RenderWindow& window);


	//Clock
	void pauseClock();
	void resumeClock();
	void handleClock();
	void savePausedTimeToFile();
	void loadPausedTimeToFile();

};