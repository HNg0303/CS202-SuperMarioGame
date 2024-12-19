#include "Character.h"
using namespace std;

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
    if (data->type == FixtureDataType::MapTile && data->entity->getName() == "goal") {
        win = true;
        return;
    }
    if (data->type == FixtureDataType::MapTile && data->entity->getName() == "spike") {
        handleDeath();
        return;
    }
    if (headFixture == self && data->type == FixtureDataType::MapTile && data->entity->getName() == "qblock") {
        data->entity->markDeleted();
        return;
    }
    if (groundFixture == self && data->type == FixtureDataType::MapTile) {
            onGround++;
    }
    else if (data->type == FixtureDataType::Entity && data->entity && data->entity->getName() == "coin") {
        //deleteEntity(data->entity);
        data->entity->markDeleted();
        cout << "Coin: " << ++coin << endl;
    }
    else if (data->entity && data->type == FixtureDataType::Enemy && data->entity->getName() == "goombas") {
        if (groundFixture == self) {
            Enemy* enemy = dynamic_cast<Enemy*> (data->entity);
            if (enemy) {
                enemy->Die();
                cout << "Kill Goombas" << endl;
            }
        }
        else {
            handleDeath();
        }
    }
    else if (data->entity && data->type == FixtureDataType::Entity && data->entity->getName() == "levelUp") {
        data->entity->markDeleted();
        cout << "LEVEL UP HEHEE !!!!!!!!!!" << endl;
        changeStateCounter = 1;
        transform = true;
    }
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

void Character::handleDeath() {
    if (changeStateCounter == 1) {
        changeStateCounter -= 1;
        transform = true;
    }
    else {
        isDead = true;
    }
}
void Character::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void Character::setPos(Vector2f position) {
    this->startPos = position;
    this->position = position;
}

Vector2f Character::getPos() {
    return position;
}

Character::~Character() {
    if (dynamicBody) {
        Physics::world.DestroyBody(dynamicBody);
        dynamicBody = nullptr;
    }
    delete fixtureData;
    fixtureData = nullptr;
}

Mario::Mario(float x, float y) {
    position.x = x;
    position.y = y;
    movementVelocity = 7.0f;
    jumpVelocity = 5.0f;
    angle = 0.0f;
    if (!fixtureData)
        fixtureData = new FixtureData();
    //groundFixture = new b2Fixture();
    cout << "Initialize Mario successfully !\n";
}



void Mario :: Draw(Renderer& renderer, Resources& resource) {
    if (changeStateCounter == 0 || changeStateCounter == 2) //Small Mario.
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 2.0f), 0, faceLeft);
    if (changeStateCounter == 1) //Big Mario
        renderer.Draw(drawingTexture, position, Vector2f(1.5f, 3.0f), 0, faceLeft);
    else return;
}
 
