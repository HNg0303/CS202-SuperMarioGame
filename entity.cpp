#include "entity.h"
#include "Resources.h"

vector<Entity*> onEntities;

Entity::Entity(string name_i, double frameDuration_i, float x, float y, Vector2f coords) :
	name(name_i), frameDuration(frameDuration_i), currentFrame(0), coords(coords)
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

Vector2f Entity::getCoords() {
	return coords;
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
		Vector2f scale;
		if (name == "goal") scale = Vector2f(this->size.x / frames[currentFrame].getSize().x, this->size.y * 5.0f / frames[currentFrame].getSize().y);
		else scale = Vector2f(this->size.x / frames[currentFrame].getSize().x, this->size.y / frames[currentFrame].getSize().y);
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
		}
		destroyingTimer += deltaTime;
		if (destroyingTimer >= 1.5f)
			deleteEntity(this);
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
	//cout << "Im moving !!" << endl;
	//position.x += speed * direction;
	b2Vec2 velocity = body->GetLinearVelocity();
	if (!y_direction) {
		velocity.x += speed * x_direction;
		velocity.y = 0.0f;
	}
	if (!x_direction) {
		velocity.y += speed * y_direction;
		velocity.x = 0.0f;
	}
	//velocity.y += speed * y_direction;
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
	bool outOfBoundX = body->GetPosition().x <= xBound.first || body->GetPosition().x >= xBound.second;
	bool outOfBoundY = body->GetPosition().y >= yBound.first || body->GetPosition().y <= yBound.second;
	b2Vec2 velocity = body->GetLinearVelocity();
	if (outOfBoundX) {
		x_direction = -x_direction;
	}
	if (outOfBoundY) {
		y_direction = -y_direction;
	}
}



void Unmoveable::Update(float deltaTime)
{
	if (deleted) {
		deleteEntity(this);
		return;
	}
	if (!body) Begin();
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
	fixtureData->entity = this;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

Block::~Block() {
	/*for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto* data = reinterpret_cast<FixtureData*>(fixture->GetUserData().pointer);
		delete data;  // Free the FixtureData memory
	}*/
	delete fixtureData;
	fixtureData = nullptr;
	Physics::world.DestroyBody(body);
	body = nullptr;
}


void PowerUp::Begin() {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
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
	//fixtureDef.isSensor = true;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;

	body->CreateFixture(&fixtureDef);
}



PowerUp :: ~PowerUp() {
	delete fixtureData;
	fixtureData = nullptr;
	Physics::world.DestroyBody(body);
	body = nullptr;
	cout << "Successfully Destroyed Level Up !" << endl;
}

void Enemy::Begin() {
	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->type = FixtureDataType::Enemy;
	fixtureData->listener = this;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circle;
	circle.m_p.Set(0.0f, 0.2f);
	circle.m_radius = 0.3f;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &circle;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	fixture = body->CreateFixture(&fixtureDef);
}

void Enemy::OnBeginContact(b2Fixture* self, b2Fixture* other) {
	if (!self) {
		std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
		return;  // Exit the function if fixture is invalid
	}
	FixtureData* otherData = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
	FixtureData* selfData = reinterpret_cast<FixtureData*> (self->GetUserData().pointer);
	if (!otherData) return;
	if (fixture == self) {
		if (otherData->type == FixtureDataType::Enemy || otherData->type == FixtureDataType::MapTile) {
			x_direction = -x_direction;
			//y_direction = -y_direction;
		}
	}
}

Enemy :: ~Enemy() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	delete fixtureData;
	fixtureData = nullptr;
	cout << "Successfully Destroyed Enemy !" << endl;
}

void Enemy::Die() {
	isDead = true;
	deleted = true;
	size.y = size.y / 5.0f;
	position.y += size.y / 5.0f;
}

void Flame::Begin() {
	fixtureData = new FixtureData();
	//fixtureData->entity = this;
	fixtureData->listener = this;
	fixtureData->type = FixtureDataType::Entity;



	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	if (faceLeft) {
		x_direction = -x_direction;
		bodyDef.position.Set(position.x + 148.0f, position.y);
	}
	else bodyDef.position.Set(position.x + 152.0f, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 0.0f;
	fixture = body->CreateFixture(&fixtureDef);
}

void Flame::OnBeginContact(b2Fixture* self, b2Fixture* other) {
	if (!self) {
		std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
		return;  // Exit the function if fixture is invalid
	}
	FixtureData* otherData = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
	FixtureData* selfData = reinterpret_cast<FixtureData*> (self->GetUserData().pointer);
	if (!otherData) return;
	if (fixture == self) {
		if (otherData->type == FixtureDataType::Enemy) {
			Enemy* enemy = dynamic_cast<Enemy*> (otherData->entity);
			if (enemy) {
				enemy->Die();
				cout << "Kill " << enemy->getName() << endl;
			}
		}
		else if (otherData->type == FixtureDataType::MapTile || otherData->type == FixtureDataType::Character) {
			isDead = true;
			deleted = true;
			cout << "Your flame is extinguished !" << endl;
		}
	}
}

Flame :: ~Flame() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	delete fixtureData;
	fixtureData = nullptr;
	cout << "Successfully Destroyed Flame !" << endl;
}

void deleteEntity(Entity* entity) {
	const auto& it = find(onEntities.begin(), onEntities.end(), entity);
	if (it != onEntities.end()) {
		onEntities.erase(it);
		delete entity;
	}
}

void clearEntities() {
	for (Entity* entity : onEntities) {
		delete entity;
		entity = nullptr;
	}
	onEntities.clear();
}

void Elevator::Begin()
{
	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->type = FixtureDataType::Enemy;


	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2, size.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);
}

Elevator:: ~Elevator() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	delete fixtureData;
	fixtureData = nullptr;
	cout << "Successfully Destroyed Enemy !" << endl;
}

void DeathBlock::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
	if (!self) {
		std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
		return;  // Exit the function if fixture is invalid
	}
	FixtureData* otherData = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
	FixtureData* selfData = reinterpret_cast<FixtureData*> (self->GetUserData().pointer);
	if (!otherData) return;
	if (fixture == self) {
		if (otherData->type == FixtureDataType::Enemy || otherData->type == FixtureDataType::MapTile) {
			x_direction = -x_direction;
			//y_direction = -y_direction;
		}
	}
}

void DeathBlock::Begin() {
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
	fixtureData->entity = this;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}
