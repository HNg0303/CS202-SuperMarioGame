#include "entity.h"
#include "Resources.h"

vector<Entity*> onEntities;
int Entity::curTheme = 1;
Sound soundFX{};


Entity::Entity(string name_i, double frameDuration_i, float x, float y, Vector2f coords) :
	name(name_i), frameDuration(frameDuration_i), currentFrame(0), coords(coords)
{
	position.x = x;
	position.y = y;
	//cout << Entity::curTheme << endl;
	//cout << "Resource/asset/frame/frame" + to_string(Entity::curTheme) + "/" + name << endl;
	frames = loadFrame(convertToUnixPath("Resource/asset/frame/frame" + to_string(Entity::curTheme) + "/" + name));
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
		scale = Vector2f(this->size.x / frames[currentFrame].getSize().x, this->size.y / frames[currentFrame].getSize().y);
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
			if (this->getName() == "bowser")
			{
				soundFX.setBuffer(Resources::sfx["bowserfall.wav"]);
				soundFX.setVolume(30);
				soundFX.play();
			}
			Physics::world.DestroyBody(this->body);
			this->body = nullptr;
		}
		destroyingTimer += deltaTime;
		if (destroyingTimer >= 1.5f) {
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
		move(deltaTime);
		const b2Vec2& bodyPos = body->GetPosition();
		position.x = bodyPos.x;
		position.y = bodyPos.y;
		//cout << "MyBound: " << xBound.first << ", " << xBound.second << endl;
		//cout << "Position:" << position.x << ", " << position.y << endl;
	}
	//cout << "x: " << position.x << ", y:" << position.y << endl;
}

void Moveable::move(float deltaTime)
{
	//cout << "Im moving !!" << endl;
	//position.x += speed * direction;

	b2Vec2 velocity = body->GetLinearVelocity();
	if (abs(velocity.x) <= 0.02f) {
		x_direction = -1.0f * x_direction;
	}
	/*
	if (abs(velocity.y) <= 0.02f) {
		y_direction = -1.0f * y_direction;
	}*/
	velocity.x = speed * x_direction;
	velocity.y = speed * y_direction;
	body->SetLinearVelocity(velocity);  // Update the body's velocity
	checkAndChangeDirection();
}

void Moveable::checkAndChangeDirection()
{
	if (body->GetPosition().x <= xBound.first && x_direction != 0.0f) {
		x_direction = 1.0f;
		//velocity.x = 0.0f;
	}
	else if (body->GetPosition().x >= xBound.second && x_direction != 0.0f) {
		x_direction = -1.0f;
		//velocity.x = 0.0f;
	}
	if (body->GetPosition().y >= yBound.first && y_direction != 0.0f) {
		cout << "Go Up" << endl;
		y_direction = -1.0f;
		//velocity.x = 0.0f;
	}
	else if (body->GetPosition().y <= yBound.second && y_direction != 0.0f) {
		cout << "Go Down" << endl;
		y_direction = 1.0f;
		//velocity.x = 0.0f;
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
		position = Vector2f(body->GetPosition().x, body->GetPosition().y);
		if (clock.getElapsedTime().asSeconds() > frameDuration)
		{
			currentFrame = (currentFrame + 1) % frames.size();
			clock.restart();
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
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
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
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
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
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;

	body->CreateFixture(&fixtureDef);
}



PowerUp :: ~PowerUp() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
		
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
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
	y_direction = 0.0f;
	b2CircleShape circle;
	//circle.m_p.Set(0.0f, 0.0f);
	circle.m_radius = 0.51f;

	b2PolygonShape rect;
	rect.SetAsBox(size.x / 2.0f, (size.y - 0.8f)/2, b2Vec2(0.0f, size.y / 2.0f), 0.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &circle;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);
	
	fixtureDef.shape = &rect;
	fixtureDef.isSensor = true;
	fixture = body->CreateFixture(&fixtureDef);

	body->SetLinearVelocity(b2Vec2(speed * x_direction, 0.0f));
}

void Enemy::OnBeginContact(b2Fixture* self, b2Fixture* other) {
	if (!self) {
		std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
		return;  // Exit the function if fixture is invalid
	}
	FixtureData* otherData = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
	FixtureData* selfData = reinterpret_cast<FixtureData*> (self->GetUserData().pointer);
	if (!otherData) return;
	if (self == fixture) {
		if (otherData->type == FixtureDataType::Enemy || otherData->type == FixtureDataType::MapTile) {
			cout << "Colliding !" << endl;
			//x_direction = -1.0f * x_direction;
			//y_direction = -1.0f * y_direction;
			body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}
	}
}

Enemy :: ~Enemy() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
	cout << "Successfully Destroyed Enemy !" << endl;
}

