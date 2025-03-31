#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "json.hpp"
#include "Tile.hpp"
#include <unordered_map>
#include <string>
#include <vector>

#ifndef UI_HPP
#define UI_HPP

class UI
{
public:
    // Constructs a stage by reading data from a JSON file.
    UI(const std::string &jsonFile);

    // Loads a texture and stores it in the texture map.
    bool loadTexture(const std::string &textureName, const std::string &filename);

    // Retrieves a texture by name.
    const sf::Texture &getTexture(const std::string &textureName) const;

    // Adds a Tile to the stage.
    void addTile(const Tile &tile);

    // Accessors for stage data.
    std::vector<Tile> &getTiles();

private:
    // Map to hold textures (persist for the stage's lifetime)
    std::unordered_map<std::string, sf::Texture> textures;

    // Tiles for the stage.
    std::vector<Tile> tiles;

    // Helper to parse a single tile from JSON.
    Tile parseTile(const nlohmann::json &tileData);
};

#endif // UI_HPP
