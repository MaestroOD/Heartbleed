#include "UI.hpp"
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <vector>

#define tilex 32.f
#define tiley 32.f
#define totalx = 32 * tilex
#define totaly = 25 * tiley

#define origin = sf::Vector2f(0.f,0.f)
#define innerorigin = sf::Vector2f(leftmargin, topmargin)

using json = nlohmann::json;

UI::UI(const std::string& jsonFile) {

    constexpr float tileSize = 32.f;
    constexpr float leftmargin = 0 * tileSize;
    constexpr float topmargin = 0 * tileSize;

    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open UI file: " << jsonFile << std::endl;
        return;
    }
    
    json j;
    file >> j;
        
    // Process tiles.
    if (j.contains("tiles")) {
        for (const auto& tileData : j["tiles"]) {
            // Retrieve the texture name.
            std::string textureName = tileData.value("texture", "");
            // If a texture is specified and not already loaded, load it.
            if (!textureName.empty() && textures.find(textureName) == textures.end()) {
                if (!loadTexture(textureName, textureName)) {
                    std::cerr << "Error loading texture: " << textureName << std::endl;
                }
            }
            // Parse the tile and add it to the stage.
            Tile tile = parseTile(tileData);
            addTile(tile);
        }
    }
}

bool UI::loadTexture(const std::string& textureName, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error: Could not load texture from " << filename << std::endl;
        return false;
    }
    texture.setRepeated(true);
    textures.emplace(textureName, std::move(texture));
    return true;
}

const sf::Texture& UI::getTexture(const std::string& textureName) const {
    auto it = textures.find(textureName);
    if (it == textures.end()) {
        throw std::runtime_error("Texture " + textureName + " not found.");
    }
    return it->second;
}

void UI::addTile(const Tile& tile) {
    tiles.push_back(tile);
}

std::vector<Tile>& UI::getTiles() {
    return tiles;
}

Tile UI::parseTile(const nlohmann::json& tileData) {
    constexpr float tileSize = 32.f;
    constexpr float leftmargin = 3 * tileSize;
    constexpr float topmargin = 3 * tileSize;

    // Convert the JSON "position" (top-left corner in tile units) to pixels.
    float posX = tileData["position"]["x"].get<float>() * tileSize;
    float posY = tileData["position"]["y"].get<float>() * tileSize;

    // Convert the JSON "size" (in tile units) to pixels.
    float width  = tileData["size"]["width"].get<float>() * tileSize;
    float height = tileData["size"]["height"].get<float>() * tileSize;

    // Adjust the position to account for SFML's center-based origin:
    // The center is the top-left plus half the width and height.
    sf::Vector2f centerPosition(leftmargin + posX + width / 2, topmargin + posY + height / 2);
    sf::Vector2f size(width, height);

    // Parse the tile type.
    int type = tileData["type"].get<int>();

    // Retrieve the texture pointer if a texture name is provided.
    std::string textureName = tileData.value("texture", "");
    sf::Texture* texturePtr = nullptr;
    if (!textureName.empty()) {
        try {
            // Note: const_cast is used here to match the expected pointer type in Tile.
            texturePtr = const_cast<sf::Texture*>(&getTexture(textureName));
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            texturePtr = nullptr;
        }
    }

    return Tile(texturePtr, size, centerPosition, type);
}

