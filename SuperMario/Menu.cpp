#include "Menu.h"
#include <fstream>

Menu::Menu()
{
	try {
		if (!font.loadFromFile("Resource/asset/font/PixeloidSans.ttf"))
		{
			throw - 1;
		}
	}
	catch (int)
	{
		std::cout << "Error: Cannot load menu font.";
		exit(1);
	}

	selectedItemIndex = 0;
}

Menu::Menu(int n, std::vector<std::string> textVec)
{
	try {
		if (!font.loadFromFile("Resource/asset/font/PixeloidSans.ttf"))
		{
			throw - 1;
		}
	}
	catch (int)
	{
		std::cout << "Error: Cannot load menu font.";
		exit(1);
	}

	selectedItemIndex = 0;

	texts.resize(n);
	for (int i = 0; i < n; ++i)
	{
		texts[i].setFont(font);
		texts[i].setFillColor(sf::Color::White);
		texts[i].setString(textVec[i]);
	}
	sf::Color color(248, 233, 67);
	texts[0].setFillColor(color);
	
}


Menu::~Menu()
{

}


bool MainMenu::comparator(result  i1, result  i2)
{
	return (i1.score > i2.score);
}

void MainMenu::sortResults()
{
	std::sort(loadedResults.begin(), loadedResults.end(), comparator);
}

void MainMenu::readResultsFromFile() //load scoreboard
{
	std::string line, line2;
	std::string coinsTemp;;
	std::string scoreTemp;
	std::string timeTemp;
	std::ifstream infile;
	std::string userName;

	loadedResults.clear();

	infile.open("results/results.txt");

	try
	{
		if (!infile)
		{
			throw - 1;
		}
	}
	catch (int)
	{
		std::cout << "can not load results";
		exit(1);
	}

	int counter = 0;

	while (getline(infile, line)) {


		getline(infile, line2);


		coinsTemp = (std::to_string(line2[7] - 48) + std::to_string(line2[8] - 48) + std::to_string(line2[9] - 48));
		scoreTemp = (std::to_string(line2[18] - 48) + std::to_string(line2[19] - 48) + std::to_string(line2[20] - 48) + std::to_string(line2[21] - 48));
		timeTemp = (std::to_string(line2[29] - 48) + std::to_string(line2[30] - 48) + std::to_string(line2[31] - 48));

		userName.clear();
		for (int i = 33; i < line2.length(); i++)
		{
			userName = userName + line2[i];
		}

		loadedResults.push_back(result(line, scoreTemp, timeTemp, coinsTemp, userName));

		coinsTemp.clear();
		scoreTemp.clear();
		timeTemp.clear();

		counter++;
		getline(infile, line2);
	}

	infile.close();

	sortResults();
}

void MainMenu::loadResultsToArray() //set scoreboard details
{
	readResultsFromFile();
	int numberOfResults = 5;
	if (loadedResults.size() < 5)
		numberOfResults = loadedResults.size();

	resultsToDisplay.resize(numberOfResults);
	for (int i = 0; i < numberOfResults; i++)
	{
		std::string toDisplay = std::to_string(i + 1) + ". Score: " + loadedResults.at(i).score + "   coins: " + loadedResults.at(i).coins + " time " + loadedResults.at(i).time + "    " + loadedResults.at(i).userName;
		resultsToDisplay[i].setString(toDisplay);

		resultsToDisplay[i].setFont(font);
		resultsToDisplay[i].setFillColor(sf::Color::White);
	}
}

void MainMenu::drawScoreboardBackground(sf::RenderWindow& window, int center) //draw scoreboard background
{
	sf::Texture texture = loadTexture("Resource/asset/image/scoreboard.png");
	drawCenterMenuBackground(window, texture);
}

void MainMenu::drawScoreaboardDetail(sf::RenderWindow& window, int center) //draw scoreboard details
{
	for (int i = 0; i < NUMBER_OF_RESULTS; i++)
	{
		resultsToDisplay[i].setCharacterSize(23);
		resultsToDisplay[i].setPosition(sf::Vector2f(center - 320, 400 + 50*i));
		window.draw(resultsToDisplay[i]);
	}
}

