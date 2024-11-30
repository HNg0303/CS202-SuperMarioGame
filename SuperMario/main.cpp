#include "Character.h"
#include <box2d/box2d.h>
#include <iostream>
using namespace std;
int main() {
    /*
RenderWindow window(VideoMode(800, 600), "Super Mario");

// Create characters
Character* mario = CharacterFactory::createCharacter(MARIO, 100, 300);
//auto luigi = CharacterFactory::createCharacter(LUIGI, 200, 300);

while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();
    }

    // Update and render characters
    window.clear();
    mario->draw(window);
    window.display();
}
delete mario;
return 0;
*/
// Create a Box2D world
    b2Vec2 gravity = { 0.0f, -10.0f };
    b2World world(gravity);
}