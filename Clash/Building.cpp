//
//  Building.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <vector>

#include "Building.h"
#include "GameState.h"
#include "Timer.h"
#include "SDL_Functions.h"
#include "Map.h"
#include "Ressources.h"
#include "Unit.h"
#include "Player.h"
#include "Tooltip.h"
#include "GameData.h"

void CommandCenter::doAction(int clickX, int clickY) {
    clickX-=10;
    clickY-=10;
    if(clickX >= 0 && clickX <= 55 && clickY >= 0 && clickY <= 55) {
        placeUnit(42);
    }
}

void CommandCenter::doMotion(int clickX, int clickY) {
    clickX-=10;
    clickY-=10;
    if(clickX >= 0 && clickX <= 55 && clickY >= 0 && clickY <= 55) {
        this->gameState->setActiveTooltip(tooltips[0]);
    }
}

void Gold::doAction(int clickX, int clickY) { }
void Gold::doMotion(int clickX, int clickY) { }

void SupplyDepot::doAction(int clickX, int clickY) { }
void SupplyDepot::doMotion(int clickX, int clickY) { }

void Factory::doAction(int clickX, int clickY) { }
void Factory::doMotion(int clickX, int clickY) { }

void MilAirport::doAction(int clickX, int clickY) { }
void MilAirport::doMotion(int clickX, int clickY) { }

void ResearchLab::doAction(int clickX, int clickY) { }
void ResearchLab::doMotion(int clickX, int clickY) { }

void Tower::doAction(int clickX, int clickY) { }
void Tower::doMotion(int clickX, int clickY) { }

void Barracks::doAction(int clickX, int clickY) {
    clickX-=10;
    clickY-=10;
    if(clickX >= 0 && clickX <= 55 && clickY >= 0 && clickY <= 55) {
        placeUnit(41);
    }
}

void Barracks::doMotion(int clickX, int clickY) {
    clickX-=10;
    clickY-=10;
    if(clickX >= 0 && clickX <= 55 && clickY >= 0 && clickY <= 55) {
        this->gameState->setActiveTooltip(tooltips[0]);
    }
}

void Building::createBuilding(int x, int y) {
    for(int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            gameState->getMap()->setTile(x+i,y+j,true);
        }
    }
}

int Building::buildUnit() {
	bool help = true;
	while(help) {
		if(gameState->getUnits()->size() == 0) help = false;
		for(unsigned int k = 0; k < gameState->getUnits()->size(); k++) {
            int startUnitX = gameState->getUnits()->at(k)->getX();
            int startUnitY = gameState->getUnits()->at(k)->getY();
            int endUnitX = startUnitX + gameState->getUnits()->at(k)->getSize()*64;
            int endUnitY = startUnitY + gameState->getUnits()->at(k)->getSize()*64;

            if(!(this->unitBuild.front().positionX >= startUnitX &&
                 this->unitBuild.front().positionX <= endUnitX &&
                 this->unitBuild.front().positionY >= startUnitY &&
                 this->unitBuild.front().positionY <= endUnitY)) {
                help = false;
            } else {
                this->unitBuild.front().positionX+=40;
                break;
            }
		}
	}

    if(!this->unitBuild.empty()) {
        if(gameState->getTimer()->get_ticks()-this->unitBuild.front().startTime >= this->unitBuild.front().buildTime) {
        	Unit* unit = NULL;
            switch(this->unitBuild.front().id) {
                case 41:
                    unit = new Soldier(this->unitBuild.front().positionX,this->unitBuild.front().positionY,gameState, owner);
                    break;
                case 42:
                    unit = new Worker(this->unitBuild.front().positionX,this->unitBuild.front().positionY,gameState, owner);
                    break;
            }
            if(waypoint[0] != -1 && waypoint[1] != -1) {
            	unit->moveTo(waypoint[0], waypoint[1]);
            }
            gameState->getUnits()->push_back(unit);
            this->unitBuild.pop();
            this->unitBuild.front().startTime = gameState->getTimer()->get_ticks();
            return 1;
        }
    }
    return 0;
}

int Building::getGivesSupply() {
    return this->givesSupply;
}