void MainMenu::drawScoreboard(sf::RenderWindow& window, int center) //draws scoreboard
{
	drawScoreboardBackground(window, center); //background image for score
	drawScoreaboardDetail(window, center);
}

void MainMenu::drawHelpMenu(sf::RenderWindow& window, int center) //help menu
{
	sf::Texture texture = loadTexture("Resource/asset/image/help.png");
	drawCenterMenuBackground(window, texture);
}

void ChooseCharacterMenu::drawChooseCharacter(sf::RenderWindow& window, int center) //background for choosing character background
{
	sf::Texture texture = loadTexture("Resource/asset/image/choosecharacter.png");
	drawMenuBackground(window, texture, 0, 0);

	float H = 750;
	texts[0].setCharacterSize(50);
	texts[0].setPosition(sf::Vector2f(200, H));
	window.draw(texts[0]);

	texts[1].setCharacterSize(50);
	texts[1].setPosition(sf::Vector2f(890, H));
	window.draw(texts[1]);
}

void ChooseLevelMenu::drawChooseLevel(sf::RenderWindow& window, float x, float y) //background for choosing level
{
	sf::Texture texture = loadTexture("Resource/asset/image/chooseLevel.png");
	drawMenuBackground(window, texture, 0, 0);
	drawItemSelection(window, texts, 60, x, y, 662, 80, 150);
}
void MainMenu::drawMainMenu(sf::RenderWindow& window, float x, float y) //draw menu screen
{
	sf::Texture texture = loadTexture("Resource/asset/image/MainMenu.png");
	drawMenuBackground(window, texture, 0, 0);
	drawItemSelection(window, texts, 35, x, y, 553, 50, 140);
}

void PauseMenu::drawPauseMenu(sf::RenderWindow& window, float x, float y)
{
	sf::Texture texture = loadTexture("Resource/asset/image/PauseMenu.png");
	drawMenuBackground(window, texture, x, y);
	drawItemSelection(window, texts, 40, x, y, 630, 70, 160);
}

void AskRestart::drawAskRestart(sf::RenderWindow& window, float x, float y)
{
	sf::Texture texture = loadTexture("Resource/asset/image/Ask_Restart.png");
	drawMenuBackground(window, texture, x, y);

	float X = 400;
	float Y = 460;
	texts[0].setCharacterSize(65);
	texts[0].setPosition(sf::Vector2f(X, Y));
	window.draw(texts[0]);

	texts[1].setCharacterSize(65);
	texts[1].setPosition(sf::Vector2f(X + 340, Y));
	window.draw(texts[1]);
}

sf::Texture Menu::loadTexture(std::string name)
{
	sf::Texture texture;
	try {
		if (!texture.loadFromFile(name))
		{
			throw - 1;
		}
	}
	catch (int)
	{
		std::cout << "Error: Cannot load " + name;
		exit(1);
	}

	return texture;

}

//draw menu background with {x,y}
void Menu::drawMenuBackground(sf::RenderWindow& window, sf::Texture texture, float x, float y)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition({ x,y });
	window.draw(sprite);
}

//draw menu background in center
void Menu::drawCenterMenuBackground(sf::RenderWindow& window, sf::Texture texture)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition({ (WINDOW_WIDTH - sprite.getGlobalBounds().width) / 2, (WINDOW_HEIGHT - sprite.getGlobalBounds().height) / 2 });
	window.draw(sprite);
}

//draw vertically item selection
void Menu::drawItemSelection(sf::RenderWindow& window, std::vector<sf::Text>& texts, float size, float x, float y, float width, float space, float Ypos)
{ 
	//{x,y}
	//space: between lines
	//Ypos: y position of first item

	int num = texts.size();
	for (int i = 0; i < num; ++i)
	{
		texts[i].setCharacterSize(size);
		float X = x + (width - texts[i].getGlobalBounds().width) / (2.0);
		texts[i].setPosition(sf::Vector2f(X, y + Ypos + space * i));
		window.draw(texts[i]);
	}
}



