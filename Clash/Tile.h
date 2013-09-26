//
//  Tile.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Tile__
#define __Clash__Tile__

#include <iostream>

/**
 * This class manages single tiles on the map with it's positioning
 * and attributes.
 */
class Tile {
protected:
    /**
     * The speed in which units can walk over this terrain, not yet used
     */
    int speed;

    /**
     * If there's something placed on this terrain, like buildings
     */
    bool used;

    /**
     * Type of this terrain
     * 1 = runnable by everyone
     * 2 = runnable by water units or air
     * 3 = runnable by air units
     */
    int type;

    /**
     * Map-Coordinates of this tile
     */
    int x;

    /**
     * Map-Coordinates of this tile
     */
    int y;

    /**
     * ID of this tile
     */
    int ID;

    /**
     * The color this tile gets on minimap
     */
    float mColor[3];
public:
    /**
     * Gets the speed a unit can run on this tile
     * @return speed
     */
    int getSpeed() const;

    /**
     * Gets if it's something on this tile
     * @return bool
     */
    bool getUsed() const;

    /**
     * Gets runnable type of this tile
     * @return
     */
    int getType() const;

    /**
     * Gets x coordinate on map of this tile
     * @return x coordinate
     */
    int getX() const;

    /**
     * Gets y coordinate on map of this tile
     * @return y coordinate
     */
    int getY() const;

    /**
     * Gets ID of this tile
     * @return id
     */
    int getID() const;

    /**
     * Gets hashcode of color this tile should get on minimap.
     * @return
     */
    float* getMColor();

    /**
     * Setting this tile as used. Is called when something is build here.
     * @param toggle value
     */
    void setUsed(bool toggle);
};

/**
 * Specific tile: Grass
 */
class TilesGrass: public Tile {
public:
    /**
     * Constructor of this specific tile. Sets the attributes for
     * this tile.
     *
     * @param x x coordinate of position on map
     * @param y y coordinate of position on map
     */
    TilesGrass(int x, int y);
};

/**
 * Specific tile: Water
 */
class TilesWater: public Tile {
public:
    /**
     * Constructor of this specific tile. Sets the attributes for
     * this tile.
     *
     * @param x x coordinate of position on map
     * @param y y coordinate of position on map
     */
    TilesWater(int x, int y);
};

/**
 * Specific tile: PineWoods
 */
class TilesPineWoods: public Tile {
public:
    /**
     * Constructor of this specific tile. Sets the attributes for
     * this tile.
     *
     * @param x x coordinate of position on map
     * @param y y coordinate of position on map
     */
    TilesPineWoods(int x, int y);
};


#endif /* defined(__Clash__Tile__) */
