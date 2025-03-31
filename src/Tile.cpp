#include "Tile.hpp"

Tile::Tile(Texture *texture, Vector2f size, Vector2f position, int t) : collider(body)
{
    type = t;
    body.setSize(size);
    body.setOrigin(size / 2.0f);
    body.setTexture(texture);
    body.setPosition(position);
    Vector2 intposition = Vector2(0, 0);
    Vector2 intsize = Vector2(int(size.x), int(size.y));

    body.setTextureRect(sf::IntRect(intposition, intsize));
}

Tile::~Tile()
{
}

void Tile::render(RenderWindow &window)
{
    window.draw(body);
}

int Tile::getType()
{
    return type;
}