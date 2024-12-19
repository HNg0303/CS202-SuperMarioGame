#include "GameFlow.h"

GameFlow* GameFlow::getInstance() {
	if (instance == nullptr) {
		instance = new GameFlow();
	}
	return instance;
}

GameFlow::~GameFlow()
{
	savePausedTimeToFile();
	delete this->window;
}

/*
void GameFlow::loadMobs()
{
	std::string line;
	float x1, x2, y, speed;
	std::fstream infile;
	std::string mobName;

	entities.clear();
	
	infile.open("assets/txt/mobs.txt");
	if (!infile) {
		std::cout << "can not open file to read results from";
	}

	while (getline(infile, line)) {
		{
			std::stringstream ss(line);
			ss >> mobName;
			std::string temp;
			ss >> x1;
			ss >> y;
			ss >> x2;
			ss >> speed;

			//std::string entityName[11] = { "cheep","coin","flame","goombas","koopas","levelUp","plant1","plant2","plant3","qblock","star","winning","princess_winning" };

			if (mobName.compare("goombas") == 0)
				entities.push_back(std::make_unique<Moveable>("goombas", 0.5, speed, x1, x2, y));

			else if (mobName.compare("koopas") == 0)
				entities.push_back(std::make_unique<Moveable>("koopas", 0.5, speed, x1, x2, y));

			else if (mobName.compare("cheep") == 0)
				entities.push_back(std::make_unique<Moveable>("cheep", 0.5, speed, x1, x2, y));

			else if (mobName.compare("flame") == 0)
				entities.push_back(std::make_unique<Moveable>("flame", 0.5, speed, x1, x2, y));

			else if (mobName.compare("coin") == 0)
				entities.push_back(std::make_unique<Unmoveable>("coin", 0.3, x1, y));

			else if (mobName.compare("mushroom") == 0)
				entities.push_back(std::make_unique<Unmoveable>("mushroom", 0.3, x1, y));

			else if (mobName.compare("levelUp") == 0)
				entities.push_back(std::make_unique<Unmoveable>("levelUp", 0.3, x1, y));

			else if (mobName.compare("plant1") == 0)
				entities.push_back(std::make_unique<Unmoveable>("plant1", 0.3, x1, y));

			else if (mobName.compare("plant2") == 0)
				entities.push_back(std::make_unique<Unmoveable>("plant2", 0.3, x1, y));

			else if (mobName.compare("plant3") == 0)
				entities.push_back(std::make_unique<Unmoveable>("plant3", 0.3, x1, y));

			else if (mobName.compare("qblock") == 0)
				entities.push_back(std::make_unique<Unmoveable>("qblock", 0.2, x1, y));

			else if (mobName.compare("star") == 0)
				entities.push_back(std::make_unique<Unmoveable>("star", 0.3, x1, y));

			else if (mobName.compare("winning") == 0)
				entities.push_back(std::make_unique<Unmoveable>("winning", 0.05, x1, y));

			else if (mobName.compare("princess_winning") == 0)
				entities.push_back(std::make_unique<Unmoveable>("princess_winning", 0.5, x1, y));

		}
	}

	infile.close();
}*/

void GameFlow::loadFont(sf::Font& font)
{
	try
	{
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
}

void GameFlow::setupText(sf::Text& text, const sf::Font& font, const std::string& string, unsigned int size, const sf::Color& color)
{
	text.setFont(font);
	text.setString(string);
	text.setCharacterSize(size);
	text.setFillColor(color);
}

void GameFlow::setupHorizontalPosition(sf::Text& labelText, sf::Text& valueText, float& x, float y, float padding1, float padding2)
{
	labelText.setPosition(x, y);
	x += labelText.getLocalBounds().width + padding1;
	valueText.setPosition(x, y);
	x += valueText.getLocalBounds().width + padding2;
}


void GameFlow::setupVerticalPosition(sf::Text& labelText, sf::Text& valueText, float x, float &y, float padding1, float padding2)
{
	labelText.setPosition(x, y);
	valueText.setPosition(x + labelText.getLocalBounds().width + padding1, y);
	y += valueText.getLocalBounds().height + padding2;
}

std::string GameFlow::formatTime(const sf::Time& time)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(0) << time.asSeconds();
	return oss.str() + "s";
}

std::string GameFlow::formatValue(int value)
{
	std::ostringstream oss;
	oss << "    " << value;
	return oss.str();
}

void GameFlow::drawText(sf::Text& labelText, sf::Text& valueText)
{
	this->window->draw(labelText);
	this->window->draw(valueText);
}

