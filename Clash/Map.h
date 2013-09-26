//
//  Map.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Map__
#define __Clash__Map__

#include <iostream>
#include <vector>
#include <string>

#include "Tile.h"

class GameState;

/**
 * Saves game map
 */
class Map {
private:
    /**
     * State of the game object
     */
    GameState* gameState;

    /**
     * Map total size
     */
    int size;

    /**
     * Is set when creating of map is done, either on importing a map or on
     * creating it by randomizer.
     */
    bool created;

    /**
     *  Tile-based grid of the map
     */
    std::vector<std::vector<Tile*> > grid;
public:
    /**
     * Constructor
     */
    Map();

    /**
     * Destructor
     */
    ~Map();

    /**
     * Getting size of the map
     * @return integer
     */
    int getSize() const;

    /**
     * Setting state of the game pointer
     *
     * @param gs state of the game pointer
     */
    void setGameState(GameState* gs);

    /**
     * Creating random map, deprecated
     *
     * @param size size of the map
     * @return zero if done, something else if error
     */
    int createMap(int size);

    /**
     * Importing map from filename
     *
     * @param filename file to import
     * @return zero if done, something else if error
     */
    int importMap(std::string);

    /**
     * Gives back tileId of tile at position of given coordinates
     *
     * @param x x-coordinate of tile
     * @param y y-coordinate of tile
     * @return tileid
     */
    int tileID(int x, int y);

    /**
     * Gives back minimap color of tile at position of given coordinates
     *
     * @param x x-coordinate of tile
     * @param y y-coordinate of tile
     * @return minimap color of tile
     */
    float* tileColor(int x, int y);

    /**
     * Gives back tileType of tile at position of given coordinates
     *
     * @param x x-coordinate of tile
     * @param y y-coordinate of tile
     * @return tileType
     */
    int tileType(int x, int y);

    /**
     * Gives back boolean value if tile at position of given coordinates
     * is in use (build on by a building) or not
     *
     * @param x x-coordinate of tile
     * @param y y-coordinate of tile
     * @return boolean if something is build on this tile
     */
    bool tileUsed(int x, int y);

    /**
     * Setting a tile build or not build. Called by Building class when
     * buildings get build or destroyed.
     *
     * @param x x-coordinate of tile
     * @param y y-coordinate of tile
     * @param toggle boolean value if used or not
     * @return pointer to tile which is changed
     */
    Tile* setTile(int x, int y, bool toggle);
};

#endif /* defined(__Clash__Map__) */
