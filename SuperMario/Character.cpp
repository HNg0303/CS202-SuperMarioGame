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
    acc[0] = 57;
    acc[1] = 80;
    spd[0] = 0;
    spd[1] = 70;
    startJumpPosition = 500;
    changeStateCounter = 0;
    goRight = goUp = goLeft = goDown = jumping = onGround = false;
    

    // Set Mario Sprite Properties
    if (!texture.loadFromFile("D:/CS202-SuperMarioGame/SuperMario/Resource/Mario/mario.png")) { cout << "Can't load MARIO_CHARACTER\n"; }
    //if (!marioSuperTexture.loadFromFile(MARIO_SUPER_CHARACTER)) { std::cout << "Can't load MARIO_SUPER_CHARACTER\n"; }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(2, 2);

    //Define here
    //Sound buffer and sound effect : Jump, d
};


void Mario :: setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Mario :: draw(RenderWindow& window) {
    window.draw(sprite);
}