void GameFlow::handleGameInfo()
{
	sf::Font font;
	loadFont(font);

	sf::Text labelText, timeText, labelCoinText, coinText, labelLivesText, livesText;
	sf::Vector2f viewCenter = UIView.getCenter();
	sf::Vector2f viewSize = UIView.getSize();

	float viewLeft = viewCenter.x - (viewSize.x / 2.0f);
	float viewTop = viewCenter.y - (viewSize.y / 2.0f);
	float padding1 = 20.0f;
	float padding2 = 40.0f;
	
	// Setup texts
	setupText(labelText, font, "TIME:", 25, sf::Color::White);
	setupText(timeText, font, formatTime(pausedTime + clock.getElapsedTime()), 24, sf::Color::White);

	setupText(labelCoinText, font, "COINS:", 25, sf::Color::Yellow);
	setupText(coinText, font, formatValue(coins), 24, sf::Color::Yellow);

	setupText(labelLivesText, font, "LIVES:", 25, sf::Color::Red);
	setupText(livesText, font, formatValue(character->lives), 24, sf::Color::Red);

	
	// Position texts
	float currentX = viewLeft + padding1;
	setupHorizontalPosition(labelLivesText, livesText, currentX, viewTop + padding1, 0, padding2);
	setupHorizontalPosition(labelCoinText, coinText, currentX, viewTop + padding1, 0, padding2);
	setupHorizontalPosition(labelText, timeText, currentX, viewTop + padding1, 30, padding2);
	
	//cout << timeText.getPosition().x << " " << coinText.getPosition().x << " " << livesText.getPosition().x << endl;
	// Draw texts
	window->setView(UIView);
	drawText(labelText, timeText);
	drawText(labelCoinText, coinText);
	drawText(labelLivesText, livesText);
}


void GameFlow::savePausedTimeToFile()
{
	std::ofstream outFile(convertToUnixPath(fs::current_path().string() + "/Resource/asset/txt/pausedTime.txt"));
	if (outFile.is_open())
	{
		outFile << pausedTime.asSeconds() + clock.getElapsedTime().asSeconds();
		outFile.close();
	}
	else
	{
		std::cerr << "Unable to open file for saving paused time." << std::endl;
	}
}

void GameFlow::loadPausedTimeToFile()
{
	std::ifstream inFile(convertToUnixPath(fs::current_path().string() + "/Resource/asset/txt/pausedTime.txt"));
	if (inFile.is_open())
	{
		float seconds;
		inFile >> seconds;
		pausedTime = sf::seconds(seconds);
		inFile.close();
	}
	else
	{
		std::cerr << "Unable to open file for loading paused time. Starting with zero time." << std::endl;
		pausedTime = sf::Time::Zero;
	}
}


void GameFlow::pauseClock()
{
	isPaused = true;
	pausedTime += clock.getElapsedTime();
}

void GameFlow::resumeClock()
{
	isPaused = true;
	pausedTime = sf::Time::Zero;
}

void GameFlow::handleMainMenu() //handles controls in menu
{
	int center = WINDOW_WIDTH / 2;


	mainMenu.drawMainMenu(*window, 620, 350);

	this->window->display();


	while (window->pollEvent(sfEvent))
	{

		handleClosed();

		if (sfEvent.type == sf::Event::KeyPressed)
		{
			mainMenu.handleUpDown(sfEvent);

			if (sfEvent.key.code == sf::Keyboard::Enter)
			{
				if (mainMenu.GetPressedItem() == 0)
				{
					//if (pausedTime == sf::Time::Zero)
					curState = static_cast<int>(GameState::ChooseCharacter);
					//else curState = static_cast<int>(GameState::PlayingGame);
				}

				if (mainMenu.GetPressedItem() == 1)
				{
					this->window->clear();
					curState = static_cast<int>(GameState::ChooseCharacter);
					chooseCharacterMenu.drawChooseCharacter(*window, center);
					this->window->display();
				}

				if (mainMenu.GetPressedItem() == 2)
				{

					curState = static_cast<int>(GameState::Scoreboard);
					mainMenu.drawScoreboard(*window, center);
					this->window->display();

				}

				if (mainMenu.GetPressedItem() == 3)
				{
					curState = static_cast<int>(GameState::HelpMenu);
					mainMenu.drawHelpMenu(*window, center);
					this->window->display();
				}

				if (mainMenu.GetPressedItem() == 4)
				{
					curState = static_cast<int>(GameState::Exit);
					window->close();
					delete this->window;
					exit(0);
				}
			}
		}
	}
}

