#pragma once
#include "INCLUDE.h"
#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"
#include "entity.h"
#include "Animation.h"

enum CharacterType { MARIO, LUIGI, CustomCharacter };

class Character : public ContactListener {
public:
    float movementVelocity;
    float jumpVelocity;
    Vector2f position;
    float angle;
    bool faceLeft = 0;
    int onGround = 0;
    bool transform = false;
    bool win = false;
    //Testing collecting coin
    int lives = 3;
    int coin = 0;
    float transformTimer = 0.0f;
    void handleDeath();
    void setPos(Vector2f position);
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
    virtual void setPosition(float x, float y);
    virtual Vector2f getPos();
    ~Character();
    virtual void Begin() = 0;
    virtual void Update(float& deltaTime) = 0;
    virtual void Draw(Renderer& renderer, Resources& resource) = 0;
    bool isDead = false;
protected:
    b2Body* dynamicBody = nullptr;
    int changeStateCounter = 0; //3 states (Small, Big, Super) => we will need 3 bodies for each state.
    sf::Vector2f startPos{};
    FixtureData* fixtureData = nullptr;
    b2Fixture* groundFixture;
    b2Fixture* headFixture;


    Texture drawingTexture{};
    Texture standAnimation{}, jumpAnimation{};
    Animation runAnimation, deathAnimation;
    Sound jumpSFX{};

    //float spd[2]; //Acceleration and Speed on the x-axis and y-axis => Use velocity.
    //float angle{};// => Use velocity in b2_body.
};


class CharacterFactory {
public:
    static Character* createCharacter(CharacterType type);
};


class Mario : public Character {
public:
    Mario(float x = 1.0f, float y = 1.0f); //set Position, Velocity and JumpVelocity
    void Draw(Renderer& renderer, Resources& resource) override;
    void Begin() override;
    void Update(float& deltaTime) override;
};


class Luigi : public Character {
public:
    Luigi(float x = 1.0f, float y = 1.0f);
    void Draw(Renderer& renderer, Resources& resource) override;
    void Begin() override;
    void Update(float& deltaTime) override;
};

