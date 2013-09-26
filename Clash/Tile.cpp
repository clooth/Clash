//
//  Tile.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "Tile.h"

void Tile::setUsed(bool toggle) {
    this->used = toggle;
}

float* Tile::getMColor() {
    return this->mColor;
}

int Tile::getSpeed() const {
    return this->speed;
}

bool Tile::getUsed() const {
    return this->used;
}

int Tile::getType() const {
    return this->type;
}

int Tile::getX() const {
    return this->x;
}

int Tile::getY() const {
    return this->y;
}
int Tile::getID() const {
    return this->ID;
}

TilesGrass::TilesGrass(int x, int y) {
    this->x = x;
    this->y = y;

    this->speed = 1;
    this->type = 1;
    this->used = false;
    this->ID = 0;
    this->mColor[0] = 0.0f;
    this->mColor[1] = 0.6f;
    this->mColor[2] = 0.0f;
}

TilesWater::TilesWater(int x, int y) {
    this->x = x;
    this->y = y;

    this->speed = 0;
    this->type = 2;
    this->used = false;
    this->ID = 1;
    this->mColor[0] = 0.0f;
    this->mColor[1] = 0.0f;
    this->mColor[2] = 0.7f;
}

TilesPineWoods::TilesPineWoods(int x, int y) {
    this->x = x;
    this->y = y;

    this->speed = 0;
    this->type = 3;
    this->used = false;
    this->ID = 2;
    this->mColor[0] = 0.0f;
    this->mColor[1] = 0.4f;
    this->mColor[2] = 0.0f;
}