void GameFlow::handlePlayingGame()
{
	clock.restart();
	if (isRestarted) {
		Restart();
		this->game = new Game(this->map, this->character, this->camera);
		cout << "Restart successfully !" << endl;
		this->game->Begin(*window);
		isRestarted = false;
	}
	if (!game) {
	this->game = new Game(this->map, this->character, this->camera);
		this->game->Begin(*window);
		
	}
	
	while (window->isOpen())
	{
		if (isPaused == true)
		{
			isPaused = false;
			clock.restart();
		}
		while (window->pollEvent(sfEvent))
		{
			handleClosed();

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				/*if (sfEvent.key.code == sf::Keyboard::Left)
				{
					//chooseCharacterMenu.MoveLeft();
				}

				if (sfEvent.key.code == sf::Keyboard::Right)
				{
					//chooseCharacterMenu.MoveRight();
				}

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					
				}*/

				if (sfEvent.key.code == sf::Keyboard::P)
				{
					pauseClock();
					curState = static_cast<int>(GameState::PauseMenu);
					return;
				}

				
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::ChooseLevel);
					return;
				}
			}
		}

		
		// Render
		window->clear();
		float deltaTime = gameClock.restart().asSeconds();
		
		//this->view = game->view;
		//cout << "In Handle Playing Game !" << endl;
		game->Update(deltaTime, *window);
		if (game->win) {
			if (chooseLevel.GetPressedItem() == 1) {
				curState = static_cast <int>(GameState::WinGame);
				return;
			}
			else chooseLevel.setPressedItem(chooseLevel.GetPressedItem() + 1);
			isRestarted = true;
			return;
		}
		else if (game->lose) {
			curState = static_cast <int>(GameState::LooseGame);
			return;
		}
		//cout << "Error in render" << endl;
		game->Render(*renderer, resources);
		coins = game->getCoin();
		handleGameInfo();
		//handleEntity();
		
		
		window->display();
	}
}

void GameFlow::handleEntity()
{
	for (const auto& anEntity : entities)
	{
		anEntity->Update(0.0f);
		anEntity->draw(window, Vector2f(0.0f,0.0f));
	
		//std::cout << "already draw" << '\n';
	}
	//std::cout << entities.size() << '\n';

}

void GameFlow::handleChooseCharacter()
{

	while (true)
	{
		window->clear();
		chooseCharacterMenu.drawChooseCharacter(*window, view.getCenter().x);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				window->close();
				return;
			}

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				chooseCharacterMenu.handleLeftRight(sfEvent);
				

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					curState = static_cast<int>(GameState::ChooseLevel);
					return;
				}

				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu); 
					return;
				}
			}
		}
	}
}

void GameFlow::handleChooseLevel()
{

	while (true)
	{
		window->clear();
		chooseLevel.drawChooseLevel(*window, 230, 241);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				window->close();
				return;
			}

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				chooseLevel.handleUpDown(sfEvent);

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{

					resumeClock();
					isRestarted = true;
					curState = static_cast<int>(GameState::PlayingGame);
					return;
				}

				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::ChooseCharacter);
					return;
				}
			}
		}
	}
}

void GameFlow::handlePauseMenu()
{
	while (true)
	{
		window->clear();
		pauseMenu.drawPauseMenu(*window, 309, 250);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			handleClosed();


			if (sfEvent.type == sf::Event::KeyPressed)
			{
				pauseMenu.handleUpDown(sfEvent);

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					if (pauseMenu.GetPressedItem() == 0) //continue
					{
						curState = static_cast<int>(GameState::PlayingGame);
						return;
					}

					if (pauseMenu.GetPressedItem() == 1) //restart
					{
						curState = static_cast<int>(GameState::AskRestart);
						return;
					}

					if (pauseMenu.GetPressedItem() == 2) //restart
					{
						curState = static_cast<int>(GameState::MainMenu);
						return;
					}
				}

				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::PlayingGame);
					return;
				}
			}
		}
	}
}

void GameFlow::handleAskRestart()
{
	while (true)
	{
		window->clear();
		askRestart.drawAskRestart(*window, 297, 297);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			handleClosed();

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				askRestart.handleLeftRight(sfEvent);

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					if (askRestart.GetPressedItem() == 0) //restart
					{
						resumeClock();
						isRestarted = true;
						curState = static_cast<int>(GameState::PlayingGame);
						return;
					}

					if (askRestart.GetPressedItem() == 1) //continue
					{
						curState = static_cast<int>(GameState::PlayingGame);
						return;
					}
				}

				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::PlayingGame);
					return;
				}
			}
		}
	}
}

