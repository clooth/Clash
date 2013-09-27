//
//  Map.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <sstream>
#include <fstream>
#include <cstdlib>

#include "Map.h"
#include "GameState.h"
#include "Building.h"

namespace Clash
{
    /**
     * Default constructor
     */
    Map::Map()
    : m_game_state(NULL)
    , m_created(false)
    , m_size(0)
    {
    }

    /**
     * Game state setter
     *
     * @param game_state state of the game pointer
     */
    void Map::set_game_state(GameState* game_state) {
        m_game_state = game_state;
    }

    bool Map::is_tile_used(int x, int y) {
        return m_tile_grid[x][y]->get_in_use();
    }

    /**
     * Importing map from filename
     *
     * @param filename file to import
     * @return zero if done, something else if error
     */
    bool Map::import_map_from_file(std::string filename) {
        if (!m_created)
        {
            int tiletype;

            std::ifstream map(filename.c_str());
            map >> m_size;
            m_tile_grid.resize(m_size);

            for (int i = 0; i < m_size; i++) {
                m_tile_grid[i].resize(m_size);
            }

            for (int i = 0; i < m_size; i++) {
                for (int j = 0; j < m_size; j++) {
                    map >> tiletype;
                    switch(tiletype) {
                        case 00:
                            m_tile_grid[j][i] = new MapTileGrass(j, i);
                            break;
                        case 01:
                            m_tile_grid[j][i] = new TilesWater(j, i);
                            break;
                        case 02:
                            m_tile_grid[j][i] = new TilesPineWoods(j, i);
                            break;
                        case 03:
                            m_tile_grid[j][i] = new TilesGrass(j, i);
                            m_game_state->getBuildings()->push_back(new Gold(j,i,gameState));
                            break;
                    }
                }
            }
            created = true;
        }
        return 0;
    }

    int Map::tileID(int x, int y) {
        return this->grid[x][y]->getID();
    }

    int Map::tileType(int x, int y) {
        return this->grid[x][y]->getType();
    }

    float* Map::tileColor(int x, int y) {
        return this->grid[x][y]->getMColor();
    }
    
    Tile* Map::setTile(int x, int y, bool toggle) {
        this->grid[x][y]->setUsed(toggle);
        return grid[x][y];
    }
    
    Map::~Map() {
        for(unsigned int i=0; i<grid.size();i++) {
            for(unsigned int j=0; j<grid[i].size();j++) {
                delete(grid[i][j]);
            }
        }
    }
    
    int Map::getSize() const {
        return this->size;
    }

}