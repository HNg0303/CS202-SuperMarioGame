#pragma once
#include "INCLUDE.h"



enum CharacterType {MARIO, LUIGI, CustomCharacter};

class Character {
    const float movementSpeed = 3.0f;
protected:
    Texture superTexture;
    Texture texture;
    Sprite sprite;
    string name;
    Vector2f position{};

    int changeStateCounter; //3 states (Small, Big, Super)
    bool jumping, onGround;

    float spd[2]; //Acceleration and Speed on the x-axis and y-axis
    float angle{};
public:
    virtual ~Character() = default;
    virtual void draw(RenderWindow& window) = 0;
    //virtual void update(float deltaTime) = 0;
};


class CharacterFactory {
public: 
    static Character* createCharacter(CharacterType type, float x, float y);
};


class Mario : public Character {
protected:
    Clock timer1, timer2;
    bool goRight, goUp, goLeft, goDown;
    float acc[2], startJumpPosition;

public:
    Mario(float x, float y); //set accleration
    //void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
    void setPosition(float x, float y);
};

/*
class Luigi : public Character {
protected:
    Clock timer1, timer2;
    bool goRight, goUp, goLeft, goDown;
    float acc[2], startJumpPosition;
public:
    Luigi();
    Luigi(float x, float y); // set acceleration
    string getName() const override;
    void update(float deltaTime) override;
    void draw(RenderWindow& window) override;
};
*/
