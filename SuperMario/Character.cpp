#include "Character.h"


Character* CharacterFactory :: createCharacter(CharacterType type, float x, float y) {
    switch (type) {
    case MARIO:
        return new Mario(x, y);
    default:
        throw invalid_argument("Unknown character type");
    }
}


Mario::Mario(float x, float y) {
    position.x = x;
    position.y = y;
    goRight = goUp = goLeft = goDown = false;
    movementVelocity = 3.0f;
    jumpVelocity = 1.0f;
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


void Mario :: setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Mario :: Draw(Renderer& renderer, int state, Resources& resource) {
    if (state == 0) //Small Mario.
        renderer.Draw(resource.getTexture("mario.jpg"), position, Vector2f(480.0f, 450.0f), 0);
}
 
void Mario::Begin() {
    //Initialize a body of Character in the b2World.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // specify type of body
    bodyDef.position.Set(position.x, position.y); //Set position
    dynamicBody = Physics::world.CreateBody(&bodyDef);

    //Create and add Fixtures for the body => This will set for collision
    b2FixtureDef fixtureDef;
    b2PolygonShape shape{};
    shape.SetAsBox(0.5f, 1.0f);

    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    //Create fixture for body => Done;
    dynamicBody->CreateFixture(&fixtureDef);
}

void Mario::Update(float deltaTime) {
    float move = movementVelocity;
    if (Keyboard::isKeyPressed(Keyboard::LShift))
        move *= 2;
    
    //Update position and angle
    position = Vector2f(dynamicBody->GetPosition().x, dynamicBody->GetPosition().y);
    angle = dynamicBody->GetAngle() * (180.0f / PI); //Angle calculated in radian

}