int Building::placeUnit(int id) {
    // Placing unit next to other units around
    int absX = this->x*64 + (64*this->size)/2;
    int absY = this->y*64 + (64*this->size);

    // Checking if all ressources are available before placing that unit.
    if(     gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(id) &&
       gameState->getPlayers()->operator [](0)->getRessources()->getGas() >= gameState->getData()->getGasPrices(id) &&
       gameState->getPlayers()->operator [](0)->getRessources()->getMaxSupply(gameState) >= gameState->getData()->getSupply(id) + gameState->getPlayers()->operator [](0)->getRessources()->getCurrentSupply(gameState)) {
        //gameState->getUnits()->push_back(new Soldier(absX,absY,gameState));
        BuildUnit bu;
        bu.id = id;
        bu.positionX = absX;
        bu.positionY = absY;
        bu.buildTime = gameState->getData()->getBuildTimes(id);
        bu.startTime = gameState->getTimer()->get_ticks();
        this->unitBuild.push(bu);

        gameState->getPlayers()->operator [](0)->getRessources()->setGold(gameState->getPlayers()->operator [](0)->getRessources()->getGold()-gameState->getData()->getGoldPrices(id));
        gameState->getPlayers()->operator [](0)->getRessources()->setGas(gameState->getPlayers()->operator [](0)->getRessources()->getGas()-gameState->getData()->getGasPrices(id));
    } else {
        gameState->setAnnounceText("You don't have enough ressources.");
        gameState->updateAnnounce();
    }

    return 0;
}

bool Building::isBuildingUnit() {
    return !(this->unitBuild.empty());
}

void Building::initBuilding(GameState* gameState) {
    // Standard initializing stuff
    this->gameState = gameState;
    createBuilding(x,y);

    tooltips.resize(1);

    // Build this Building
    this->inBuild = true;
    this->startBuild = gameState->getTimer()->get_ticks();

    // Standard color is black. Should never be used but
    // is set here so the variables aren't uninitialized.
    this->mcolor[0] = 0.0f;
    this->mcolor[1] = 0.0f;
    this->mcolor[2] = 0.0f;

    // No waypoint equals -1,-1
    this->waypoint[0] = -1;
    this->waypoint[1] = -1;
}

float Building::buildBuilding() {
    if(inBuild) {
        if(gameState->getTimer()->get_ticks()-this->startBuild >= this->buildTime) {
            inBuild = false;
        }
        return (((gameState->getTimer()->get_ticks()-this->startBuild)/this->buildTime)*100);
    }
    if(dead) {
        return -2;
    }
    return -1;
}

float* Building::getMColor() {
    if(owner == NULL)
        return mcolor;
    return owner->getColor();
}

void Building::setWaypoint(int mouseX, int mouseY) {
	this->waypoint[0] = mouseX;
	this->waypoint[1] = mouseY;
}

int* Building::getWaypoint() {
	return waypoint;
}


CommandCenter::CommandCenter(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 21;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;

    if(owner == gameState->getPlayers()->at(0)) {
        tooltips[0] = new Tooltip(42, gameState);
    }
}

Barracks::Barracks(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 22;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
    this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
    if(owner == gameState->getPlayers()->at(0)) {
        tooltips[0] = new Tooltip(41, gameState);
    }
}

SupplyDepot::SupplyDepot(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 23;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
}

ResearchLab::ResearchLab(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 24;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
}

Factory::Factory(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 25;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
}

MilAirport::MilAirport(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 26;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
}

Tower::Tower(int x, int y, GameState* gameState, Player* owner, bool instant) {
    this->owner = owner;
    this->x = x;
    this->y = y;
    this->ID = 27;
    this->size = gameState->getData()->getSize(ID);
    this->buildTime = gameState->getData()->getBuildTimes(ID);
    this->life = gameState->getData()->getLife(ID);
	this->currentLife = life;
    this->givesSupply = gameState->getData()->getSupply(ID);
    this->dead = false;

    initBuilding(gameState);
    inBuild = instant;
}

Gold::Gold(int x, int y, GameState* gameState) {
    this->owner = NULL;
    this->size = 1;
    this->x = x;
    this->y = y;
    this->ID = 3;
    this->buildTime = 0;
    this->life = 2000;
    this->currentLife = 2000;
    this->givesSupply = 0;
    this->dead = false;

    initBuilding(gameState);
    this->mcolor[0] = 0.5f;
    this->mcolor[1] = 0.3f;
    this->mcolor[2] = 0.0f;
}


Building::~Building() {
    for(int i=0; i < size; i++) {
        for(int j=0; j < size; j++) {
            gameState->getMap()->setTile(x+i,y+j,false);
        }
    }

    for(unsigned int i=0; i<tooltips.size();i++) delete(tooltips.at(i));
}

CommandCenter::~CommandCenter() {}
Barracks::~Barracks() {}
Gold::~Gold() {}
SupplyDepot::~SupplyDepot() {}
Tower::~Tower() {}
Factory::~Factory() {}
MilAirport::~MilAirport() {}
ResearchLab::~ResearchLab() {}