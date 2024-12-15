#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
constexpr float PI = 3.14f;
namespace fs = std::filesystem;
using namespace std;
using namespace sf;