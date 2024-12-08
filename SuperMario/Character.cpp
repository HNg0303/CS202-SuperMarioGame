#include "Character.h"


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

void Character::OnBeginContact() {
    groundContactCount++;
    if (groundContactCount > 0) {
        onGround = true;  // Mario is on the ground
        isJumping = false;
    }
    std::cout << "Begin Contact - Ground count: " << groundContactCount << std::endl;
}

void Character::OnEndContact() {
    groundContactCount--;
    if (groundContactCount <= 0) {
        onGround = false;  // Mario is on the ground
    }
    std::cout << "Begin Contact - Ground count: " << groundContactCount << std::endl;
}

void Character::Update(float deltaTime) {
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
    if (Keyboard::isKeyPressed(Keyboard::Up) && onGround && !isJumping) {
        velocity.y -= jump;
        isJumping = true;
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


Mario::Mario(float x, float y) {
    position.x = x;
    position.y = y;
    goRight = goUp = goLeft = goDown = isJumping = false;
    movementVelocity = 7.0f;
    jumpVelocity = 5.0f;
    angle = 0.0f;
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
    /*
    //Initialize a body of Character in the b2World.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // specify type of body
    bodyDef.position.Set(position.x, position.y); //Set position
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    bodyDef.fixedRotation = true;
    dynamicBody = Physics::world.CreateBody(&bodyDef);
    
    //Create and add Fixtures for the body => This will set for collision
    b2FixtureDef fixtureDef{};
    b2PolygonShape shape;
    //shape.SetAsBox(0.2f, 1.0f);

    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    //Create fixture for body => Done;
    //dynamicBody->CreateFixture(&fixtureDef);
    
    b2CircleShape circle{};
    fixtureDef.shape = &circle;
    circle.m_p.Set(0.0f, -0.5f); //His head.
    circle.m_radius = 0.5f;
    dynamicBody->CreateFixture(&fixtureDef);

    circle.m_p.Set(0.0f, 0.5f); //His feet.
    dynamicBody->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f, 0.5f);
    fixtureDef.shape = &polygonShape;
    dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.3f, 0.1f, b2Vec2(0.0f, 1.0f), 0.0f);
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.isSensor = true;
    dynamicBody->CreateFixture(&fixtureDef);
    //dynamicBody->GetUserData().pointer = (uintptr_t)this;
    */

    // Initialize a body of Character in the b2World
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;  // Specify type of body
    bodyDef.position.Set(position.x, position.y);  // Set position
    bodyDef.fixedRotation = true;  // Prevent Mario from rotating on collision
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    dynamicBody = Physics::world.CreateBody(&bodyDef);

    // Body fixture (Main Body)
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(0.2f, 0.9f);  // Main body size
    b2FixtureDef bodyFixtureDef{};
    bodyFixtureDef.shape = &bodyShape;
    bodyFixtureDef.density = 1.0f;
    bodyFixtureDef.friction = 0.3f;
    bodyFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);  // Set body user data
    dynamicBody->CreateFixture(&bodyFixtureDef);

    // Feet sensor (Ground detection)
    b2PolygonShape feetShape;
    feetShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, 0.8f), 0.0f);  // Positioned slightly below Mario

    b2FixtureDef sensorFixtureDef{};
    sensorFixtureDef.shape = &feetShape;
    sensorFixtureDef.isSensor = true;  // Sensor detects ground
    sensorFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);  // Set sensor user data
    dynamicBody->CreateFixture(&sensorFixtureDef);
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
    // Initialize a body of Character in the b2World
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;  // Specify type of body
    bodyDef.position.Set(position.x, position.y);  // Set position
    bodyDef.fixedRotation = true;  // Prevent Luigi from rotating on collision
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    dynamicBody = Physics::world.CreateBody(&bodyDef);

    // Body fixture (Main Body)
    b2PolygonShape bodyShape;
    bodyShape.SetAsBox(0.2f, 0.8f);  // Main body size
    b2FixtureDef bodyFixtureDef{};
    bodyFixtureDef.shape = &bodyShape;
    bodyFixtureDef.density = 1.0f;
    bodyFixtureDef.friction = 0.3f;
    bodyFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);  // Set body user data
    dynamicBody->CreateFixture(&bodyFixtureDef);

    // Feet sensor (Ground detection)
    b2PolygonShape feetShape;
    feetShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, 0.8f), 0.0f);  // Positioned slightly below Mario

    b2FixtureDef sensorFixtureDef{};
    sensorFixtureDef.shape = &feetShape;
    sensorFixtureDef.isSensor = true;  // Sensor detects ground
    sensorFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);  // Set sensor user data
    dynamicBody->CreateFixture(&sensorFixtureDef);
}


void Luigi::Draw(Renderer& renderer, int state, Resources& resource) {
    if (state == 0) //Small Luigi.
        renderer.Draw(resource.getTexture("luigi.png"), position, Vector2f(1.0f, 2.0f), 0);
    else return;
}