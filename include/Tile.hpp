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

    Tile(const Tile &other)
        : body(other.body) // Copy the RectangleShape
          ,
          collider(body) // Re-bind collider to this Tile's body
          ,
          type(other.type)
    {
        // If you had a Texture* member, you'd copy it or handle ownership here
    }

    Tile &operator=(const Tile &other)
    {
        if (this != &other)
        {
            body = other.body;
            type = other.type;

            // Re-initialize collider to reference our 'body' instead of 'other.body'
            collider = Collider(body);
        }
        return *this;
    }

    void render(RenderWindow &window);
    Collider &getCollider() { return collider; };

private:
    int type; // 0 = Normal platform type, 1 = Ground, 2 = No collision
    RectangleShape body;
    Collider collider;
};
