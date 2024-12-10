#include "Game.h"

Game::~Game()
{
	delete this->window;
}

void Game::loadMobs()
{
	std::string line;
	float x1, x2, y;
	std::fstream infile;
	std::string mobName;

	entities.clear();
	//std::string entityName[11] = { "cheep","coin","flame","goombas","koopas","levelUp","plant1","plant2","plant3","qblock","star" };

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

			std::cout << mobName << '\n';

			if (mobName.compare("goombas") == 0)
				entities.push_back(std::make_unique<Moveable>("goombas", 0.5, 0.05f, x1, x2, y));

			else if (mobName.compare("koopas") == 0)
				entities.push_back(std::make_unique<Moveable>("koopas", 0.5, 0.05f, x1, x2, y));

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

		
		// Render
		window->clear();
		//std::cout << entities.size() << '\n';
		for (const auto& anEntity: entities)
		{
			
			anEntity->update();
			anEntity->draw(*window);
			//std::cout << "already draw" << '\n';
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