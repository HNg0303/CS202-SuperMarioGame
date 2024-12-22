#include "Physics.h"


b2World Physics::world{ b2Vec2(0.0f, 9.8f) };
MyDebugDraw* Physics::debugDraw{};

MyDebugDraw::MyDebugDraw(RenderTarget& target) : target(target) {};

void MyDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
    CircleShape circle(size);
    circle.setPosition(p.x, p.y);
    circle.setOrigin(size / 2.0f, size / 2.0f);
    circle.setFillColor(Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    target.draw(circle);
}

void MyDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
    CircleShape circle(radius);
    circle.setPosition(center.x, center.y);
    circle.setOrigin(radius / 2.0f, radius / 2.0f);
    circle.setFillColor(Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    target.draw(circle);

    b2Vec2 p = center + (radius * axis);
    DrawSegment(center, p, color);
}

void MyDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
    CircleShape circle(radius);
    circle.setPosition(center.x, center.y);
    circle.setOrigin(radius / 2.0f, radius / 2.0f);
    circle.setFillColor(Color::Transparent);
    circle.setOutlineThickness(0.02f);
    circle.setOutlineColor(Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    target.draw(circle);
}

void MyDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    ConvexShape shape(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i) {
        shape.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));
    }
    shape.setFillColor(Color::Transparent);
    shape.setOutlineThickness(0.02f);
    shape.setOutlineColor(Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    target.draw(shape);
}

void MyDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    VertexArray va(Lines, 2);
    Color sfColor((color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    va[0].position = Vector2f(p1.x, p1.y);
    va[0].color = sfColor;

    va[1].position = Vector2f(p2.x, p2.y);
    va[1].color = sfColor;
    target.draw(va);
}

void MyDebugDraw::DrawTransform(const b2Transform& xf) {
    b2Vec2 p = xf.p;
    b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
    b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

    DrawSegment(p, px, b2Color(1.0f, 0.0f, 0.0f));
    DrawSegment(p, py, b2Color(0.0f, 1.0f, 0.0f));
}

void MyDebugDraw :: DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    ConvexShape shape(vertexCount);
    for (size_t i = 0; i < vertexCount; ++i) {
        shape.setPoint(i, Vector2f(vertices[i].x, vertices[i].y));
    }
    shape.setFillColor(Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
    target.draw(shape);
}



class myGlobalContact : public b2ContactListener {

    virtual void BeginContact(b2Contact* contact) {
        FixtureData* data = reinterpret_cast<FixtureData*> (contact->GetFixtureA()->GetUserData().pointer);

        if (data && data->listener) {
            data->listener->OnBeginContact(contact->GetFixtureA(), contact->GetFixtureB());
        }


        data = reinterpret_cast<FixtureData*> (contact->GetFixtureB()->GetUserData().pointer);
        if (data && data->listener) {
            data->listener->OnBeginContact(contact->GetFixtureB(), contact->GetFixtureA());
        }
    }

    /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact) {
        
        FixtureData* data = reinterpret_cast<FixtureData*> (contact->GetFixtureA()->GetUserData().pointer);

        if (data && data->listener) {
            data->listener->OnEndContact(contact->GetFixtureA(), contact->GetFixtureB());
        }

        data = reinterpret_cast<FixtureData*> (contact->GetFixtureB()->GetUserData().pointer);
        if (data && data->listener) {
            data->listener->OnEndContact(contact->GetFixtureB(), contact->GetFixtureA());
        }
    }
};

void Physics::Init() {
    world.SetContactListener(new myGlobalContact());
}

void Physics::Update(float& deltaTime) {
    world.Step(deltaTime , 8, 3);
}

void Physics::draw(Renderer& renderer) {
    if (!debugDraw) {
        debugDraw = new MyDebugDraw(renderer.target);
        debugDraw->SetFlags(0u);
        world.SetDebugDraw(debugDraw);
    }
    world.DebugDraw();
}

