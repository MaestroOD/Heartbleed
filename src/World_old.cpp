#include "World.hpp"

#include <vector>

#define tilew 32.f
#define tileh 32.f

std::vector<std::vector<Tile>> generateWorld()
{

    std::vector<std::vector<Tile>> gametiles;

    /* 	nullptr nullptr;
        if (!nullptr.loadFromFile("images/tile-wires.png")) {
            std::cerr << "Error loading in tile nullptr" << std::endl;
        } */

    // -----------------------------------------
    // Stage 1
    // -----------------------------------------

    std::vector<Tile> stage1;
    Tile s1wall1(nullptr, sf::Vector2f(100.f, 300.f), sf::Vector2f(500.f, 180.f), 0);
    Tile s1platform1(nullptr, sf::Vector2f(600.f, 50.f), sf::Vector2f(320.f, 320.f), 1);
    Tile s1platform2(nullptr, sf::Vector2f(50.f, 5.f), sf::Vector2f(320.f, 200.f), 0);
    Tile s1platform3(nullptr, sf::Vector2f(32.f, 32.f), sf::Vector2f(40.f, 40.f), 2);

    stage1.emplace_back(s1wall1);
    stage1.emplace_back(s1platform1);
    stage1.emplace_back(s1platform2);
    stage1.emplace_back(s1platform3);

    // -----------------------------------------
    // Stage 2(nullptr
    // -----------------------------------------

    std::vector<Tile> stage2;
    Tile s2wall1(nullptr, sf::Vector2f(100.f, 300.f), sf::Vector2f(500.f, 180.f), 0);
    Tile s2platform1(nullptr, sf::Vector2f(600.f, 50.f), sf::Vector2f(320.f, 320.f), 1);
    Tile s2platform2(nullptr, sf::Vector2f(60.f, 6.f), sf::Vector2f(320.f, 200.f), 0);
    stage2.emplace_back(s2wall1);
    stage2.emplace_back(s2platform1);
    stage2.emplace_back(s2platform2);

    // -----------------------------------------
    // Stage 3
    // -----------------------------------------

    std::vector<Tile> stage3;
    Tile s3wall1(nullptr, sf::Vector2f(100.f, 300.f), sf::Vector2f(500.f, 180.f), 0);
    Tile s3platform1(nullptr, sf::Vector2f(600.f, 50.f), sf::Vector2f(320.f, 320.f), 1);
    Tile s3platform2(nullptr, sf::Vector2f(70.f, 7.f), sf::Vector2f(320.f, 200.f), 0);
    stage3.emplace_back(s3wall1);
    stage3.emplace_back(s3platform1);
    stage3.emplace_back(s3platform2);

    // -----------------------------------------
    // Stage 4: Zig-Zag Downward Platforms
    // -----------------------------------------

    std::vector<Tile> stage4;

    // Left boundary so player can’t leave the left edge (x=0..50)
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(50.f, 800.f), // width=50, height=800
        sf::Vector2f(25.f, 400.f), // center at (25,400)
        0                          // normal wall
        ));

    // A few “stepped” platforms going downward:
    // 1) near y=200 (width=300), center x=200
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(300.f, 20.f),
        sf::Vector2f(200.f, 200.f),
        0 // normal platform
        ));
    // 2) near y=350 (width=300), center x=350
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(300.f, 20.f),
        sf::Vector2f(350.f, 350.f),
        0));
    // 3) near y=500 (width=300), center x=200
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(300.f, 20.f),
        sf::Vector2f(200.f, 500.f),
        0));
    // 4) near y=650 (width=300), center x=350
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(300.f, 20.f),
        sf::Vector2f(350.f, 650.f),
        0));

    // Big floor at bottom: y=775, from x=0..1000
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(1000.f, 50.f),
        sf::Vector2f(500.f, 775.f),
        1 // ground
        ));

    // Goal tile in bottom-right corner, type=2 => no collision
    stage4.emplace_back(Tile(
        nullptr,
        sf::Vector2f(40.f, 40.f),
        sf::Vector2f(980.f, 780.f),
        2));

    // -----------------------------------------
    // Stage 5: Simple “Downward” Corridor
    // -----------------------------------------

    std::vector<Tile> stage5;

    // Left boundary
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(50.f, 800.f),
        sf::Vector2f(25.f, 400.f),
        0));

    // A few horizontal platforms at different Y-levels:
    // Each is narrower than the full width so the player can move around edges to fall down.
    // Platform near y=200, center x=350, width=400
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(400.f, 20.f),
        sf::Vector2f(350.f, 200.f),
        0));
    // Platform near y=400
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(400.f, 20.f),
        sf::Vector2f(350.f, 400.f),
        0));
    // Platform near y=600
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(400.f, 20.f),
        sf::Vector2f(350.f, 600.f),
        0));

    // Floor at bottom
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(1000.f, 50.f),
        sf::Vector2f(500.f, 775.f),
        1 // ground
        ));

    // Goal tile near bottom-right
    stage5.emplace_back(Tile(
        nullptr,
        sf::Vector2f(40.f, 40.f),
        sf::Vector2f(980.f, 780.f),
        2 // no collision
        ));
    /////////////////////////////
    // Spencer's Tutorial Stage
    /////////////////////////////

    std::vector<Tile> TutorialLevel;
    // total game window size
    float xwidth = 32 * tilew;
    float ywidth = 25 * tileh;
    // inner game working area margins
    float innerx_min = 3 * tilew;
    float innery_min = 3 * tileh;
    float innerx_max = xwidth - innerx_min;
    float innery_max = ywidth - innery_min;

    // this block is the frame size in which all game elements should be contained
    // left wall
    float horizontalwall_w = 25 * tilew;
    float horizontalwall_h = 1 * tileh;

    float verticalwall_w = 1 * tileh;
    float verticalwall_h = 19 * tilew;

    Tile TutWall1(nullptr, sf::Vector2f(verticalwall_w, verticalwall_h), sf::Vector2f(innerx_min + verticalwall_w / 2, innery_min + verticalwall_h / 2), 0);
    // top wall
    Tile TutWall2(nullptr, sf::Vector2f(verticalwall_w, verticalwall_h), sf::Vector2f(innerx_max - verticalwall_w / 2, innery_max - verticalwall_h / 2), 0);
    // right wall
    Tile TutWall3(nullptr, sf::Vector2f(horizontalwall_w, horizontalwall_h), sf::Vector2f(innerx_min + horizontalwall_w / 2, innery_min + horizontalwall_h / 2), 0);
    // bottom wall
    Tile TutWall4(nullptr, sf::Vector2f(horizontalwall_w, horizontalwall_h), sf::Vector2f(innerx_max - horizontalwall_w / 2, innery_max - horizontalwall_h / 2), 0);

    TutorialLevel.push_back(TutWall1);
    TutorialLevel.push_back(TutWall2);
    TutorialLevel.push_back(TutWall3);
    TutorialLevel.push_back(TutWall4);
    // above block is basic framework for level building

    float threetileplat_w = 3 * tilew;
    float threetileplat_h = 1 * tileh;

    Tile TutPlat1(nullptr, sf::Vector2f(threetileplat_w, threetileplat_h), sf::Vector2f(innerx_min + tileh + threetileplat_w / 2, innery_max - 3 * tileh - threetileplat_h / 2), 0);
    TutorialLevel.push_back(TutPlat1);

    Tile TutPlat2(nullptr, sf::Vector2f(threetileplat_w, threetileplat_h), sf::Vector2f(innerx_min + 3 * tileh + threetileplat_w / 2, innery_max - 6 * tileh - threetileplat_h / 2), 0);
    TutorialLevel.push_back(TutPlat2);

    Tile TutPlat3(nullptr, sf::Vector2f(threetileplat_w, threetileplat_h), sf::Vector2f(innerx_min + tileh + threetileplat_w / 2, innery_max - 9 * tileh - threetileplat_h / 2), 0);
    TutorialLevel.push_back(TutPlat3);

    Tile TutPlat4(nullptr, sf::Vector2f(threetileplat_w, threetileplat_h), sf::Vector2f(innerx_min + 3 * tileh + threetileplat_w / 2, innery_max - 12 * tileh - threetileplat_h / 2), 0);
    TutorialLevel.push_back(TutPlat4);

    Tile TutPlat5(nullptr, sf::Vector2f(threetileplat_w, threetileplat_h), sf::Vector2f(innerx_min + tileh + threetileplat_w / 2, innery_max - 15 * tileh - threetileplat_h / 2), 0);
    TutorialLevel.push_back(TutPlat5);

    float centerwall_w = 2 * tilew;
    float centerwall_h = 14 * tileh;

    Tile TutCenterWall(nullptr, sf::Vector2f(centerwall_w, centerwall_h), sf::Vector2f((innerx_min + innerx_max) / 2, innery_min + centerwall_h / 2), 0);
    TutorialLevel.push_back(TutCenterWall);

    gametiles.push_back(TutorialLevel);
    gametiles.push_back(stage4);
    gametiles.push_back(stage5);
    gametiles.push_back(stage1);
    gametiles.push_back(stage2);
    gametiles.push_back(stage3);

    return gametiles;
}