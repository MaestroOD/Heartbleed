#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Collider
{
public:
    Collider(RectangleShape &body);
    ~Collider();

    void Move(float dx, float dy) { body.move({dx, dy}); };

    bool checkCollision(Collider &other, Vector2f &direction, float push); // Push determines whether the object can be moved or not when collided
    Vector2f getPosition() { return body.getPosition(); };
    Vector2f getHalfSize() { return body.getSize() / 2.0f; };

private:
    RectangleShape &body;
};
