#include "Tile.hpp"

Tile::Tile(Texture *texture, Vector2f size, Vector2f position, int t) : collider(body)
{
    type = t;
    body.setSize(size);
    body.setOrigin(size / 2.0f);
    body.setTexture(texture);
    body.setPosition(position);
}

Tile::~Tile()
{
}

void Tile::render(RenderWindow &window)
{
    window.draw(body);
}
