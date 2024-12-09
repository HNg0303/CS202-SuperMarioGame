#include "Game.h"

Game::~Game()
{
	delete this->window;
}

void Game::loadMobs()
{
	std::string line;
	float x, y;
	std::fstream infile;
	std::string mobName;
	Entity* wsk;

	mobs.clear();

	infile.open("assets/txt/mobs.txt");
	if (!infile) {
		std::cout << "can not open file to read results from";
	}

	while (getline(infile, line)) {
		{
			std::stringstream ss(line);
			ss >> mobName;
			std::string temp;
			ss >> x;
			ss >> y;

			/*if (mobName.compare("Turtle") == 0)
				wsk = new Goombas;

			else if (mobName.compare("Spikey") == 0)
				wsk = new Koopas;

			/*else if (mobName.compare("FlyTur") == 0)
				wsk = new FlyTur;

			else if (mobName.compare("Plant1") == 0)
				wsk = new Plant1;

			else if (mobName.compare("Plant2") == 0)
				wsk = new Plant2;

			else
				wsk = nullptr; // if no mobName read from exist take Abysss*/

			/*if (wsk != nullptr)
			{
				wsk->setPosition({ x, y });
				mobs.push_back(*wsk);
			}*/
			//This line of code adds a copy of the object pointed to by wsk to the mobs vector. 
		//The push_back function is a method available for vectors, and it adds a new element to the end of the vector.
		}
	}

	infile.close();

	//repairSFMLTextures();
}

void Game::drawMobs() //draws enemies
{
	/*for (int i = 0; i < mobs.size(); i++)
	{
		//std::cout << "yess";
		if (mobs[i].getIsAlive())
		{
			//std::cout << "yess";
			//if (movingSide == LEFT)
				mobs.at(i).MovingDirectiongLeft();
				mobs.at(i).update();
				//else if (movingSide == RIGHT)
				//mobs.at(i).MovingDirectiongRight();

			mobs.at(i).update();
			mobs[i].setPosition(sf::Vector2f(100*i,200));
			window->draw(mobs[i]);
		}
	}*/
	//mobs.at(0).moveLeft();
	mobs.at(0).update();
	window->draw(mobs[0]);
}

void Game::handleMainMenu() //handles controls in menu
{
	int center = WINDOW_WIDTH / 2;


	mainMenu.drawMainMenu(*window, 620, 350);

	this->window->display();


	while (window->pollEvent(sfEvent))
	{
		if (sfEvent.type == sf::Event::Closed)
			window->close();

		if (sfEvent.type == sf::Event::KeyPressed)
		{
			mainMenu.handleUpDown(sfEvent);

			if (sfEvent.key.code == sf::Keyboard::Enter)
			{
				if (mainMenu.GetPressedItem() == 0)
				{
					

				}

				if (mainMenu.GetPressedItem() == 1)
				{
					this->window->clear();
					curState = 1;
					chooseCharacterMenu.drawChooseCharacter(*window, center);
					this->window->display();
				}
				
				if (mainMenu.GetPressedItem() == 2)
				{
					curState = 2;
					mainMenu.drawScoreboard(*window, center);
					this->window->display();

				}

				if (mainMenu.GetPressedItem() == 3)
				{
					curState = 3;
					mainMenu.drawHelpMenu(*window, center);
					this->window->display();
				}

				if (mainMenu.GetPressedItem() == 4)
				{
					curState = 4;
					window->close();
					delete this->window;
					exit(0);
				}
			}
		}
	}
}