void Mario::Begin() {
    //Set up animation and SFX
    jumpSFX.setBuffer(Resources::sfx["jump.wav"]);
    jumpSFX.setVolume(7);
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    isDead = false;
    transform = false;

    if (changeStateCounter == 0) {
        standAnimation = Resources::textures["mario1.png"];
        jumpAnimation = Resources::textures["mariojump.png"];
        runAnimation = Animation(0.45f,
            {
                Frame(0.15f, Resources::textures["mariorun1.png"]),
                Frame(0.3f, Resources::textures["mariorun2.png"]),
                Frame(0.45f, Resources::textures["mariorun3.png"])
            });
        deathAnimation = Animation(0.9f,
            {
                Frame(0.1f, Resources::textures["mariodeath.png"]),
                Frame(0.2f, Texture{}),
                Frame(0.3f, Resources::textures["mariodeath.png"]),
                Frame(0.4f, Texture{}),
                Frame(0.5f, Resources::textures["mariodeath.png"]),
                Frame(0.6f, Texture{}),
                Frame(0.7f, Resources::textures["mariodeath.png"]),
                Frame(0.8f, Texture{}),
                Frame(0.9f, Resources::textures["mariodeath.png"])
            });
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
        circle.m_radius = 0.4f;
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

        polygonShape.SetAsBox(0.2f, 0.1f, b2Vec2(0.0f, -0.8f), 0.0f);
        fixtureDef.shape = &polygonShape;
        fixtureDef.isSensor = true;
        headFixture = dynamicBody->CreateFixture(&fixtureDef);
    }
    else if (changeStateCounter == 1 || changeStateCounter == 2) {
        if (changeStateCounter == 1)
        {
            standAnimation = Resources::textures["mario2.png"];
            jumpAnimation = Resources::textures["mariojump1.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["mariorun4.png"]),
                    Frame(0.3f, Resources::textures["mariorun5.png"]),
                    Frame(0.45f, Resources::textures["mariorun6.png"])
                });
            deathAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["mariolvdown1.png"]),
                    Frame(0.2f, Resources::textures["mario1.png"]),
                    Frame(0.3f, Resources::textures["mariolvdown1.png"]),
                    Frame(0.4f, Resources::textures["mario1.png"]), 
                    Frame(0.5f, Resources::textures["mariolvdown1.png"]),
                    Frame(0.6f, Resources::textures["mario1.png"]), 
                    Frame(0.7f, Resources::textures["mariolvdown1.png"]),
                    Frame(0.8f, Resources::textures["mario1.png"]), 
                    Frame(0.9f, Resources::textures["mariolvdown1.png"])
                });
        }
        else
        {
            standAnimation = Resources::textures["mario3.png"];
            jumpAnimation = Resources::textures["mariojump2.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["mariorun7.png"]),
                    Frame(0.3f, Resources::textures["mariorun8.png"]),
                    Frame(0.45f, Resources::textures["mariorun9.png"])
                });
            deathAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["mariolvdown2.png"]),
                    Frame(0.2f, Resources::textures["mario1.png"]),
                    Frame(0.3f, Resources::textures["mariolvdown2.png"]),
                    Frame(0.4f, Resources::textures["mario1.png"]),
                    Frame(0.5f, Resources::textures["mariolvdown2.png"]),
                    Frame(0.6f, Resources::textures["mario1.png"]),
                    Frame(0.7f, Resources::textures["mariolvdown2.png"]),
                    Frame(0.8f, Resources::textures["mario1.png"]),
                    Frame(0.9f, Resources::textures["mariolvdown2.png"])
                });
        }
        float scale = 1.5f;
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
        circle.m_p.Set(0.0f, -0.5f*scale); //His head.
        circle.m_radius = 0.4f*scale;
        dynamicBody->CreateFixture(&fixtureDef);

        circle.m_p.Set(0.0f, 0.5f*scale); //His feet.
        dynamicBody->CreateFixture(&fixtureDef);

        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(0.3f*scale, 0.5f*scale);
        fixtureDef.shape = &polygonShape;
        dynamicBody->CreateFixture(&fixtureDef);

        polygonShape.SetAsBox(0.2f*(scale) , 0.1f*(scale - 0.1f), b2Vec2(0.0f, 1.5f), 0.0f);
        //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
        fixtureDef.shape = &polygonShape;
        fixtureDef.isSensor = true;
        groundFixture = dynamicBody->CreateFixture(&fixtureDef);
    }
    cout << "Initialize Mario Body successfully !!!!!" << endl;
}

