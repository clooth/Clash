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

Map::Map() {
    gameState = NULL;
    created = false;
    size = 0;
}

void Map::setGameState(GameState* gs) {
    this->gameState = gs;
}

bool Map::tileUsed(int x, int y) {
    return this->grid[x][y]->getUsed();
}

int Map::createMap(int size) {
    if(!created) {
        this->grid.resize(size);
        std::cout << "Map Generation, Random: ";
        for (int i = 0; i < size; i++) {
            this->grid[i].resize(size);
            for (int j = 0; j < size; j++) {
                int random = rand() % 10;
                std::cout << random;
                if(random == 1 || random == 2)
                    this->grid[i][j] = new TilesWater(i, j);
                else
                    this->grid[i][j] = new TilesGrass(i, j);
            }
        }
        std::cout << std::endl;
        created = true;
    }
    return 0;
}

int Map::importMap(std::string filename) {
    if(!created) {
        std::ifstream map(filename.c_str());
        int tiletype;
        map >> size;
        this->grid.resize(size);
        for (int i = 0; i < size; i++) {
            this->grid[i].resize(size);
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                map >> tiletype;
                switch(tiletype) {
                    case 00:
                        this->grid[j][i] = new TilesGrass(j, i);
                        break;
                    case 01:
                        this->grid[j][i] = new TilesWater(j, i);
                        break;
                    case 02:
                        this->grid[j][i] = new TilesPineWoods(j, i);
                        break;
                    case 03:
                        this->grid[j][i] = new TilesGrass(j, i);
                        this->gameState->getBuildings()->push_back(new Gold(j,i,gameState));
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
