#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "json.hpp"
#include "Tile.hpp"
#include "Enemy.hpp"
#include <unordered_map>
#include <string>
#include <vector>

#ifndef STAGE_HPP
#define STAGE_HPP

class Stage
{
public:
    // Constructs a stage by reading data from a JSON file.
    Stage(const std::string &jsonFile);

    // Loads a texture and stores it in the texture map.
    bool loadTexture(const std::string &textureName, const std::string &filename);

    // Retrieves a texture by name.
    const sf::Texture &getTexture(const std::string &textureName) const;

    // Adds a Tile to the stage.
    void addTile(const Tile &tile);

    // Accessors for stage data.
    std::vector<Tile> &getTiles();
    sf::Vector2f getPlayerSpawn() const;
    sf::Vector2f getGoalPoint() const;
    std::vector<Enemy> &getEnemies();
    std::string getName();

private:
    std::string stageName;
    // Map to hold textures (persist for the stage's lifetime)
    std::unordered_map<std::string, sf::Texture> textures;

    // Tiles for the stage.
    std::vector<Tile> tiles;

    // Additional stage data.
    sf::Vector2f playerSpawn;
    sf::Vector2f goalPoint;
    std::vector<Enemy> enemies;

    // Helper to parse a single tile from JSON.
    Enemy parseEnemy(const nlohmann::json &enemyData);
    Tile parseTile(const nlohmann::json &tileData);
};

#endif // STAGE_HPP
