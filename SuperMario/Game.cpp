#include "Game.h"

vector<Entity*>Game::onEntities;

Game::Game(Map& map, Character* character, Camera& camera) : map(map), character(character), camera(camera) {
};

Game :: ~Game() {
	if (instance != nullptr)
		delete instance;
	instance = nullptr;
}

Game* Game::getInstance(Map& map, Character* character, Camera& camera) {
	if (instance == nullptr) {
		instance = new Game(map, character, camera);
	}
	return instance;
}

Entity* Game::getEntity(string name) {
	for (auto& entity : entities) {
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

void Game::loadEntities(vector<unique_ptr<Entity>>& entities) {
	for (auto& entity : entities) {
		this->entities.push_back(entity.get());
	}
}

void Game :: Begin(sf::RenderWindow& window)
{
	//Generate map
	//map.CreateCheckerBoard(4, 2);
	
	Physics::Init();

	sf::Image map_image;
	string mapPath = convertToUnixPath(fs::current_path().string()) + "/Resource/map2ColorProcessed.png";
	map_image.loadFromFile(mapPath);
	character->position = map.CreateFromImage(map_image, onEntities);
	character->Begin();
	for (auto& entity : onEntities)
		entity->Begin();
	window.setView(camera.GetView(window.getSize()));
}

void Game :: Update(float& deltaTime, RenderWindow& window) {
	Physics::Update(deltaTime);
	character->Update(deltaTime);
	camera.position = character->getPos();
	for (auto& entity : onEntities)
		entity->update();
	window.setView(camera.GetView(window.getSize()));
}

void Game :: Render(Renderer& renderer, Resources& resource) {
	//map.Draw(renderer, resource);
	for (auto& entity : onEntities)
		entity->draw(renderer.getRenderWindow(), Vector2f(map.cellSize, map.cellSize));
	character->Draw(renderer, 0, resource);
	//Physics::draw(renderer);
}

/*void Game::deleteEntities(Entity* entity) {
	const auto& it = find(onEntities.begin(), onEntities.end(), entity);
	if (it != onEntities.end())
		onEntities.erase(it);
}*/