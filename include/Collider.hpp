#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Collider
{
public:
    Collider(RectangleShape &bodyRef);
    Collider(Sprite &bodyRef); // For player and enemies with sprite sheets
    ~Collider();               // We'll define it in the .cpp

    // Copy & assignment
    Collider(const Collider &other);
    Collider &operator=(const Collider &other);

    bool checkCollision(Collider &other, Vector2f &direction, float push);
    void Move(float dx, float dy);

    Vector2f getPosition() const;
    Vector2f getHalfSize() const;

private:
    RectangleShape *bodyPtr;
    Sprite *spritePtr;
};
