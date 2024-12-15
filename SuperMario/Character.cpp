#include "Character.h"
#include "Game.h"

Character* CharacterFactory :: createCharacter(CharacterType type) {
    switch (type) {
    case MARIO:
        return new Mario();
    case LUIGI:
        return new Luigi();
    default:
        throw invalid_argument("Unknown character type");
    }
}

void Character::OnBeginContact(b2Fixture* self, b2Fixture* other) {
    if (!self) {
        std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
        return;  // Exit the function if fixture is invalid
    }
    FixtureData* data = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
    if (!data) return;
    if (groundFixture == self && data->type == FixtureDataType::MapTile)
        onGround++;
    if (data->type == FixtureDataType::Entity && data->entity->getName() == "coin") {
        /*const auto& it = find(Game::onEntities.begin(), Game::onEntities.end(), data->entity);
        if (it != Game::onEntities.end())
            Game::onEntities.erase(it);*/
        data->entity->markDeleted();
        cout << "Coin: " << ++coin << endl;
    }

    /*
    if (groundContactCount > 0) {
        onGround = true;  // Mario is on the ground
        isJumping = false;
    }
    std::cout << "Begin Contact - Ground count: " << groundContactCount << std::endl;*/
}

void Character::OnEndContact(b2Fixture* self, b2Fixture* other) {
    if (!other) {
        std::cerr << "Warning: Null fixture detected in OnEndContact!" << std::endl;
        return;  // Exit the function if fixture is invalid
    }
    FixtureData* data = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
    if (!data) return;
    if (groundFixture == self && data->type == FixtureDataType::MapTile && onGround > 0)
        onGround--;
    /*
    groundContactCount--;
    if (groundContactCount <= 0) {
        onGround = false;  // Mario is on the ground
    }
    std::cout << "Begin Contact - Ground count: " << groundContactCount << std::endl;*/
}

void Character::Update(float& deltaTime) {
    float move = movementVelocity;
    float jump = jumpVelocity;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
        move *= 2;
    b2Vec2 velocity = dynamicBody->GetLinearVelocity();
    velocity.x = 0;
    if (Keyboard::isKeyPressed(Keyboard::Right))
        velocity.x += move;
    if (Keyboard::isKeyPressed(Keyboard::Left))
        velocity.x -= move;
    if (Keyboard::isKeyPressed(Keyboard::Up) && onGround) {
        velocity.y -= jump;
    }
    dynamicBody->SetLinearVelocity(velocity);
    //Update position and angle

    position = Vector2f(dynamicBody->GetPosition().x, dynamicBody->GetPosition().y);
    angle = dynamicBody->GetAngle() * (180.0f / PI); //Angle calculated in radian
}

void Character::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}


Vector2f Character::getPos() {
    return position;
}

Character::~Character() {
    delete fixtureData;
    delete groundFixture;
}

Mario::Mario(float x, float y) {
    position.x = x;
    position.y = y;
    goRight = goUp = goLeft = goDown = isJumping = false;
    movementVelocity = 7.0f;
    jumpVelocity = 5.0f;
    angle = 0.0f;
    fixtureData = new FixtureData();
    //groundFixture = new b2Fixture();
    cout << "Initialize Mario successfully !\n";
    /*
    // Set Mario Sprite Properties
    if (!texture.loadFromFile("./Resource/Mario/mario.png")) { cout << "Can't load MARIO_CHARACTER\n"; }
    //if (!marioSuperTexture.loadFromFile(MARIO_SUPER_CHARACTER)) { std::cout << "Can't load MARIO_SUPER_CHARACTER\n"; }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(2, 2);

    //Define here
    //Sound buffer and sound effect : Jump, d
    */
}


void Mario :: Draw(Renderer& renderer, int state, Resources& resource) {
    if (state == 0) //Small Mario.
        renderer.Draw(resource.getTexture("mario1.png"), position, Vector2f(1.0f, 2.0f), 0);
    else return;
}
 
void Mario::Begin() {
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    //Initialize a body of Character in the b2World.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // specify type of body
    bodyDef.position.Set(position.x, position.y); //Set position
    bodyDef.fixedRotation = true;
    dynamicBody = Physics::world.CreateBody(&bodyDef);
    
    //Create and add Fixtures for the body => This will set for collision
    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
    b2PolygonShape shape;
    //shape.SetAsBox(0.2f, 1.0f);

    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    //Create fixture for body => Done;
    
    b2CircleShape circle{};
    fixtureDef.shape = &circle;
    circle.m_p.Set(0.0f, -0.5f); //His head.
    circle.m_radius = 0.5f;
    dynamicBody->CreateFixture(&fixtureDef);

    circle.m_p.Set(0.0f, 0.5f); //His feet.
    dynamicBody->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3f, 0.5f);
    fixtureDef.shape = &polygonShape;
    dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, 1.0f), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    groundFixture = dynamicBody->CreateFixture(&fixtureDef);
    //dynamicBody->GetUserData().pointer = (uintptr_t)this;


    /*
    fixtureData.type = FixtureDataType::Character;
    fixtureData.listener = this;


    // Initialize a body of Character in the b2World
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;  // Specify type of body
    bodyDef.position.Set(position.x, position.y);  // Set position
    bodyDef.fixedRotation = true;  // Prevent Mario from rotating on collision
    dynamicBody = Physics::world.CreateBody(&bodyDef);

    // Body fixture (Main Body)
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(0.2f, 0.9f);  // Main body size
    b2FixtureDef FixtureDef{};
    FixtureDef.shape = &bodyShape;
    FixtureDef.density = 1.0f;
    FixtureDef.friction = 0.3f;
    dynamicBody->CreateFixture(&bodyFixtureDef);

    // Feet sensor (Ground detection)
    b2PolygonShape feetShape;
    feetShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, 0.8f), 0.0f);  // Positioned slightly below Mario

    b2FixtureDef sensorFixtureDef{};
    sensorFixtureDef.shape = &feetShape;
    sensorFixtureDef.isSensor = true;  // Sensor detects ground
    sensorFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);  // Set sensor user data
    dynamicBody->CreateFixture(&sensorFixtureDef);*/
}


Luigi :: Luigi(float x, float y) {
    position.x = x;
    position.y = y;
    goRight = goUp = goLeft = goDown = false;
    movementVelocity = 5.0f;
    jumpVelocity = 8.0f;
    angle = 0.0f;
}

void Luigi :: Begin() { 
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    //Initialize a body of Character in the b2World.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // specify type of body
    bodyDef.position.Set(position.x, position.y); //Set position
    bodyDef.fixedRotation = true;
    dynamicBody = Physics::world.CreateBody(&bodyDef);

    //Create and add Fixtures for the body => This will set for collision
    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (fixtureData);
    b2PolygonShape shape;
    //shape.SetAsBox(0.2f, 1.0f);

    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    //Create fixture for body => Done;

    b2CircleShape circle{};
    fixtureDef.shape = &circle;
    circle.m_p.Set(0.0f, -0.5f); //His head.
    circle.m_radius = 0.5f;
    dynamicBody->CreateFixture(&fixtureDef);

    circle.m_p.Set(0.0f, 0.5f); //His feet.
    dynamicBody->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3f, 0.5f);
    fixtureDef.shape = &polygonShape;
    dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, 1.0f), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    groundFixture = dynamicBody->CreateFixture(&fixtureDef);
}


void Luigi::Draw(Renderer& renderer, int state, Resources& resource) {
    if (state == 0) //Small Luigi.
        renderer.Draw(resource.getTexture("luigi.png"), position, Vector2f(1.0f, 2.0f), 0);
    else return;
}