void Enemy::Die() {
	isDead = true;
	deleted = true;
	size.y = size.y / 5.0f;
	//position.y += size.y / 5.0f;
}

void Flame::Begin() {
	soundFX.setBuffer(Resources::sfx["fire.wav"]);
	soundFX.setVolume(15);
	soundFX.play();

	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->listener = this;
	fixtureData->type = FixtureDataType::Entity;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	if (faceLeft) {
		x_direction = -1.0f;
		bodyDef.position.Set(position.x + 148.0f, position.y);
	}
	else bodyDef.position.Set(position.x + 152.0f, position.y);
	y_direction = 0.0f;
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 0.0f;
	fixture = body->CreateFixture(&fixtureDef);

	body->SetLinearVelocity(b2Vec2(speed * x_direction, 0.0f));
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
			Enemy* enemy = nullptr;
			if (otherData->entity->getName() == "bowser")
				enemy = reinterpret_cast<Bowser*> (otherData->entity);
			else
				enemy = dynamic_cast<Enemy*> (otherData->entity);
			if (enemy) {
				enemy->Die();
				cout << "Kill " << enemy->getName() << endl;
			}
		}
		else if (otherData->type == FixtureDataType::MapTile) {
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
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
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
		//entity = nullptr;
	}
	onEntities.clear();
}

void Elevator::Begin()
{
	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->type = FixtureDataType::Enemy;

	
	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);
	x_direction = 0.0f;
	
	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);

	body->SetLinearVelocity(b2Vec2(0.0f, speed * y_direction));
}

Elevator:: ~Elevator() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
	cout << "Successfully Destroyed Enemy !" << endl;
}

void Bowser::Begin() {
	fixtureData = new FixtureData();
	fixtureData->entity = this;
	fixtureData->type = FixtureDataType::Enemy;


	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);
	y_direction = 0.0f;
	b2PolygonShape shape;
	shape.SetAsBox(size.x / 2.0f, size.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.0f;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);

	body->SetLinearVelocity(b2Vec2(speed * x_direction, 0.0f));
}

void Bowser::Update(float deltaTime) {
	//cout << "Update Bowser !!!" << endl;
	fireTime += deltaTime;
	if (fireTime > 4.0f) {
		Entity* flame = new Flame("flame", 0.5, 5.0f, body->GetPosition().x - 151.0f, body->GetPosition().x + 200.0f, body->GetPosition().y, body->GetPosition().y + 1000.0f, Vector2f(2.0f, 1.0f), Vector2f(body->GetPosition().x, body->GetPosition().y));
		flame->faceLeft = true;
		flame->Begin();
		cout << "Bowser has fire too !" << endl;
		onEntities.push_back(flame);
		fireTime = 0.0f;
		return;
	}
	Moveable::Update(deltaTime);
}

Bowser :: ~Bowser() {
	if (body) {
		Physics::world.DestroyBody(body);
		body = nullptr;
	}
	if (fixtureData) {
		delete fixtureData;
		fixtureData = nullptr;
	}
	cout << "Successfully Destroyed Enemy !" << endl;
}