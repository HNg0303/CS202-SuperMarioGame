#include "Game.h"

//Music music{};

Game::Game(Map* map, Character* character, Camera* camera) : map(map), character(character), camera(camera) {};

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
/*
void Game::loadEntities(vector<unique_ptr<Entity>>& entities) {
	for (auto& entity : entities) {
		this->entities.push_back(entity.get());
	}
}*/

void Game :: Begin(sf::RenderWindow& window)
{
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	
	Physics::Init();

	sf::Image map_image;
	string mapPath = convertToUnixPath(fs::current_path().string()) + "/Resource/map2ColorProcessed.png";
	map_image.loadFromFile(mapPath);
	character->position = map->CreateFromImage(map_image, onEntities);
	character->Begin();
	for (auto& entity : onEntities)
		entity->Begin();
	view = camera->GetView(window.getSize());
	window.setView(view);
	cout << "Initialize game successfully !!!!" << endl;
	/*music.openFromFile(convertToUnixPath(fs::current_path().string() + "/Resource/Music/music.wav"));
	music.setLoop(true);
	music.setVolume(30);
	music.play();*/
}

void Game :: Update(float& deltaTime, RenderWindow& window) {
	Physics::Update(deltaTime);
	character->Update(deltaTime);
	camera->position = character->getPos();
	map->Update();
	for (auto& entity : onEntities)
		entity->Update(deltaTime);
	view = camera->GetView(window.getSize());
	window.setView(view);
}

void Game :: Render(Renderer& renderer, Resources& resource) {
	//map.Draw(renderer, resource);
	for (auto& entity : onEntities)
		entity->draw(renderer.getRenderWindow(), Vector2f(map->cellSize, map->cellSize));
	character->Draw(renderer, resource);
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