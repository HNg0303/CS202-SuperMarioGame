#include "Character.h"

Sound soundEffect{};

Character* CharacterFactory::createCharacter(CharacterType type,int lives) {
    switch (type) {
    case MARIO:
        return new Mario(0, 0, lives);
    case LUIGI:
        return new Luigi(0, 0, lives);
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
        soundEffect.setBuffer(Resources::sfx["win.wav"]);
        soundEffect.setVolume(10);
        soundEffect.play();
        win = true;
        return;
    }
    if (data->type == FixtureDataType::MapTile && (data->entity->getName() == "spike" || data->entity->getName() == "lava"|| data->entity->getName() == "spikeyTurtle")) {
        handleDeath();
        return;
    }
    if (data->type == FixtureDataType::Entity && (data->entity->getName() == "flame")) {
        data->entity->markDeleted();
        handleDeath();
        return;
    }
    if (headFixture == self && data->type == FixtureDataType::MapTile && data->entity->getName() == "qblock") {
        soundEffect.setBuffer(Resources::sfx["starappear.wav"]);
        soundEffect.setVolume(10);
        soundEffect.play();
        data->entity->markDeleted();
        Entity* star = new PowerUp("star", 0.3, data->entity->position.x, data->entity->position.y - 1.0f, data->entity->size, Vector2f(data->entity->position.x, data->entity->position.y - 1.0f));
        //star->Begin();
        cout << "Initialize Star successfully" << endl;
        onEntities.push_back(star);
        return;
    }
    if (groundFixture == self && data->type == FixtureDataType::MapTile) {
        onGround++;
    }
    else if (data->type == FixtureDataType::Entity && data->entity && data->entity->getName() == "coin") {
        if (!data->entity->deleted) {
            soundEffect.setBuffer(Resources::sfx["coin.wav"]);
            soundEffect.setVolume(10);
            soundEffect.play();
            data->entity->markDeleted();
            ++coin;
        }
    }
    else if (data->entity && data->type == FixtureDataType::Enemy) {
        if (data->entity->getName() == "fireBar") {
            b2Vec2 velocity = data->entity->body->GetLinearVelocity();
            velocity.x = 0.0f;
            data->entity->body->SetLinearVelocity(velocity);
            handleDeath();
            return;
        }
        else if (data->entity->getName() == "bowser") {
            handleDeath();
            return;
        }
        else if (data->entity->getName() == "goombas") {
            if (groundFixture == self) {
                soundEffect.setBuffer(Resources::sfx["kill.wav"]);
                soundEffect.setVolume(10);
                soundEffect.play();
                Enemy* enemy = dynamic_cast<Enemy*> (data->entity);
                if (enemy) {
                    enemy->Die();
                    cout << "Kill Goombas" << endl;
                }
                return;
            }
            else
                handleDeath();
        }
    }
    else if (data->entity && data->type == FixtureDataType::Entity && data->entity->getName() == "levelUp") {
        soundEffect.setBuffer(Resources::sfx["lvlup.wav"]);
        soundEffect.setVolume(10);
        soundEffect.play();
        if(!changeStateCounter)
            position.y -= 0.5f;
        data->entity->markDeleted();
        cout << "LEVEL UP HEHEE !!!!!!!!!!" << endl;
        changeStateCounter = 1;
        transform = true;
    }
    else if (data->entity && data->type == FixtureDataType::Entity && data->entity->getName() == "star") {
        soundEffect.setBuffer(Resources::sfx["lvlup.wav"]);
        soundEffect.setVolume(10);
        soundEffect.play();
        if (!changeStateCounter)
            position.y -= 0.5f;
        data->entity->markDeleted();
        cout << "FLAME HEHEE !!!!!!!!!!" << endl;
        changeStateCounter = 2;
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
}

void Character::handleDeath() {
    soundEffect.setBuffer(Resources::sfx["death.wav"]);
    soundEffect.setVolume(50);
    soundEffect.play();
    if (changeStateCounter) {
        changeStateCounter = 0;
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

Mario::Mario(float x, float y, int lives) {
    position.x = x;
    position.y = y;
    movementVelocity = 7.0f;
    jumpVelocity = 3.0f;
    angle = 0.0f;

    this->lives = lives;
    if (!fixtureData)
        fixtureData = new FixtureData();
    //groundFixture = new b2Fixture();
    cout << "Initialize Mario successfully !\n";
}



void Mario::Draw(Renderer& renderer) {
    if (changeStateCounter == 0) //Small Mario.
    {
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 1.0f), 0, faceLeft);
    }
    if (changeStateCounter == 1 || changeStateCounter == 2)  //Big Mario
    {
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 2.0f), 0, faceLeft);
    }
    else return;
}

