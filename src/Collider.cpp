#include "Collider.hpp"

Collider::Collider(sf::RectangleShape &bodyRef)
    : bodyPtr(&bodyRef)
{
}

Collider::Collider(sf::Sprite &bodyRef)
    : spritePtr(&bodyRef)
{
}

Collider::Collider()
    : bodyPtr(nullptr),
      spritePtr(nullptr)
{
}

Collider::~Collider()
{
    // default destructor is fine, or keep empty if needed
}

Collider::Collider(const Collider& other)
{
    this->bodyPtr = other.bodyPtr;
    this->spritePtr = other.spritePtr;
}


Collider& Collider::operator=(const Collider& other)
{
    if (this != &other)
    {
        this->bodyPtr = other.bodyPtr;
        this->spritePtr = other.spritePtr;
    }
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
        // If we’re wrapping a RectangleShape
        return bodyPtr->getSize() / 2.0f;
    }
    else if (spritePtr)
    {
        // If we’re wrapping a Sprite
        // 1) Get the local width/height = 32×18 in your case
        sf::FloatRect local = spritePtr->getLocalBounds(); 

        // 2) Multiply by the sprite’s scale (2×2), so 64×36 overall
        float scaledWidth  = local.size.x  * spritePtr->getScale().x;
        float scaledHeight = local.size.y * spritePtr->getScale().y;

        // 3) Return half
        return {scaledWidth / 2.f, scaledHeight / 2.f};

        //return local.getCenter();
    }
    else
    {
        // If somehow neither pointer is set
        return {0.f, 0.f};
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
    float intersectX = std::abs(deltaX) - (std::abs(otherHalfSize.x) + std::abs(thisHalfSize.x));
    float intersectY = std::abs(deltaY) - (std::abs(otherHalfSize.y) + std::abs(thisHalfSize.y));

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