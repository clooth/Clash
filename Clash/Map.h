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

#include "MapTile.h"

class GameState;

namespace Clash
{
    /**
     * Game map representaiton
     */
    class Map
    {
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
        int get_size() const;

        /**
         * Game state setter
         *
         * @param game_state state of the game pointer
         */
        void set_game_state(GameState* game_state);

        /**
         * Importing map from filename
         *
         * @param filename file to import
         * @return true if successful, false if not
         */
        bool import_map_from_file(std::string filename);

        /**
         * Gives back tileId of tile at position of given coordinates
         *
         * @param x x-coordinate of tile
         * @param y y-coordinate of tile
         * @return tileid
         */
        int tile_id(int x, int y);

        /**
         * Gives back minimap color of tile at position of given coordinates
         *
         * @param x x-coordinate of tile
         * @param y y-coordinate of tile
         * @return minimap color of tile
         */
        float* tile_color(int x, int y);

        /**
         * Gives back tileType of tile at position of given coordinates
         *
         * @param x x-coordinate of tile
         * @param y y-coordinate of tile
         * @return tileType
         */
        int tile_type(int x, int y);

        /**
         * Gives back boolean value if tile at position of given coordinates
         * is in use (build on by a building) or not
         *
         * @param x x-coordinate of tile
         * @param y y-coordinate of tile
         * @return boolean if something is build on this tile
         */
        bool is_tile_used(int x, int y);

        /**
         * Setting a tile build or not build. Called by Building class when
         * buildings get build or destroyed.
         *
         * @param x x-coordinate of tile
         * @param y y-coordinate of tile
         * @param toggle boolean value if used or not
         * @return pointer to tile which is changed
         */
        MapTile* set_tile(int x, int y, bool toggle);

    private:
        /**
         * Active game state
         */
        GameState* m_game_state;

        /**
         * Map total size
         */
        int m_size;

        /**
         * Is set when creating of map is done, either on importing a map or on
         * creating it by randomizer.
         */
        bool m_created;

        /**
         *  Tile-based grid of the map
         */
        std::vector<std::vector<MapTile*>> m_tile_grid;
    };
}
#endif /* defined(__Clash__Map__) */