void Mario::Begin() {
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    isDead = false;
    transform = false;

    float scale = 0.5f;

    //Set up animation and SFX
    if (changeStateCounter == 0) {
        standAnimation = Resources::textures["mario0.png"];
        jumpAnimation = Resources::textures["mariojump0.png"];
        runAnimation = Animation(0.45f,
            {
                Frame(0.15f, Resources::textures["mariorun1.png"]),
                Frame(0.3f, Resources::textures["mariorun2.png"]),
                Frame(0.45f, Resources::textures["mariorun3.png"])
            });
        changeStateAnimation = Animation(0.9f,
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
        jumpSFX.setBuffer(Resources::sfx["jump.wav"]);
        jumpSFX.setVolume(7);
    }
    else if (changeStateCounter == 1 || changeStateCounter == 2) {
        if (changeStateCounter == 1)
        {
            standAnimation = Resources::textures["mario1.png"];
            jumpAnimation = Resources::textures["mariojump1.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["mariorun4.png"]),
                    Frame(0.3f, Resources::textures["mariorun5.png"]),
                    Frame(0.45f, Resources::textures["mariorun6.png"])
                });
        }
        else
        {
            standAnimation = Resources::textures["mario2.png"];
            jumpAnimation = Resources::textures["mariojump2.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["mariorun7.png"]),
                    Frame(0.3f, Resources::textures["mariorun8.png"]),
                    Frame(0.45f, Resources::textures["mariorun9.png"])
                });
        }
        jumpSFX.setBuffer(Resources::sfx["jumpbig.wav"]);
        jumpSFX.setVolume(7);
        scale = 1.0f;
    }

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
    circle.m_p.Set(0.0f, -0.5f * scale); //His head.
    circle.m_radius = 0.4f * scale;
    dynamicBody->CreateFixture(&fixtureDef);

    circle.m_p.Set(0.0f, 0.5f * scale); //His feet.
    dynamicBody->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f * scale, 0.3f * scale);
    fixtureDef.shape = &polygonShape;
    dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f * (scale), 0.1f * (scale), b2Vec2(0.0f, 1.0f*(scale)), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    groundFixture = dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f * (scale), 0.1f * (scale), b2Vec2(0.0f, -0.8f*(scale)), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    headFixture = dynamicBody->CreateFixture(&fixtureDef);
    cout << "Initialize Mario Body successfully !!!!!" << endl;
}

