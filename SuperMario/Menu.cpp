#include "Menu.h"
#include "INCLUDE.h"
#include "Resources.h"
#include <fstream>
#include <filesystem>

Menu::Menu()
{
	try {
		if (!font.loadFromFile(convertToUnixPath(fs::current_path().string() + "/Resource/asset/font/PixeloidSans.ttf")))
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
		if (!font.loadFromFile(convertToUnixPath(fs::current_path().string() + "/Resource/asset/font/PixeloidSans.ttf")))
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
	return (i1.coin > i2.coin || (i1.coin == i2.coin && i1.time < i2.time));
}

void MainMenu::sortResults()
{
	sort(loadedResults.begin(), loadedResults.end(), comparator);
}

void MainMenu::readResultsFromFile() //load scoreboard
{
	ifstream inf;
	inf.open(convertToUnixPath(fs::current_path().string() + "/Resource/asset/txt/results.txt"));
	if (inf.is_open())
	{
		loadedResults.clear();
		string levelStr, username;
		int coin, time;
		while (inf >> levelStr >> coin >> time >> username)
		{
			loadedResults.push_back(result(levelStr, coin, time, username));
		}
		inf.close();
		sortResults();
		cout << "read results succesfully" << endl;
	}
	else cout << "Fail to read results" << endl;
}

void MainMenu::saveResultsFromFile(string level, int coin, int time, string name)
{
	ofstream ouf;
	ouf.open(convertToUnixPath(fs::current_path().string() + "/Resource/asset/txt/results.txt"), ios_base::app);
	if (ouf.is_open())
	{
		ouf << level << " " << coin << " " << time << " " << name << endl;
		cout << "save result succesfully" << endl;
		ouf.close();
	}
	else cout << "Fail to save result" << endl;
}

//level coins time name
//Coins: 064 Score : 0890 Time : 055 RUBIKA

void MainMenu::loadResultsToArray() // Set scoreboard details
{
	int numberOfResults = 5;
	if (loadedResults.size() < 5)
		numberOfResults = loadedResults.size();

	resultsToDisplay.resize(numberOfResults + 1);

	std::ostringstream header;
	header << std::left << std::setw(7) << "No."
		<< std::setw(15) << "Level"
		<< std::setw(10) << "Coin"
		<< std::setw(10) << "Time"
		<< "Username";
	resultsToDisplay[0].setString(header.str());
	resultsToDisplay[0].setFont(font);
	resultsToDisplay[0].setFillColor(sf::Color::White);

	for (int i = 0; i < numberOfResults; i++)
	{
		std::ostringstream oss;
		oss << std::right << std::to_string(i + 1)  // Cột số thứ tự
			<< std::setw(12) << loadedResults[i].level                  // Cột "Level"
			<< std::setw(17 - loadedResults[i].level.length()) << loadedResults[i].coin                   // Cột "Coin"
			<< std::setw(13 - to_string(loadedResults[i].coin).length()) << loadedResults[i].time                   // Cột "Time"
			<< std::setw(19 - to_string(loadedResults[i].time).length()) << loadedResults[i].username;                               // Cột "Username"

		resultsToDisplay[i+1].setString(oss.str());
		resultsToDisplay[i+1].setFont(font);
		resultsToDisplay[i+1].setFillColor(sf::Color::White);
	}
}

void MainMenu::drawScoreboardBackground(sf::RenderWindow& window, int center) //draw scoreboard background
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/scoreboard.png"));
	drawCenterMenuBackground(window, texture);
}

void MainMenu::drawScoreaboardDetail(sf::RenderWindow& window, int center) //draw scoreboard details
{
	for (int i = 0; i < min(6, int(resultsToDisplay.size())); i++)
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
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/help.png"));
	drawCenterMenuBackground(window, texture);
}

void ChooseCharacterMenu::drawChooseCharacter(sf::RenderWindow& window, int center) //background for choosing character background
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/choosecharacter.png"));
	drawMenuBackground(window, texture, 0, 0);

	float H = 750;
	texts[0].setCharacterSize(50);
	texts[0].setPosition(sf::Vector2f(200, H));
	window.draw(texts[0]);

	texts[1].setCharacterSize(50);
	texts[1].setPosition(sf::Vector2f(880, H));
	window.draw(texts[1]);
}

void ChooseLevelMenu::drawChooseLevel(sf::RenderWindow& window, float x, float y) //background for choosing level
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/chooseLevel.png"));
	drawMenuBackground(window, texture, 0, 0);
	drawItemSelectionVertical(window, texts, 60, x, y, 662, 80, 150);
}

void ChooseThemes::drawChooseThemes(sf::RenderWindow& window, float x, float y) //background for choosing themes
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/chooseThemes.png"));
	drawCenterMenuBackground(window, texture);
	drawItemSelectionVertical(window, texts, 26, x, y, 553, 50, 150);
}


void MainMenu::drawMainMenu(sf::RenderWindow& window, float x, float y) //draw menu screen
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/MainMenu.png"));
	drawMenuBackground(window, texture, 0, 0);
	drawItemSelectionVertical(window, texts, 35, x, y, 553, 50, 140);
}

void PauseMenu::drawPauseMenu(sf::RenderWindow& window, float x, float y)
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/PauseMenu.png"));
	drawMenuBackground(window, texture, x, y);
	drawItemSelectionVertical(window, texts, 40, x, y, 630, 70, 160);
}

void AskRestart::drawAskRestart(sf::RenderWindow& window, float x, float y)
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/Ask_Restart.png"));
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

void AskNextLevel::drawAskNextLevel(sf::RenderWindow& window, float x, float y)
{
	sf::Texture texture = loadTexture(convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/AskNextLevel.png"));
	drawMenuBackground(window, texture, x, y);

	float X = 490;
	float Y = 500;
	texts[0].setCharacterSize(43);
	texts[0].setPosition(sf::Vector2f(X, Y));
	window.draw(texts[0]);

	texts[1].setCharacterSize(43);
	texts[1].setPosition(sf::Vector2f(X, Y + 80));
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
void Menu::drawItemSelectionVertical(sf::RenderWindow& window, std::vector<sf::Text>& texts, float size, float x, float y, float width, float space, float Ypos)
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
void Menu::drawItemSelectionHorizontal(sf::RenderWindow& window, std::vector<sf::Text>& texts, float size, float x, float y, float height, float space, float Xpos)
{
	// {x, y} is the starting point of the text area
	// space: distance between items horizontally
	// Xpos: x position of the first item

	int num = texts.size();
	for (int i = 0; i < num; ++i)
	{
		texts[i].setCharacterSize(size);
		float Y = y + (height - texts[i].getGlobalBounds().height) / 2.0;
		texts[i].setPosition(sf::Vector2f(x + Xpos + space * i, Y));
		window.draw(texts[i]);
	}
}



