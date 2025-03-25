#include "Collider.hpp"

Collider::Collider(sf::RectangleShape &bodyRef)
    : bodyPtr(&bodyRef)
{
}

Collider::Collider(sf::Sprite &bodyRef)
    : spritePtr(&bodyRef)
{
}

Collider::~Collider()
{
    // default destructor is fine, or keep empty if needed
}

// Copy constructor
Collider::Collider(const Collider &other)
{
    // copy logic
    this->bodyPtr = other.bodyPtr;
}

// Assignment operator
Collider &Collider::operator=(const Collider &other)
{
    if (this != &other)
        this->bodyPtr = other.bodyPtr;
    return *this;
}

void Collider::Move(float dx, float dy)
{
    if (bodyPtr)
    {
        bodyPtr->move({dx, dy});
    }
    else
    {
        spritePtr->move({dx, dy});
    }
}

Vector2f Collider::getPosition() const
{
    if (bodyPtr)
    {
        return bodyPtr->getPosition();
    }
    else
    {
        return spritePtr->getPosition();
    }
}

Vector2f Collider::getHalfSize() const
{
    if (bodyPtr)
    {
        return bodyPtr->getSize() / 2.0f;
    }
    else
    {
        return {32, 32}; // constant
    }
}

bool Collider::checkCollision(Collider &other, sf::Vector2f &direction, float push)
{
    sf::Vector2f otherPosition = other.getPosition();
    sf::Vector2f otherHalfSize = other.getHalfSize();
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisHalfSize = getHalfSize();

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;
    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.f && intersectY < 0.f)
    {
        push = std::min(std::max(push, 0.0f), 1.0f);

        if (intersectX > intersectY)
        {
            if (deltaX > 0.f)
            {
                Move(intersectX * (1.f - push), 0.f);
                other.Move(-intersectX * push, 0.f);

                direction.x = 1.f;
                direction.y = 0.f;
            }
            else
            {
                Move(-intersectX * (1.f - push), 0.f);
                other.Move(intersectX * push, 0.f);

                direction.x = -1.f;
                direction.y = 0.f;
            }
        }
        else
        {
            if (deltaY > 0.f)
            {
                Move(0.f, intersectY * (1.f - push));
                other.Move(0.f, -intersectY * push);

                direction.x = 0.f;
                direction.y = 1.f;
            }
            else
            {
                Move(0.f, -intersectY * (1.f - push));
                other.Move(0.f, intersectY * push);

                direction.x = 0.f;
                direction.y = -1.f;
            }
        }
        return true;
    }

    return false;
}