void Mario::Update(float& deltaTime)
{
    if (isDead || transform) {
        if (dynamicBody) {
            Physics::world.DestroyBody(dynamicBody);
            dynamicBody = nullptr;
        }
        changeStateAnimation.Update(deltaTime);
        drawingTexture = changeStateAnimation.getTexture();
        transformTimer += deltaTime;
        if (transformTimer > 1.0f) {
            if (isDead) {
                if (lives > 1) lives -= 1;
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
        Entity* flame = new Flame("flame", 0.5, 7.0f, position.x - 150.0f, position.x + 200.0f, position.y + 0.2f, position.y + 1000.0f, Vector2f(2.0f, 1.0f), position);
        flame->faceLeft = this->faceLeft;
        flame->Begin();
        onEntities.push_back(flame);
        changeStateCounter = 0;
        transformTimer += 1.0f;
        transform = true;
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
    //Check Bound 
    if (position.y >= yBound || position.x < xBound.first || position.x > xBound.second) 
        handleDeath();
    angle = dynamicBody->GetAngle() * (180.0f / PI); //Angle calculated in radian
}

void Mario::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
    int prevStateCounter = changeStateCounter;
    Character::OnBeginContact(self, other);
    if (transform)
    {
        if (prevStateCounter < changeStateCounter)
        {
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.075f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.15f, Texture{}),
                    Frame(0.225f, Resources::textures["mariojump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.3f, Texture{}),
                    Frame(0.375f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.45f, Texture{}),
                    Frame(0.525f, Resources::textures["mariojump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.6f, Texture{}),
                    Frame(0.675f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.75f, Texture{}),
                    Frame(0.825f, Resources::textures["mariojump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.9f, Texture{})
                });
            return;
        }
        if (prevStateCounter > changeStateCounter)
        {
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["mariolvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.2f, Resources::textures["mario" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.3f, Resources::textures["mariolvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.4f, Resources::textures["mario" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.5f, Resources::textures["mariolvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.6f, Resources::textures["mario" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.7f, Resources::textures["mariolvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.8f, Resources::textures["mario" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.9f, Resources::textures["mariolvdown" + to_string(prevStateCounter) + ".png"])
                });
            return;
        }
        changeStateAnimation = Animation(0.9f,
            {
                Frame(0.1f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.2f, Texture{}),
                Frame(0.3f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.4f, Texture{}),
                Frame(0.5f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.6f, Texture{}),
                Frame(0.7f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.8f, Texture{}),
                Frame(0.9f, Resources::textures["mariojump" + to_string(changeStateCounter) + ".png"])
            });
    }
}


Luigi::Luigi(float x, float y, int lives) {
    position.x = x;
    position.y = y;
    movementVelocity = 5.0f;
    jumpVelocity = 3.5f;
    angle = 0.0f;

    this->lives = lives;

    if (!fixtureData)
        fixtureData = new FixtureData();
    cout << "Initializing Luigi successfully !";
}

void Luigi::Begin() {
    //Set up Fixture Data for handle collision
    fixtureData->type = FixtureDataType::Character;
    fixtureData->listener = this;

    isDead = false;
    transform = false;

    float scale = 0.5f;

    //Set up animation and SFX
    if (changeStateCounter == 0) {
        standAnimation = Resources::textures["luigi0.png"];
        jumpAnimation = Resources::textures["luigijump0.png"];
        runAnimation = Animation(0.45f,
            {
                Frame(0.15f, Resources::textures["luigirun1.png"]),
                Frame(0.3f, Resources::textures["luigirun2.png"]),
                Frame(0.45f, Resources::textures["luigirun3.png"])
            });
        changeStateAnimation = Animation(0.9f,
            {
                Frame(0.1f, Resources::textures["luigideath.png"]),
                Frame(0.2f, Texture{}),
                Frame(0.3f, Resources::textures["luigideath.png"]),
                Frame(0.4f, Texture{}),
                Frame(0.5f, Resources::textures["luigideath.png"]),
                Frame(0.6f, Texture{}),
                Frame(0.7f, Resources::textures["luigideath.png"]),
                Frame(0.8f, Texture{}),
                Frame(0.9f, Resources::textures["luigideath.png"])
            });
        jumpSFX.setBuffer(Resources::sfx["jump.wav"]);
        jumpSFX.setVolume(7);
    }
    else if (changeStateCounter == 1 || changeStateCounter == 2) {
        if (changeStateCounter == 1)
        {
            standAnimation = Resources::textures["luigi1.png"];
            jumpAnimation = Resources::textures["luigijump1.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["luigirun4.png"]),
                    Frame(0.3f, Resources::textures["luigirun5.png"]),
                    Frame(0.45f, Resources::textures["luigirun6.png"])
                });
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["luigilvdown1.png"]),
                    Frame(0.2f, Resources::textures["luigi.png"]),
                    Frame(0.3f, Resources::textures["luigilvdown1.png"]),
                    Frame(0.4f, Resources::textures["luigi.png"]),
                    Frame(0.5f, Resources::textures["luigilvdown1.png"]),
                    Frame(0.6f, Resources::textures["luigi.png"]),
                    Frame(0.7f, Resources::textures["luigilvdown1.png"]),
                    Frame(0.8f, Resources::textures["luigi.png"]),
                    Frame(0.9f, Resources::textures["luigilvdown1.png"])
                });
        }
        else
        {
            standAnimation = Resources::textures["luigi2.png"];
            jumpAnimation = Resources::textures["luigijump2.png"];
            runAnimation = Animation(0.45f,
                {
                    Frame(0.15f, Resources::textures["luigirun7.png"]),
                    Frame(0.3f, Resources::textures["luigirun8.png"]),
                    Frame(0.45f, Resources::textures["luigirun9.png"])
                });
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["luigilvdown2.png"]),
                    Frame(0.2f, Resources::textures["luigi.png"]),
                    Frame(0.3f, Resources::textures["luigilvdown2.png"]),
                    Frame(0.4f, Resources::textures["luigi.png"]),
                    Frame(0.5f, Resources::textures["luigilvdown2.png"]),
                    Frame(0.6f, Resources::textures["luigi.png"]),
                    Frame(0.7f, Resources::textures["luigilvdown2.png"]),
                    Frame(0.8f, Resources::textures["luigi.png"]),
                    Frame(0.9f, Resources::textures["luigilvdown2.png"])
                });
        }
        jumpSFX.setBuffer(Resources::sfx["jumpbig.wav"]);
        jumpSFX.setVolume(7);
        scale = 1.0f;
    }

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
    circle.m_p.Set(0.0f, -0.5f * scale); //His head.
    circle.m_radius = 0.4f * scale;
    dynamicBody->CreateFixture(&fixtureDef);

    circle.m_p.Set(0.0f, 0.4f * scale); //His feet.
    dynamicBody->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.5f * scale, 0.3f * scale);
    fixtureDef.shape = &polygonShape;
    dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f * (scale), 0.1f * (scale), b2Vec2(0.0f, 1.0f * (scale)), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    groundFixture = dynamicBody->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f * (scale), 0.1f * (scale), b2Vec2(0.0f, -0.8f * (scale)), 0.0f);
    //fixtureDef.userData.pointer = reinterpret_cast<uintptr_t> (this);
    fixtureDef.shape = &polygonShape;
    fixtureDef.isSensor = true;
    headFixture = dynamicBody->CreateFixture(&fixtureDef);
    cout << "Initialize Luigi Body successfully !!!!!" << endl;
}

