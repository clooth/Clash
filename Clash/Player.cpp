//
//  Player.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "Player.h"

Player::Player(int i) {
	// Init of color, should be overwritten by setColor(), but in case
	// it won't, it's more secure to init this array.
	this->color[0] = 0.0f;
	this->color[1] = 0.0f;
	this->color[2] = 0.0f;

	this->id = i;
    this->ressources = new Ressources(this);
}

Player::~Player() {
	delete(ressources);
}

int Player::getID() const {
    return id;
}

Ressources* Player::getRessources() {
    return this->ressources;
}

void Player::setColor(float r, float g, float b) {
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}

float* Player::getColor() {
    return color;
}
