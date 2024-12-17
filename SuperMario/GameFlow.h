#pragma once
#include "Menu.h"
#include "Game.h"



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900


class GameFlow
{
private:
	inline static GameFlow* instance = nullptr;
	int curState = 0;
	Map* map = nullptr;
	Camera* camera = nullptr;
	Character* character = nullptr;
	//main game
	Game* game = nullptr;
	bool isRestarted = false;
	//Constructor Private for singleton.
	GameFlow() : mainMenu(5, { "START GAME", "RESTART", "SCOREBOARD", "HELP", "EXIT" }),
		chooseCharacterMenu(2, { "MARIO", "LUIGI" }),
		chooseLevel(3, { "EASY","MEDIUM","HARD" }),
		pauseMenu(3, { "CONTINUE", "RESTART","BACK TO MAIN MENU" }),
		askRestart(2, { "YES","NO" }),
		sfEvent()
	{
		loadPausedTimeToFile();
		//loadMobs();
		mainMenu.loadResultsToArray();
		//game->loadEntities(entities);
		string name;
		map = new Map(1.0f);
		camera = new Camera(30.0f);
		cout << "Enter your Hero Name(Luigi or Mario): "; getline(cin, name);
		if (name == "Luigi")
			character = CharacterFactory::createCharacter(LUIGI);
		else if (name == "Mario")
			character = CharacterFactory::createCharacter(MARIO);

		this->window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mario");
		this->window->setFramerateLimit(60);
		this->renderer = new Renderer(*this->window);
		//view.reset(sf::FloatRect(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT));
		UIView = sf::View(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
		resources.loadResource();
	}

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
	
	static GameFlow* getInstance();
	//clock 

	/*static sf::Clock clock;
	static sf::Time pausedTime;
	static bool isPaused;
	*/
	


	sf::Clock gameClock;
	sf::Clock clock;
	sf::Time pausedTime;
	int coins = 0;
	bool isPaused = true;
	bool alreadyPlayed = false;

	//std::string entityName[11] = { "cheep","coin","flame","goombas","koopas","levelUp","plant1","plant2","plant3","qblock","star" };
	std::string entityName[2] = { "goombas","koopas" };

	sf::RenderWindow* window;
	sf::View view;
	sf::View UIView;

	//Menu
	MainMenu mainMenu;
	ChooseCharacterMenu chooseCharacterMenu;
	ChooseLevelMenu chooseLevel;
	PauseMenu pauseMenu;
	AskRestart askRestart;
	sf::Event sfEvent;

	//Mobs
	std::vector<std::unique_ptr<Entity>> entities;

	//Resource
	Resources resources;

	//Renderer
	Renderer* renderer;
	

	

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
	//void loadMobs();
	//void drawMobs();

	std::vector<sf::Texture>  loadFrame(std::string folderPath);
	//void loadFrame(std::string folderPath, sf::RenderWindow& window);


	//Clock
	void pauseClock();
	void resumeClock();
	void handleClock();
	void savePausedTimeToFile();
	void loadPausedTimeToFile();

	//void restart
	void Restart();

};