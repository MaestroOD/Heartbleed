#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.hpp"
#include <iostream>

using namespace sf;

class Tile
{
public:
    Tile(Texture *texture, Vector2f size, Vector2f position, int t);
    ~Tile();

    void render(RenderWindow &window);
    Collider &getCollider() { return collider; };

private:
    int type; // 0 = Normal platform type, 1 = Ground, 2 = No collision
    RectangleShape body;
    Collider collider;
};
