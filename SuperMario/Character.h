#pragma once
#include "INCLUDE.h"
#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"
#include "entity.h"

enum CharacterType {MARIO, LUIGI, CustomCharacter};

class Character : public ContactListener {
public:
    float movementVelocity;
    float jumpVelocity;
    Vector2f position;
    float angle;
    int onGround = 0;

    //Testing collecting coin
    int coin = 0;

    virtual void OnBeginContact(b2Fixture* self, b2Fixture *other) override;
    virtual void OnEndContact(b2Fixture* self, b2Fixture * other) override;
    virtual void setPosition(float x, float y);
    virtual Vector2f getPos();
    virtual ~Character();
    virtual void Begin() = 0;
    virtual void Update(float& deltaTime);
    virtual void Draw(Renderer& renderer, int state, Resources& resource) = 0;
protected:
    b2Body* dynamicBody = nullptr;
    int changeStateCounter; //3 states (Small, Big, Super) => we will need 3 bodies for each state.
    bool isJumping;

    FixtureData* fixtureData = nullptr;
    b2Fixture* groundFixture;
    //float spd[2]; //Acceleration and Speed on the x-axis and y-axis => Use velocity.
    //float angle{};// => Use velocity in b2_body.
};


class CharacterFactory {
public: 
    static Character* createCharacter(CharacterType type);
};


class Mario : public Character {
protected:
    Clock timer1, timer2;
    bool goRight, goUp, goLeft, goDown;
public:
    Mario(float x = 1.0f, float y = 1.0f); //set Position, Velocity and JumpVelocity
    void Draw(Renderer& renderer, int state, Resources& resource) override;
    void Begin() override;
};


class Luigi : public Character {
protected:
    Clock timer1, timer2;
    bool goRight, goUp, goLeft, goDown;
public:
    Luigi(float x = 1.0f, float y = 1.0f);
    void Draw(Renderer& renderer, int state, Resources& resource) override;
    void Begin() override;
};