void Luigi::Update(float& deltaTime)
{
    if (isDead || transform) {
        if (dynamicBody) {
            Physics::world.DestroyBody(dynamicBody);
            dynamicBody = nullptr;
        }
        transformTimer += deltaTime;
        changeStateAnimation.Update(deltaTime);
        drawingTexture = changeStateAnimation.getTexture();
        if (transformTimer > 1.0f) {
            if (isDead) {
                if (lives > 1) lives -= 1;
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
    float move = movementVelocity;
    float jump = jumpVelocity;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
        move *= 2;
    b2Vec2 velocity = dynamicBody->GetLinearVelocity();
    velocity.x = 0;
    if (Keyboard::isKeyPressed(Keyboard::F) && changeStateCounter == 2) {
        Entity* flame = new Flame("flame", 0.5, 7.0f, position.x - 150.0f, position.x + 200.0f, position.y + 0.2f, position.y + 1000.0f, Vector2f(2.0f, 1.0f), position);
        drawingTexture = Resources::textures["luigiflamethrow.png"];
        flame->faceLeft = this->faceLeft;
        flame->Begin();
        onEntities.push_back(flame);
        changeStateCounter = 0;
        transformTimer += 1.0f;
        transform = true;
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

void Luigi::Draw(Renderer& renderer) {
    if (changeStateCounter == 0) //Small Mario.
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 1.0f), 0, faceLeft);
    if (changeStateCounter == 1 || changeStateCounter == 2) //Big 
        renderer.Draw(drawingTexture, position, Vector2f(1.0f, 2.0f), 0, faceLeft);
    else return;
}

void Luigi::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
    int prevStateCounter = changeStateCounter;
    Character::OnBeginContact(self, other);
    if (transform)
    {
        if (prevStateCounter < changeStateCounter)
        {
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.075f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.15f, Texture{}),
                    Frame(0.225f, Resources::textures["luigijump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.3f, Texture{}),
                    Frame(0.375f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.45f, Texture{}),
                    Frame(0.525f, Resources::textures["luigijump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.6f, Texture{}),
                    Frame(0.675f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.75f, Texture{}),
                    Frame(0.825f, Resources::textures["luigijump" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.9f, Texture{})
                });
            return;
        }
        if (prevStateCounter > changeStateCounter)
        {
            changeStateAnimation = Animation(0.9f,
                {
                    Frame(0.1f, Resources::textures["luigilvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.2f, Resources::textures["luigi" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.3f, Resources::textures["luigilvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.4f, Resources::textures["luigi" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.5f, Resources::textures["luigilvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.6f, Resources::textures["luigi" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.7f, Resources::textures["luigilvdown" + to_string(prevStateCounter) + ".png"]),
                    Frame(0.8f, Resources::textures["luigi" + to_string(changeStateCounter) + ".png"]),
                    Frame(0.9f, Resources::textures["luigilvdown" + to_string(prevStateCounter) + ".png"])
                });
            return;
        }
        changeStateAnimation = Animation(0.9f,
            {
                Frame(0.1f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.2f, Texture{}),
                Frame(0.3f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.4f, Texture{}),
                Frame(0.5f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.6f, Texture{}),
                Frame(0.7f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"]),
                Frame(0.8f, Texture{}),
                Frame(0.9f, Resources::textures["luigijump" + to_string(changeStateCounter) + ".png"])
            });
    }
}