void Game::handlePlayingGame()
{
	int n = 2;
	std::vector<size_t> currentFrames(11, 0);
	std::vector<std::vector<sf::Texture>> textures(11);
	for (int i = 0; i < 2; ++i)
	{
		textures[i] = loadFrame("assets/frame/" + entityName[i]);
		if (textures[i].empty())
		{
			std::cerr << "No frames loaded for entity " << i << std::endl;
			return;
		}
	}
	std::vector<sf::Sprite> sprites(11);
	std::pair<float, float> X[2];

	for (int i = 0; i < 2; ++i)
	{
		sprites[i].setScale(2.0f, 2.0f);
		sprites[i].setPosition(100, i * 200.0f); // Set positions for each sprite
		X[i].first = 100;
		X[i].second = 500;
	}

	sf::Clock clock;
	sf::Clock clock2;
	float frameDuration = 0.4f; // Time per frame in seconds
	float speed = 100.f; // pixel per second
	float direction = 1.f;

	//each enity
	//name
	//frameDuration: fixed
	// (name, velocity, x, y): move -> range of movement(x,y)
	// (name, x, y): do not move, coordinate(x,y)

	while (window->isOpen())
	{
		
		while (window->pollEvent(sfEvent))
		{
			if (sfEvent.type == sf::Event::Closed)
			{
				window->close();
				return;
			}

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
					curState = static_cast<int>(GameState::PauseMenu);
					return;
				}

				//handleExit(sfEvent, curState, GameState::MainMenu);
				
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu);
					return;
				}
			}
		}

		if (clock.getElapsedTime().asSeconds() > frameDuration)
		{
			std::cout << "come HERE";
			for (int i = 0; i < 2; ++i)
			{
				currentFrames[i] = (currentFrames[i] + 1) % textures[i].size();
				sprites[i].setTexture(textures[i][currentFrames[i]]);
			}
			clock.restart();
		}

		float deltaTime = clock2.restart().asSeconds();

		for (int i = 0; i < 2; ++i)
		{
			sf::Vector2f position = sprites[i].getPosition();
			position.x += direction * speed * deltaTime;

			// Đổi hướng nếu vượt phạm vi
			if (position.x <= X[i].first)
			{
				position.x = X[i].first; // Giữ trong phạm vi
				direction = 1.f; // Chuyển hướng sang phải
			}
			else if (position.x >= X[i].second)
			{
				position.x = X[i].second; // Giữ trong phạm vi
				direction = -1.f; // Chuyển hướng sang trái
			}

			sprites[i].setPosition(position);
		}
		// Render
		window->clear();
		for (const auto& sprite : sprites)
		{
			window->draw(sprite);
			std::cout << "already draw";
		}
		window->display();
	}
}

void Game::handleChooseCharacter()
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

void Game::handleChooseLevel()
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

void Game::handlePauseMenu()
{
	while (true)
	{
		window->clear();
		pauseMenu.drawPauseMenu(*window, 309, 250);
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

void Game::handleAskRestart()
{
	while (true)
	{
		window->clear();
		askRestart.drawAskRestart(*window, 297, 297);
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
				askRestart.handleLeftRight(sfEvent);

				if (sfEvent.key.code == sf::Keyboard::Enter)
				{
					if (askRestart.GetPressedItem() == 1) //continue
					{
						curState = static_cast<int>(GameState::PlayingGame);
						return;
					}

					if (askRestart.GetPressedItem() == 0) //restart
					{


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

void Game::handleScoreboard()
{
	while (true)
	{
		mainMenu.drawScoreboard(*window, WINDOW_WIDTH / 2);
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
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu); 
					return;
				}
			}
		}
	}
}


void Game::handleHelpMenu()
{
	while (true)
	{
		mainMenu.drawHelpMenu(*window, WINDOW_WIDTH / 2);
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
				if (sfEvent.key.code == sf::Keyboard::Escape)
				{
					curState = static_cast<int>(GameState::MainMenu); 
					return;
				}
			}
		}
	}
}

void Game::run()
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


void handleExit(sf::Event sfEvent, int &curState, Game::GameState &newState)
{
	if (sfEvent.key.code == sf::Keyboard::Escape)
	{
		curState = static_cast<int>(newState);
		return;
	}
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


std::vector<sf::Texture> Game::loadFrame(std::string folderPath)
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