void GameFlow::handleScoreboard()
{
	while (true)
	{
		mainMenu.drawScoreboard(*window, WINDOW_WIDTH / 2);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			handleClosed();

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu); 
					return;
				}
			}
		}
	}
}

void GameFlow::handleHelpMenu()
{
	while (true)
	{
		mainMenu.drawHelpMenu(*window, WINDOW_WIDTH / 2);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			handleClosed();

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu); 
					return;
				}
			}
		}
	}
}

void GameFlow::handleLooseGame()
{
	string name = convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/GameOver.png");
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

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition({0,0 });
	
	while (true)
	{
		window->clear();
		window->draw(sprite);
		window->display();

		while (window->pollEvent(sfEvent))
		{
			handleClosed();

			if (sfEvent.type == sf::Event::KeyPressed)
			{
				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					curState = static_cast<int>(GameState::MainMenu);
					return;
				}
			}
		}
	}
}
void GameFlow::handleWinGame()
{
	//Load background image
	sf::Texture texture;
	std::string bgPath = convertToUnixPath(fs::current_path().string() + "/Resource/asset/image/gameWon.png");
	if (!texture.loadFromFile(bgPath))
	{
		std::cerr << "Error: Cannot load background: " << bgPath << std::endl;
		exit(1);
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);

	//Load font
	sf::Font font;
	loadFont(font);

	// Setup text fields
	sf::Text coinLabel, coinInfo, timeLabel, timeInfo, usernameLabel, usernameText, levelLabel, levelInfo;
	
	string levelStr = "";
	int level = chooseLevel.GetPressedItem();
	if (level == 0)
		levelStr = "EASY";
	else if (level == 1)
		levelStr = "MEDIUM";
	else levelStr = "HARD";
	setupText(levelLabel, font, "Level:", 40, sf::Color::Yellow);
	setupText(levelInfo, font, levelStr, 40, sf::Color::White);

	setupText(coinLabel, font, "Coins:", 40, sf::Color::Yellow);
	setupText(coinInfo, font, formatValue(coins), 40, sf::Color::White);

	setupText(timeLabel, font, "Time:", 40, sf::Color::Yellow);
	setupText(timeInfo, font, formatTime(pausedTime + clock.getElapsedTime()), 40, sf::Color::White);

	setupText(usernameLabel, font, "Username:", 40, sf::Color::Yellow);
	setupText(usernameText, font, "", 30, sf::Color::Black);

	//Setup input box
	sf::RectangleShape inputBox(sf::Vector2f(300, 50));
	inputBox.setFillColor(sf::Color::White);
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(sf::Color::Black);

	//Set Position
	float centerX = WINDOW_WIDTH / 2 - 300;
	float startY = WINDOW_HEIGHT / 2 - 50;
	float padding1 = 200;
	float padding2 = 50;

	setupVerticalPosition(levelLabel, levelInfo, centerX, startY, padding1 + 10, padding2);
	setupVerticalPosition(coinLabel, coinInfo, centerX, startY, padding1, padding2);
	setupVerticalPosition(timeLabel, timeInfo, centerX, startY, padding1 + 40, padding2);
	setupVerticalPosition(usernameLabel, usernameText, centerX, startY, padding1, padding2);

	timeInfo.setPosition({ coinInfo.getPosition().x + 50,timeInfo.getPosition().y });
	levelInfo.setPosition({ coinInfo.getPosition().x + 50 - levelInfo.getLocalBounds().width/2,levelInfo.getPosition().y});
	
	//Position inputbox
	inputBox.setPosition(usernameLabel.getPosition().x + usernameLabel.getLocalBounds().width + 20, usernameLabel.getPosition().y);
	usernameText.setPosition(inputBox.getPosition().x + 20, inputBox.getPosition().y + 5);

	//Handle username input
	std::string username = "";
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				return;
			}

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8 && !username.empty()) //Backspace
				{
					username.pop_back();
				}
				else if (event.text.unicode < 128 && event.text.unicode >= 32 && username.size() < 10) //Normal characters
				{
					username += static_cast<char>(event.text.unicode);
				}
				usernameText.setString(username);
			}

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			{
				//saveScoreboard(username, coins, pausedTime, character->lives);
				curState = static_cast<int>(GameState::MainMenu);
				return;
			}
		}

		//Render
		window->clear();
		window->draw(sprite);
		window->draw(levelLabel);
		window->draw(levelInfo);
		window->draw(coinLabel);
		window->draw(coinInfo);
		window->draw(timeLabel);
		window->draw(timeInfo);
		window->draw(usernameLabel);
		window->draw(inputBox);
		window->draw(usernameText);
		window->display();
	}
}

