#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_draw.h>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
constexpr float PI = 3.14f;
namespace fs = std::filesystem;
using namespace std;
using namespace sf;