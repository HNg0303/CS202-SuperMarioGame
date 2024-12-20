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
	int mapState = 0;
	Map* map = nullptr;
	Camera* camera = nullptr;
	Character* character = nullptr;
	//main game
	
	
	Game* game = nullptr;
	bool isRestarted = false;
	//Constructor Private for singleton.
	GameFlow() : mainMenu(6, { "START GAME", "RESTART", "SCOREBOARD", "THEMES", "HELP", "EXIT" }),
		chooseCharacterMenu(2, { "MARIO", "LUIGI" }),
		chooseThemes(2, {"Default", "Ice"}),
		chooseLevel(3, { "EASY","MEDIUM","HARD" }),
		pauseMenu(3, { "CONTINUE", "RESTART","BACK TO MAIN MENU" }),
		askRestart(2, { "YES","NO" }),
		sfEvent()
	{
		loadPausedTimeToFile();
		
		mainMenu.loadResultsToArray();
		//game->loadEntities(entities);
		
		//map = new Map(1.0f);
		
		camera = new Camera(30.0f);
		

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
		PlayingGame,
		LooseGame,
		WinGame,
		Themes
	};
	
	static GameFlow* getInstance();
	//clock 


	sf::Clock gameClock;
	sf::Clock clock;
	sf::Time pausedTime;
	int coins = 0;
	bool isPaused = true;
	bool alreadyPlayed = false;


	sf::RenderWindow* window;
	sf::View view;
	sf::View UIView;

	//Menu
	MainMenu mainMenu;
	ChooseCharacterMenu chooseCharacterMenu;
	ChooseLevelMenu chooseLevel;
	PauseMenu pauseMenu;
	AskRestart askRestart;
	ChooseThemes chooseThemes;
	
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
	void handleLooseGame();
	void handleWinGame();
	void handleChooseThemes();
	void handleClosed();
	void handleGameInfo();
	//void handleContinue();

	//Game Info
	void loadFont(sf::Font& font);
	void setupText(sf::Text& text, const sf::Font& font, const std::string& string, unsigned int size, const sf::Color& color);
	void setupHorizontalPosition(sf::Text& labelText, sf::Text& valueText, float& x, float y, float padding1, float padding2);
	void setupVerticalPosition(sf::Text& labelText, sf::Text& valueText, float x, float &y, float padding1, float padding2);
	std::string formatTime(const sf::Time& time);
	std::string formatValue(int value);
	void drawText(sf::Text& labelText, sf::Text& valueText);


	std::vector<sf::Texture> loadFrame(std::string folderPath);


	//Clock
	void pauseClock();
	void resumeClock();
	void savePausedTimeToFile();
	void loadPausedTimeToFile();

	//void restart
	void Restart();

	//void startGame
	void Start();
};