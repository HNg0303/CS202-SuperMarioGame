#pragma once
#include "../Headers/INCLUDE.h"
#include "../Headers/Renderer.h"
#include "../Headers/Resources.h"
#include "../Headers/Physics.h"
#include "../Headers/entity.h"
#include "../Headers/Animation.h"

enum CharacterType { MARIO, LUIGI, CustomCharacter };

class Character : public ContactListener {
public:
    Vector2f position;
    bool win = false;
    float yBound = 0.0f;
    pair<float, float> xBound;
    int lives = 3;
    int coin = 0;
    float transformTimer = 0.0f;


    void handleDeath();
    void setPos(Vector2f position);
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
    virtual Vector2f getPos();
    ~Character();
    virtual void Begin() = 0;
    virtual void Update(float& deltaTime) = 0;
    virtual void Draw(Renderer& renderer) = 0;
    bool isDead = false;
protected:
    int changeStateCounter = 0; //3 states (Small, Big, Super) => we will need 3 bodies for each state.
    bool faceLeft = 0;
    int onGround = 0;
    bool transform = false;
    float movementVelocity;
    float jumpVelocity;
    float angle;
    sf::Vector2f startPos{};

    FixtureData* fixtureData = nullptr;
    b2Body* dynamicBody = nullptr;
    b2Fixture* groundFixture;
    b2Fixture* headFixture;

    Texture drawingTexture{};
    Texture standAnimation{}, jumpAnimation{};
    Animation runAnimation, changeStateAnimation;
    Sound jumpSFX{};
    Sound soundEffect{};
};


class CharacterFactory {
public:
    static Character* createCharacter(CharacterType type, int lives = 3);
};


class Mario : public Character {
public:
    Mario(float x = 1.0f, float y = 1.0f, int lives = 3); //set Position, Velocity and JumpVelocity
    void Draw(Renderer& renderer) override;
    void Begin() override;
    void Update(float& deltaTime) override;
    void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
};


class Luigi : public Character {
public:
    Luigi(float x = 1.0f, float y = 1.0f, int lives = 3);
    void Draw(Renderer& renderer) override;
    void Begin() override;
    void Update(float& deltaTime) override;
    void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
};

