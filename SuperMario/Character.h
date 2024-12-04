#pragma once
#include "INCLUDE.h"
#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"
#include "Animation.h"
enum CharacterType {MARIO, LUIGI, CustomCharacter};

class Character {
public:
    float movementVelocity = 0.0f;
    float jumpVelocity = 0.0f;
    Vector2f position;
    float angle = 0.0f;


    virtual ~Character() = default;
    virtual void Begin() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(Renderer& renderer, int state, Resources& resource) = 0;
protected:
    Texture superTexture;
    Texture texture;
    Sprite sprite;
    string name;
    b2Body* dynamicBody = nullptr;
    int changeStateCounter = 0; //3 states (Small, Big, Super) => we will need 3 bodies for each state.
    //bool jumping, onGround;

    //float spd[2]; //Acceleration and Speed on the x-axis and y-axis => Use velocity.
    //float angle{};// => Use velocity in b2_body.

    //virtual void update(float deltaTime) = 0;
};


class CharacterFactory {
public: 
    static Character* createCharacter(CharacterType type, float x, float y);
};


class Mario : public Character {
protected:
    Animation runAnimation;
    Clock timer1, timer2;
    bool goRight, goUp, goLeft, goDown;
    Texture drawingTexture{};
    bool faceLeft = false, onGround = true;
public:
    Mario(float x = 1.0f, float y = 1.0f); //set Position, Velocity and JumpVelocity
    void Draw(Renderer& renderer, int state, Resources& resource) override;
    void setPosition(float x, float y);
    void Begin() override;
    void Update(float deltaTime) override;
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
