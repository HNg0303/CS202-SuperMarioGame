#pragma once
#include <SFML/Graphics.hpp>
#include "Windows.h"
#include <iostream>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

#define MAX_NUMBER_OF_ITEMS 5
#define NUMBER_OF_RESULTS 5
#define NUMBER_OF_CHARACTERS 2
#define NUMBER_OF_LEVELS 3
#define NUMBER_OF_PAUSE 2

class result {
public:
	std::string date;
	std::string score;
	std::string time;
	std::string coins;
	std::string userName;
	result(std::string date, std::string score, std::string time, std::string coins, std::string userName) :date(date), score(score), time(time), coins(coins), userName(userName) {};
};

class Menu
{
protected:

	int selectedItemIndex = 0;
	std::vector<sf::Text> texts;
	sf::Font font;

public:
	Menu();
	Menu(int n, std::vector<std::string> textVec);
	~Menu();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void handleUpDown(sf::Event sfEvent);
	void handleLeftRight(sf::Event sfEvent);
	int GetPressedItem();
	void setPressedItem(int item);
	
	void drawMenuBackground(sf::RenderWindow& window, sf::Texture texture, float x, float y);
	void drawCenterMenuBackground(sf::RenderWindow& window, sf::Texture texture);
	void drawItemSelection(sf::RenderWindow& window, std::vector<sf::Text> &texts, float size, float x, float y, float width, float space, float Ypos);
	sf::Texture loadTexture(std::string name);
};

class MainMenu: public Menu
{

private:
	std::vector<result> loadedResults;
	std::vector<sf::Text> resultsToDisplay;

public:
	MainMenu(): Menu(){}
	MainMenu(int n, std::vector<std::string> textVec) : Menu(n, textVec){}
	
	void drawMainMenu(sf::RenderWindow& window, float x, float y); //draw main menu itself
	
	//draw items
	void drawHelpMenu(sf::RenderWindow& window, int center);
	void drawScoreboard(sf::RenderWindow& window, int center);
	void drawScoreboardBackground(sf::RenderWindow& window, int center);
	void drawScoreaboardDetail(sf::RenderWindow& window, int center);

	//handling scoreboard
	void readResultsFromFile();
	void loadResultsToArray();
	static bool comparator(result i1, result i2);
	void sortResults();
};

class ChooseCharacterMenu : public Menu
{
public:
	ChooseCharacterMenu(int n, std::vector<std::string> textVec) : Menu(n, textVec) {}
	void drawChooseCharacter(sf::RenderWindow& window, int center);
};

class ChooseLevelMenu : public Menu
{
public:
	ChooseLevelMenu(int n, std::vector<std::string> textVec) : Menu(n, textVec) {}
	void drawChooseLevel(sf::RenderWindow& window, float x, float y);
};

class PauseMenu : public Menu
{
public:
	PauseMenu(int n, std::vector<std::string> textVec) : Menu(n, textVec) {}
	void drawPauseMenu(sf::RenderWindow& window, float x, float y);
};

class AskRestart : public Menu
{
public:
	AskRestart(int n, std::vector<std::string> textVec) : Menu(n, textVec) {}
	void drawAskRestart(sf::RenderWindow& window, float x, float y);
};
