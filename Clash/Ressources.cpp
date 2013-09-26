//
//  Ressources.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <vector>

#include "Ressources.h"
#include "GameState.h"
#include "Unit.h"
#include "Building.h"
#include "Player.h"

Ressources::Ressources(Player* owner) {
    this->owner=owner;
    this->gold=1000;
    this->gas=1000;
    this->currentSupply=0;
    this->maxSupply=0;
}

int Ressources::getGold() const {
    return this->gold;
}

int Ressources::getGas() const {
    return this->gas;
}

int Ressources::getCurrentSupply(GameState* gameState) {
    currentSupply = 0;
    for(unsigned int i=0; i < gameState->getUnits()->size(); i++) {
    	if(gameState->getUnits()->at(i)->getOwner() == owner)
    		currentSupply += gameState->getUnits()->operator [](i)->getSupplyCost();
    }
    return this->currentSupply;
}

int Ressources::getMaxSupply(GameState* gameState) {
    maxSupply = 0;
    for(unsigned int i=0; i < gameState->getBuildings()->size(); i++) {
        if(gameState->getBuildings()->at(i)->buildBuilding() == -1 && gameState->getBuildings()->at(i)->getOwner() == owner)
            maxSupply += gameState->getBuildings()->at(i)->getGivesSupply();
    }
    return this->maxSupply;
}

void Ressources::setGold(int newGold) {
    this->gold = newGold;
}

void Ressources::setGas(int newGas) {
    this->gas = newGas;
}
