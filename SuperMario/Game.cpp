#include "Game.h"

Music music{};

Game::Game(Map* map, Character* character, Camera* camera) : map(map), character(character), camera(camera) {
	InitMapPath();
}

Game :: ~Game() {
	//map = nullptr;
	//character = nullptr;
	//camera = nullptr;
	cout << "Successfully delete game !" << endl;
}
/*
Game* Game::getInstance(Map* map, Character* character, Camera* camera, bool isRestarted) {
	if (isRestarted) {
		delete instance;
		instance = nullptr;
	}
	if (instance == nullptr) {
		instance = new Game(map, character, camera);
		cout << "Initializing new Instance of the main game !!!!!!" << endl;
	}
	return instance;
}*/

Entity* Game::getEntity(string name) {
	for (auto& entity : onEntities) {
		if (entity->getName() == name) return entity;
	}
	return nullptr;
}

void Game::chooseCharacter(CharacterType type) {
	if (type == LUIGI)
		this->character = CharacterFactory::createCharacter(LUIGI);
	else if (type == MARIO)
		this->character = CharacterFactory::createCharacter(MARIO);
}

void Game :: InitMapPath() {
	for (int i = 1; i <= 3; i++) {
		vector<string> tmp;
		for (int j = 1; j <= 3; j++) {
			string mapPath = "/Resource/Map/map" + to_string(i) + "-" + to_string(j) + ".png"; //Ex: /Resource/Map/map1-1.png
			tmp.push_back(mapPath);
		}
		mapPaths.push_back(tmp);
	}
}

void Game :: Begin(sf::RenderWindow& window)
{
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	
	Physics::Init();
	//string mapPath = convertToUnixPath(fs::current_path().string()) + mapPaths[map->getIndex()];
	string mapPath = convertToUnixPath(fs::current_path().string()) + mapPaths[map->difficulty][map->getIndex()];

	map_image.loadFromFile(mapPath);
	startPos = map->CreateFromImage(map_image, onEntities);
	character->setPos(startPos);
	character->yBound = map->cellSize * map->grid[0].size();
	character->xBound = pair<float, float>(map->cellSize, map->cellSize * map->grid.size());
	character->Begin();
	for (auto& entity : onEntities)
		entity->Begin();
	view = camera->GetView(window.getSize());
	window.setView(view);
	cout << "Initialize game successfully !!!!" << endl;
	music.openFromFile(convertToUnixPath(fs::current_path().string() + "/Resource/Music/music.wav"));
	music.setLoop(true);
	music.setVolume(25);
	music.play();
}

void Game :: Update(float& deltaTime, RenderWindow& window) {
	Physics::Update(deltaTime);
	//cout << "Physic Update is okay" << endl;
	if (character->win) {
		this->win = true;
		return;
	}
	if (character->isDead) {
		character->Update(deltaTime);
		if (!character->lives) {
			this->lose = true;
			cout << "You Lose !" << endl;
			return;
		}
		camera->position = character->getPos();
		view = camera->GetView(window.getSize());
		window.setView(view);
		return;
	}
	//cout << "Check Character dead is Okay !" << endl;
	character->Update(deltaTime);
	//cout << "Character Update is okay" << endl;
	camera->position = character->getPos();
	//cout << "Camera Update is okay" << endl;
	map->Update(character->getPos());
	//cout << "Map Update is okay" << endl;
	for (auto& entity : onEntities)
		entity->Update(deltaTime);
	view = camera->GetView(window.getSize());
	window.setView(view);
}

void Game :: Render(Renderer& renderer, Resources& resource) {
	//map->Draw(renderer, resource);
	map->drawBackground(renderer, 2);
	for (auto& entity : onEntities)
		entity->draw(renderer.getRenderWindow(), Vector2f(map->cellSize, map->cellSize));
	character->Draw(renderer);
	Physics::draw(renderer);
}

int Game::getCoin() {
	return this->character->coin;
}

/*void Game::deleteEntities(Entity* entity) {
	const auto& it = find(onEntities.begin(), onEntities.end(), entity);
	if (it != onEntities.end())
		onEntities.erase(it);
}*/