void Mario::Update(float& deltaTime)
{
    if (isDead || transform) {
        if (dynamicBody) {
            Physics::world.DestroyBody(dynamicBody);
            dynamicBody = nullptr;
        }
        if (isDead)
        {
            deathAnimation.Update(deltaTime);
            drawingTexture = deathAnimation.getTexture();
        }
        transformTimer += deltaTime;
        if (transformTimer > 1.0f) {
            if (isDead) {
                if (lives > 1) lives-=1;
                else {
                    lives -= 1;
                    cout << "You're dead !" << endl;
                    return;
                }
                position = startPos;
            }
            Begin();
            transformTimer = 0.0f;
        }
        return;
    }
    drawingTexture = Resources::textures["mario1.png"];
    float move = movementVelocity;
    float jump = jumpVelocity;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
        move *= 2;
    b2Vec2 velocity = dynamicBody->GetLinearVelocity();
    velocity.x = 0;
    if (Keyboard::isKeyPressed(Keyboard::F) && changeStateCounter == 2) {
        drawingTexture = Resources::textures["marioflamethrow.png"];
        Entity* flame = new Flame("flame", 0.5, 0.3f, position.x + 3.0f, position.x + 150.0f, position.y, Vector2f(2.0f, 1.0f), position);
        flame->Begin();
        onEntities.push_back(flame);
        changeStateCounter = 0;
        //transform = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
    {
        runAnimation.Update(deltaTime);
        drawingTexture = runAnimation.getTexture();
        faceLeft = false;
        velocity.x += move;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
    {
        runAnimation.Update(deltaTime);
        drawingTexture = runAnimation.getTexture();
        faceLeft = true;
        velocity.x -= move;
    }
    if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && onGround) {
        velocity.y -= jump;
        jumpSFX.play();
    }
    if (velocity.x == 0)
        drawingTexture = standAnimation;
    if (!onGround)
        drawingTexture = jumpAnimation;
    dynamicBody->SetLinearVelocity(velocity);
    //Update position and angle

    position = Vector2f(dynamicBody->GetPosition().x, dynamicBody->GetPosition().y);
    angle = dynamicBody->GetAngle() * (180.0f / PI); //Angle calculated in radian
}


Luigi :: Luigi(float x, float y) {
    position.x = x;
    position.y = y;
    movementVelocity = 5.0f;
    jumpVelocity = 8.0f;
    angle = 0.0f;

    fixtureData = new FixtureData();
    cout << "Initializing Luigi successfully !";
}

void Luigi :: Begin() { 
    standAnimation = Resources::textures["luigi.png"];
    jumpAnimation = Resources::textures["luigijump.png"];
    runAnimation = Animation(0.45f,
        {
            Frame(0.15f, Resources::textures["luigirun1.png"]),
            Frame(0.3f, Resources::textures["luigirun2.png"]),
            Frame(0.45f, Resources::textures["luigirun3.png"])
        });
    deathAnimation = Animation(0.9f,
        {
            Frame(0.1f, Resources::textures["mariodeath.png"]),
            Frame(0.2f, Texture{}),
            Frame(0.3f, Resources::textures["mariodeath.png"]),
            Frame(0.4f, Texture{}),
            Frame(0.5f, Resources::textures["mariodeath.png"]),
            Frame(0.6f, Texture{}),
            Frame(0.7f, Resources::textures["mariodeath.png"]),
            Frame(0.8f, Texture{}),
            Frame(0.9f, Resources::textures["mariodeath.png"])
        });
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

void Luigi::Update(float& deltaTime)
{
    if (isDead || transform) {
        if (dynamicBody) {
            Physics::world.DestroyBody(dynamicBody);
            dynamicBody = nullptr;
        }
        transformTimer += deltaTime;
        deathAnimation.Update(deltaTime);
        drawingTexture = deathAnimation.getTexture();
        if (transformTimer > 1.0f) {
            position = startPos;
            Begin();
            transformTimer = 0.0f;
        }
        return;
    }
    float move = movementVelocity;
    float jump = jumpVelocity;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
        move *= 2;
    b2Vec2 velocity = dynamicBody->GetLinearVelocity();
    velocity.x = 0;
    if (Keyboard::isKeyPressed(Keyboard::F) && changeStateCounter == 2) {
        Entity* flame = new Flame("flame", 0.5, 0.3f, position.x + 3.0f, position.x + 150.0f, position.y, Vector2f(2.0f, 1.0f), position);
        flame->Begin();
        onEntities.push_back(flame);
        changeStateCounter = 0;
        //transform = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        runAnimation.Update(deltaTime);
        drawingTexture = runAnimation.getTexture();
        faceLeft = false;
        velocity.x += move;
    }
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        runAnimation.Update(deltaTime);
        drawingTexture = runAnimation.getTexture();
        faceLeft = true;
        velocity.x -= move;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up) && onGround) {
        velocity.y -= jump;
        jumpSFX.play();
    }
    if (velocity.x == 0)
        drawingTexture = standAnimation;
    if (!onGround)
        drawingTexture = jumpAnimation;
    dynamicBody->SetLinearVelocity(velocity);
    //Update position and angle

    position = Vector2f(dynamicBody->GetPosition().x, dynamicBody->GetPosition().y);
    angle = dynamicBody->GetAngle() * (180.0f / PI); //Angle calculated in radian
}


void Luigi::Draw(Renderer& renderer, Resources& resource) {
    if (changeStateCounter == 0) //Small Luigi.
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 2.0f), 0, faceLeft);
    else return;
}


