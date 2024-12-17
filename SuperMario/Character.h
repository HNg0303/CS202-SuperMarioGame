#pragma once
#include "INCLUDE.h"
#include "Renderer.h"
#include "Resources.h"
#include "Physics.h"
#include "entity.h"
#include "Animation.h"

enum CharacterType {MARIO, LUIGI, CustomCharacter};

class Character : public ContactListener {
public:
    float movementVelocity;
    float jumpVelocity;
    Vector2f position;
    float angle;
    bool faceLeft = 0;
    int onGround = 0;
    bool transform = false;
    //Testing collecting coin
    int coin = 0;

    virtual void OnBeginContact(b2Fixture* self, b2Fixture *other) override;
    virtual void OnEndContact(b2Fixture* self, b2Fixture * other) override;
    virtual void setPosition(float x, float y);
    virtual Vector2f getPos();
    ~Character();
    virtual void Begin() = 0;
    virtual void Update(float& deltaTime) = 0;
    virtual void Draw(Renderer& renderer, Resources& resource) = 0;
protected:
    b2Body* dynamicBody = nullptr;
    int changeStateCounter = 0; //3 states (Small, Big, Super) => we will need 3 bodies for each state.
    bool isDead;
    int lives = 3;
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
    Texture drawingTexture{};
    Animation runAnimation;
    Sound jumpSFX{};
public:
    Mario(float x = 1.0f, float y = 1.0f); //set Position, Velocity and JumpVelocity
    void Draw(Renderer& renderer, Resources& resource) override;
    void Begin() override;
    void Update(float& deltaTime) override;
};


class Luigi : public Character {
protected:
public:
    Luigi(float x = 1.0f, float y = 1.0f);
    void Draw(Renderer& renderer, Resources& resource) override;
    void Begin() override;
    void Update(float& deltaTime) override;
};


