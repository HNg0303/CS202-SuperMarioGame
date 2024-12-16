#include "entity.h"
#include "Resources.h"

vector<Entity*> onEntities;

Entity :: Entity(string name_i, double frameDuration_i, float x, float y) :
	name(name_i), frameDuration(frameDuration_i), currentFrame(0), direction(1.0)
{
	position.x = x;
	position.y = y;
	frames = loadFrame(convertToUnixPath("Resource/asset/frame/" + name));
	if (frames.empty())
	{
		std::cerr << "No frames loaded for entity " << name << std::endl;
		return;
	}
}

string Entity::getName() {
	return name;
}

std::vector<sf::Texture> Entity::loadFrame(std::string folderPath)
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
				std::cerr << "Successful to load : " << entry.path().string() << std::endl;
			}
			else
			{
				std::cerr << "Failed to load: " << entry.path().string() << std::endl;
			}
		}
	}

	return textures;
}

void Entity::draw(sf::RenderWindow* window, const Vector2f& size)
{
	//if (deleted) return;
	if (!frames.empty())
	{
		sf::Sprite sprite;
		sprite.setTexture(frames[currentFrame], true);
		sf::Vector2f origin(frames[currentFrame].getSize().x / 2.0f, frames[currentFrame].getSize().y / 2.0f);
		sprite.setOrigin(origin);
		sf::Vector2f scale(this->size.x / frames[currentFrame].getSize().x, this->size.y / frames[currentFrame].getSize().y);
		sprite.setScale(scale);
		//sprite.setScale(2.f * direction, 2.f);
		sprite.setPosition(position);
		window->draw(sprite);
	}
}

void Entity::markDeleted() {
	if (deleted == false) {
		deleted = true;
	}
}


void Moveable::Update(float deltaTime) 
{
	if (isDead) {
		if (body) {
			Physics::world.DestroyBody(this->body);
			this->body = nullptr;
			destroyingTimer += deltaTime;
			if (destroyingTimer >= 3.0f) 
				deleteEntity(this);
		}
		return;
	}
	if (body) {
		//std::cout << position.x << " " << position.y;
		if (clock.getElapsedTime().asSeconds() > frameDuration)
		{
			currentFrame = (currentFrame + 1) % frames.size();
			//std::cout << "check frame " << currentFrame << " " << frames.size() << '\n';
			clock.restart();
		}
		move();
		const b2Vec2& bodyPos = body->GetPosition();
		position.x = bodyPos.x;
		position.y = bodyPos.y;
	}
	//cout << "x: " << position.x << ", y:" << position.y << endl;
 }

void Moveable::move()
{
	//position.x += speed * direction;
	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x += speed * direction;
	body->SetLinearVelocity(velocity);  // Update the body's velocity
	checkAndChangeDirection();
}

void Moveable::checkAndChangeDirection()
{
	/*
	if (position.x <= xBound.first || position.x >= xBound.second)
	{
		direction = -direction; // Reverse direction
	}*/
	//const b2Vec2& currentPos = body->GetPosition();

	// Reverse direction if bounds are exceeded
	if (body->GetPosition().x <= xBound.first || body->GetPosition().x >= xBound.second) {
		direction = -direction;

		// Adjust velocity to match the new direction
		//body->SetLinearVelocity(b2Vec2(speed * direction, 0.0f));
	}
}



void Unmoveable::Update(float deltaTime)
{
	if (deleted) {
		deleteEntity(this);
	}
	if (body) {
		//std::cout << "unmoveable" ;
		//std::cout << position.x << " " << position.y;
		if (clock.getElapsedTime().asSeconds() > frameDuration)
		{
			currentFrame = (currentFrame + 1) % frames.size();
			clock.restart();
			//std::cout << "check frame " << currentFrame << " " << frames.size() << '\n';
		}
	}
}

void Coin::Begin() {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x, position.y);

	body = Physics::world.CreateBody(&bodyDef);

	b2PolygonShape shape{};
	shape.SetAsBox(0.4f, 0.4f);

	fixtureData = new FixtureData();
	fixtureData->type = FixtureDataType::Entity;
	fixtureData->entity = this;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.isSensor = true;
	fixtureDef.density = 0.0f;

	body->CreateFixture(&fixtureDef);
}


Coin :: ~Coin() {
	delete fixtureData;
	fixtureData = nullptr;
	Physics::world.DestroyBody(body);
	body = nullptr;
	cout << "Successfully Destroyed Coin !" << endl;
}

void Block::Begin() {
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x, position.y);
	body = Physics::world.CreateBody(&bodyDef);
	//Set Shape
	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);//Set at the center
	//Set Fixture Data for handle collision
	fixtureData = new FixtureData();
	fixtureData->type = FixtureDataType::MapTile;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

Block::~Block(){
	/*for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto* data = reinterpret_cast<FixtureData*>(fixture->GetUserData().pointer);
		delete data;  // Free the FixtureData memory
	}*/
	delete fixtureData;
	fixtureData = nullptr;
	Physics::world.DestroyBody(body);
	body = nullptr;
}

void Enemy::Begin() {
	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->type = FixtureDataType::Enemy;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_p.Set(0.0f, 0.2f);
	circle.m_radius = 0.4f;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &circle;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);
}

Enemy :: ~Enemy() {
	delete fixtureData;
	fixtureData = nullptr;
	cout << "Successfully Destroyed Enemy !" << endl;
}

void Enemy::Die() {
	isDead = true;
	size.y = size.y / 5.0f;
	position.y += size.y / 5.0f;
}
void deleteEntity(Entity* entity) {
	const auto& it = find(onEntities.begin(), onEntities.end(), entity);
	if (it != onEntities.end()) {
		onEntities.erase(it);
		delete entity;
	}
}