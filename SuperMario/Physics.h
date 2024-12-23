#pragma once
#include "INCLUDE.h"
#include "Renderer.h"

class Character;
class Entity;

class MyDebugDraw : public b2Draw {
private: 
	RenderTarget& target;
public:
    MyDebugDraw(RenderTarget& target);
    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    virtual void DrawTransform(const b2Transform& xf) override;

    /// Draw a point.
    virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

};

class ContactListener {
public:
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
    virtual ~ContactListener() = default;
};

enum class FixtureDataType {
    MapTile,
    Character,
    Entity,
    Enemy
};

struct FixtureData {
    ContactListener* listener = nullptr;
    Entity* entity = nullptr;
    FixtureDataType type;
};

class Physics {
	//b2_world is a world that controls every bodies in this world.
public:
	static MyDebugDraw* debugDraw;
	static void draw(Renderer& renderer);
	static void Init();
	static void Update(float& deltaTime);
	static b2World world; //Static object helps this class act as a mediator to interaction between Mario and Map. 
};


/*
class ContactListener {
public:
	virtual void OnBeginContact() = 0;
	virtual void OnEndContact() = 0;
};*/

