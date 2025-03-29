#include "Stage.hpp"
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


sf::Color stringToColor(const std::string& colorName) {
    static const std::unordered_map<std::string, sf::Color> colorMap = {
        {"red", sf::Color::Red},
        {"green", sf::Color::Green},
        {"blue", sf::Color::Blue},
        {"white", sf::Color::White},
        {"black", sf::Color::Black},
        {"yellow", sf::Color::Yellow},
        {"magenta", sf::Color::Magenta},
        {"cyan", sf::Color::Cyan}
    };
    auto it = colorMap.find(colorName);
    if (it != colorMap.end())
        return it->second;
    else
        return sf::Color::White;
}

Stage::Stage(const std::string& jsonFile) {

    constexpr float tileSize = 32.f;
    constexpr float leftmargin = 3 * tileSize;
    constexpr float topmargin = 3 * tileSize;

    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open stage file: " << jsonFile << std::endl;
        return;
    }
    
    json j;
    file >> j;
    
    // Read player spawn point.
    if (j.contains("playerSpawn")) {
        playerSpawn.x = j["playerSpawn"]["x"].get<float>() * tileSize + leftmargin;
        playerSpawn.y = j["playerSpawn"]["y"].get<float>() * tileSize + topmargin;
    }
    
    // Read goal point.
    if (j.contains("goalPoint")) {
        goalPoint.x = j["goalPoint"]["x"].get<float>() * tileSize + leftmargin;
        goalPoint.y = j["goalPoint"]["y"].get<float>() * tileSize + topmargin;
    }
    
// Read enemy spawn locations.
if (j.contains("enemies")) {
    for (auto& enemyData : j["enemies"]) {
        Enemy tempEnemy(
           sf::Vector2f(64, 64),
           sf::Color::Red,
           enemyData["type"].get<int>() == 1
       );
       
tempEnemy.setPos(sf::Vector2f(enemyData["x"].get<float>() * tileSize + leftmargin, enemyData["y"].get<float>() * tileSize + topmargin));

      tempEnemy.setColor(stringToColor(enemyData["color"].get<std::string>()));


       if (enemyData["type"].get<int>() == 1) {
           tempEnemy.setSpeed(100.f);
       }
       else {
           tempEnemy.setDirection(-1.f);
       }

       tempEnemy.setPos(sf::Vector2f(
           enemyData["x"].get<float>() * tileSize,
           enemyData["y"].get<float>() * tileSize
       ));

            if (enemyData["iswall"].get<int>() == 1) {
                tempEnemy.setAsWall();
            }

       tempEnemy.setDetectionRange(250.f);
       // Explicit copy here (copy constructor used automatically):
       enemies.push_back(tempEnemy);

    }
}
    
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

    std::cout << "success parsing " << jsonFile << std::endl;
}

bool Stage::loadTexture(const std::string& textureName, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error: Could not load texture from " << filename << std::endl;
        return false;
    }
    texture.setRepeated(true);
    textures.emplace(textureName, std::move(texture));
    return true;
}

const sf::Texture& Stage::getTexture(const std::string& textureName) const {
    auto it = textures.find(textureName);
    if (it == textures.end()) {
        throw std::runtime_error("Texture " + textureName + " not found.");
    }
    return it->second;
}

void Stage::addTile(const Tile& tile) {
    tiles.push_back(tile);
}

std::vector<Tile>& Stage::getTiles() {
    return tiles;
}

sf::Vector2f Stage::getPlayerSpawn() const {
    return playerSpawn;
}

sf::Vector2f Stage::getGoalPoint() const {
    return goalPoint;
}

std::vector<Enemy>& Stage::getEnemies() {
    return enemies;
}

Tile Stage::parseTile(const nlohmann::json& tileData) {
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

