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

void Character::OnBeginContact(b2Fixture* self, b2Fixture* other) {
    if (!self) {
        std::cerr << "Warning: Null fixture detected in OnBeginContact!" << std::endl;
        return;  // Exit the function if fixture is invalid
    }
    FixtureData* data = reinterpret_cast<FixtureData*> (other->GetUserData().pointer);
    if (!data) return;
    if (groundFixture == self && data->type == FixtureDataType::MapTile)
        onGround++;
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
            if (changeStateCounter == 1) {
                changeStateCounter -= 1;
                transform = true;
            }
            else
                this->isDead = true;
        }
    }
    else if (data->entity && data->type == FixtureDataType::Entity && data->entity->getName() == "levelUp") {
        data->entity->markDeleted();
        cout << "BIG MARIO HEHEE !!!!!!!!!!" << endl;
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

void Character::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}


Vector2f Character::getPos() {
    return position;
}

Character::~Character() {
    Physics::world.DestroyBody(dynamicBody);
    dynamicBody = nullptr;
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
    runAnimation = Animation(0.45f,
        {
            Frame(0.15f, Resources::textures["run1.png"]),
            Frame(0.3f, Resources::textures["run2.png"]),
            Frame(0.45f, Resources::textures["run3.png"])
        });
    jumpSFX.setBuffer(Resources::sfx["jump.wav"]);
    jumpSFX.setVolume(7);
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    isDead = false;
    transform = false;

    if (changeStateCounter == 0 || changeStateCounter == 2) {
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
    }
    else if (changeStateCounter == 1) {
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
}

void Mario::Update(float& deltaTime)
{
    if (isDead || transform) {
        Physics::world.DestroyBody(dynamicBody);
        dynamicBody = nullptr;
        Begin();
    }
    drawingTexture = Resources::textures["mario1.png"];
    if (isDead) {
        if (lives)
        {
            isDead = false;
            lives--;
            cout << "You have " << lives << " left!" << endl;
        }
        else
        {
            isDead = false;
            cout << "YOU DIED !!!!!!!!!!!" << endl;
        }
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
    if (!onGround)
        drawingTexture = Resources::textures["jump.png"];
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
    if (isDead && lives) {
        isDead = false;
        lives--;
        cout << "You have " << lives << " left !" << endl;
    }
    else if (isDead && !lives) {
        isDead = false;
        cout << "YOU DIED !!!!!!!!!!!" << endl;
    }
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


void Luigi::Draw(Renderer& renderer, Resources& resource) {
    if (changeStateCounter == 0) //Small Luigi.
        renderer.Draw(resource.getTexture("luigi.png"), position, Vector2f(1.0f, 2.0f), 0, 0);
    else return;
}