void GameFlow::handleClosed()
{
	if (sfEvent.type == sf::Event::Closed)
	{
		window->close();
		return;
	}
}

void handleExit(sf::Event sfEvent, int& curState, GameFlow::GameState& newState)
{
	if (sfEvent.key.code == sf::Keyboard::Escape)
	{
		curState = static_cast<int>(newState);
		return;
	}
}

void GameFlow::run()
{
	while (window->isOpen())
	{
		switch (static_cast<GameState>(curState))
		{

		case GameState::MainMenu:
			handleMainMenu();
			break;

		case GameState::ChooseCharacter:
			handleChooseCharacter();
			break;

		case GameState::Scoreboard:
			handleScoreboard();
			break;

		case GameState::HelpMenu:
			handleHelpMenu();
			break;

		case GameState::Exit:
			window->close();
			break;

		case GameState::ChooseLevel:
			handleChooseLevel();
			break;

		case GameState::PauseMenu:
			handlePauseMenu();
			break;

		case GameState::AskRestart:
			handleAskRestart();
			break;

		case GameState::PlayingGame:
			handlePlayingGame();
			break;

		case GameState::LooseGame:
			handleLooseGame();
			break;
		
		case GameState::WinGame:
			handleWinGame();
			break;

		}
	}
}

void Menu::MoveUp() //keyboard input UP in menu
{
	texts[selectedItemIndex].setFillColor(sf::Color::White);
	if (selectedItemIndex - 1 >= 0)
		selectedItemIndex--;
	else selectedItemIndex = texts.size() - 1;

	sf::Color color(248, 233, 67);
	texts[selectedItemIndex].setFillColor(color);
}

void Menu::MoveDown() //keyboard input DOWN in menu
{
	texts[selectedItemIndex].setFillColor(sf::Color::White);
	if (selectedItemIndex + 1 < texts.size())
		selectedItemIndex++;
	else
		selectedItemIndex = 0;

	sf::Color color(248, 233, 67);
	texts[selectedItemIndex].setFillColor(color);
}

void Menu::handleUpDown(sf::Event sfEvent)
{
	if (sfEvent.key.code == sf::Keyboard::Up)
		MoveUp();
	else if (sfEvent.key.code == sf::Keyboard::Down)
		MoveDown();

}

void Menu::handleLeftRight(sf::Event sfEvent)
{
	if (sfEvent.key.code == sf::Keyboard::Left)
		MoveLeft();
	else if (sfEvent.key.code == sf::Keyboard::Right)
		MoveRight();
}

void Menu::MoveLeft()
{
	MoveUp();
}

void Menu::MoveRight()
{
	MoveDown();
}

int Menu::GetPressedItem() //what is pressed in menu
{
	return selectedItemIndex;
}

void Menu::setPressedItem(int item)
{
	selectedItemIndex = item;
}

std::vector<sf::Texture> GameFlow::loadFrame(std::string folderPath)
{
	std::vector<sf::Texture> textures;

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			sf::Texture texture;
			if (texture.loadFromFile(entry.path().string()))
			{
				textures.push_back(texture);
			}
			else
			{
				std::cerr << "Failed to load: " << entry.path().string() << std::endl;
			}
		}
	}

	return textures;
}


void GameFlow::Restart() {
	// Delete and recreate Map, Character, and Camera
	if (map) {
		delete map;
		map = nullptr;
		cout << "GameFlow :: Map deleted" << endl;
	}
	if (character) {
		delete character;
		character = nullptr;
		cout << "GameFlow :: Character deleted" << endl;
	}
	if (camera) {
		delete camera;
		camera = nullptr;
		cout << "GameFlow :: Camera deleted" << endl;
	}
	if (game) {
		delete game;
		game = nullptr;
		cout << "GameFlow :: Game deleted in gameFlow" << endl;
	}
	
	map = new Map(1.0f, chooseLevel.GetPressedItem());
	if (chooseCharacterMenu.GetPressedItem() == 1)
		character = CharacterFactory::createCharacter(LUIGI);
	else character = CharacterFactory::createCharacter(MARIO);

	camera = new Camera(30.0f);
	isRestarted = true;
	coins = 0;
}

void GameFlow::Start() {
	this->game = new Game(this->map, this->character, this->camera);
	this->game->Begin(*window);
}
