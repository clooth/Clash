//
//  Unit.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <queue>
#include <algorithm>
#include <vector>
#include <cmath>

#include "Unit.h"
#include "Node.h"
#include "GameState.h"
#include "Map.h"
#include "Building.h"
#include "Player.h"
#include "Ressources.h"
#include "Timer.h"
#include "GameData.h"
#include "Particle.h"
#include "SDL_Functions.h"
#include "AStarPath.h"

int Unit::getOrientation() {
	return this->orientation;
}

int Unit::getMovementX() {
    return this->movementX;
}

int Unit::getMovementY() {
    return this->movementY;
}

int Unit::newMove() {
	this->distance = 0;

	this->moving = false;
	this->attackB = false;
	this->working = false;

	this->movementX = -1;
	this->movementY = -1;

	return 0;
}

int Unit::moveTo(int x,int y, bool workerConst, bool attackConst, int distance) {
    // If currently attacking, we want to cancel that, if user triggers movement.
    // The argument attackConst will be set by attack method if the moveTo call
    // is triggered by it, so the attack won't cancel.
    if(attackConst) {
        this->attackB = false;
        this->attackedObject = 0;
    }

    // If currently working, we want to cancel that, if user triggers movement.
    // The argument workerConst will be set by work method if the moveTo call
    // is triggered by it, so the attack won't cancel.
    if(workerConst) {
        this->working = false;
        this->workX = 0;
        this->workY = 0;
    }

    this->movementX = x;
	this->movementY = y;

    // This one checks if another unit is at the selected block. If so, this unit
    // can not move.
	/*for(unsigned int k = 0; k < gameState->getUnits()->size(); k++) {
     if(gameState->getUnits()->at(k) != this) {
     int startUnitX = gameState->getUnits()->at(k)->getX();
     int startUnitY = gameState->getUnits()->at(k)->getY();
     int endUnitX = startUnitX + gameState->getUnits()->at(k)->getSize()*64;
     int endUnitY = startUnitY + gameState->getUnits()->at(k)->getSize()*64;

     if(movementX >= startUnitX && movementX <= endUnitX &&
     movementY >= startUnitY && movementY <= endUnitY) canWalk = false;
     if(movementX+4 >= startUnitX && movementX+4 <= endUnitX &&
     movementY >= startUnitY && movementY <= endUnitY) canWalk = false;
     if(movementX >= startUnitX && movementX <= endUnitX &&
     movementY+4 >= startUnitY && movementY+4 <= endUnitY) canWalk = false;
     if(movementX+4 >= startUnitX && movementX+4 <= endUnitX &&
     movementY+4 >= startUnitY && movementY+4 <= endUnitY) canWalk = false;
     }
     }*/

	std::cout << "c: " << this->x << "," << this->y << " / m: " << this->movementX << "," << this->movementY << " / d: " << distance << " / td: " << this->distance << std::endl;
	std::cout << "dtd: " << getDistance(this->x,this->y,movementX,movementY) << std::endl;

    // Do pathfinding itself. It will create a AStarPath algorithm which calculates
    // the way we want. After that, moving will be set to true, so the move() method
    // will get called every frame. The currentPath will be deleted at the start to
    // free memory from previous calculations.
	std::cout << "=== PATHFINDING START ===" << std::endl;
	delete currentPath;
	currentPath = new AStarPath(this,this->x, this->y, this->movementX, this->movementY, gameState, this->distance, !working);
	this->final_list = currentPath->getList();
	std::cout << "=== PATHFINDING END ===" << std::endl;
	collisionCheck();
	if( final_list.size() > 1 && !collision) {
		this->moving = true;
	}

    // If unit can't move because something is at his destination, this unit tried to get
	// to its destination at shortest distance possible.
	else if(getDistance(this->x,this->y,movementX,movementY) >= this->distance) {
		this->distance += 10;
		moveTo(movementX, movementY, false, false, this->distance);
	}

    this->lastMoveCall = gameState->getTimer()->get_ticks();
    return 0;
}

void Unit::collisionCheck() {
	bool help = false;
	if(moving && !final_list.empty()) {
		for(unsigned int i = 0; i<gameState->getUnits()->size(); i++) {
			if(gameState->getUnits()->at(i) != this) {
				int startUnitX = gameState->getUnits()->at(i)->getX();
				int startUnitY = gameState->getUnits()->at(i)->getY();
				int endUnitX = startUnitX + gameState->getUnits()->at(i)->getSize()*64;
				int endUnitY = startUnitY + gameState->getUnits()->at(i)->getSize()*64;

				if(final_list.top()->getX()*8 >= startUnitX && final_list.top()->getX()*8 <= endUnitX &&
				   final_list.top()->getY()*8 >= startUnitY && final_list.top()->getY()*8 <= endUnitY) help = true;
				if(final_list.top()->getX()*8+32 >= startUnitX && final_list.top()->getX()*8+32 <= endUnitX &&
				   final_list.top()->getY()*8 >= startUnitY && final_list.top()->getY()*8 <= endUnitY) help = true;
				if(final_list.top()->getX()*8 >= startUnitX && final_list.top()->getX()*8 <= endUnitX &&
				   final_list.top()->getY()*8+32 >= startUnitY && final_list.top()->getY()*8+32 <= endUnitY) help = true;
				if(final_list.top()->getX()*8+32 >= startUnitX && final_list.top()->getX()*8+32 <= endUnitX &&
				   final_list.top()->getY()*8+32 >= startUnitY && final_list.top()->getY()*8+32 <= endUnitY) help = true;
			}
		}
	}
	if(help) {
		collision = true;
		this->collisionWait++;
		if(collisionWait >=20) {
			collision = false;
			collisionWait = 0;
			moveTo(movementX, movementY);
		}
	} else {
		collision = false;
		collisionWait = 0;
	}
}

int Unit::move() {
	if(collision) {
		collisionCheck();
	}
    for(float i = speed*(this->gameState->getTimer()->get_ticks()-lastMoveCall); i >=0 && moving && !collision; i-=0.3) {

    	// If unit is at destination, break this loop and cancel movement
        if(final_list.empty()) {
            moving=false;
            //std::cout << getDistance(x,y,movementX, movementY) << std::endl;

            // Move if unit is not at its desired destination.
            if(getDistance(x,y,movementX, movementY) > this->distance) {
            	moveTo(movementX, movementY);
            }
            break;
        }

        // If current block of coordinates is reached, switch to next ones.
        if(x == final_list.top()->getX()*8 && y == final_list.top()->getY()*8) {
            final_list.pop();
            if(final_list.empty()) break;
        }

        collisionCheck();
        if(collision) break;

        // Only move x or y, so diagonal walking isn't as twice as fast
        // as normal walking (because you increment both x and y and
        // doing 2 steps per for loop.
        if(rand()%2) {
            // Move x-wise
            if(final_list.top()->getX()*8-x > 0) {
                x++;
                // Changing orientation because when x is incremented, unit is moving right
                orientation = 1;
            } else {
                x--;
                // Changing orientation because when x is incremented, unit is moving left
                orientation = 0;
            }
        } else {
            // Move y-wise
            if(final_list.top()->getY()*8-y > 0) {
                y++;
            } else {
                y--;
            }
        }
    }
    this->lastMoveCall = gameState->getTimer()->get_ticks();

    return 0;
}

bool Unit::isMoving() {
    return moving;
}

int Unit::getSupplyCost() {
    return this->supplyCost;
}

void Unit::initUnit(int x, int y, GameState* gameState, Player* owner) {
    this->owner = owner;
    this->currentPath = NULL;
    this->gameState = gameState;
    this->lastAttackCall = gameState->getTimer()->get_ticks();
    this->lastMoveCall = gameState->getTimer()->get_ticks();

    this->x = x;
    this->y = y;

    this->moving = false;
    this->attackB = false;

    this->supplyCost = gameState->getData()->getSupply(ID);
    this->movementX = 0;
    this->movementY = 0;

    this->working = false;
    this->workX = 0;
    this->workY = 0;

    this->collision = false;
    this->collisionWait = 0;

    this->distance = 0;
    this->orientation = 0;
}

Soldier::Soldier(int x, int y, GameState* gameState, Player* owner) {
    this->ID = 41;
    initUnit(x,y,gameState, owner);
    this->dps=15;
    this->range = 250;
    this->size = gameState->getData()->getSize(ID);
    this->speed=100;

    this->life = gameState->getData()->getLife(ID);
    this->currentLife=life;
}

Worker::Worker(int x, int y, GameState* gameState, Player* owner) {
    this->ID = 42;
    initUnit(x,y,gameState, owner);
    this->dps=1;
    this->range = 1;
    this->size = gameState->getData()->getSize(ID);
    this->speed=50;

    this->cameToGold = 0;
    this->goldBuilding = NULL;
    this->gold = 0;

    this->life = gameState->getData()->getLife(ID);
    this->currentLife=life;
}

int Worker::work() {
    if(!moving && workX != 0 && workY != 0 && x != workX && y != workY) { // Stopped moving -> should be at gold mineral
        this->working = true; // if not yet set!
        this->gameState->getPlayers()->operator [](0)->getRessources()->setGold(this->gameState->getPlayers()->operator [](0)->getRessources()->getGold()+this->gold);
        this->gold = 0;
        moveTo(workX, workY, false);
        if(this->goldBuilding->getCurrentLife() <= 0) {
            for(unsigned int i=0; i < this->gameState->getBuildings()->size(); i++) {
                if(this->gameState->getBuildings()->operator [](i) == this->goldBuilding) {
                    this->gameState->getBuildings()->erase(this->gameState->getBuildings()->begin()+i);
                }
            }
            delete this->goldBuilding;

            this->working = false;
            this->workX = 0;
            this->workY = 0;
        }
    }

    if(!moving && working && x == workX && y == workY) {
        int closestCCindex = -1;
        float closestCCdistance = 10000000;
        for(unsigned int i = 0; i < this->gameState->getBuildings()->size(); i++) {
            if(this->gameState->getBuildings()->at(i)->getID() == 21 && this->gameState->getBuildings()->at(i)->getOwner() == this->gameState->getPlayers()->at(0) ) {
                // Euclid distance between unit and CC
                float distance1 = this->gameState->getBuildings()->operator [](i)->getX()*64 - workY;
                float distance2 = this->gameState->getBuildings()->operator [](i)->getY()*64 - workX;
                if(distance1 < distance2) {
                    int help;
                    help=distance1;
                    distance1=distance2;
                    distance2=help;
                }
                float distance = std::sqrt(distance1*distance1 - distance2*distance2);
                if(distance <= closestCCdistance) {
                    closestCCdistance = distance;
                    closestCCindex = i;
                }
            }
        }
        if(closestCCindex == -1) {
            // There is no CC, error
        } else {
            if(cameToGold == 0)
                cameToGold = this->gameState->getTimer()->get_ticks();

            if((this->gameState->getTimer()->get_ticks() - cameToGold) >= 2 ) {
                this->gold = 10;
                this->goldBuilding->setDamage(10);
                moveTo( this->gameState->getBuildings()->operator [](closestCCindex)->getX()*64,
                       this->gameState->getBuildings()->operator [](closestCCindex)->getY()*64, false);
                cameToGold = 0;
            }
        }
    }
    return 0;
}

int Worker::setWork(int x, int y, Building* goldBuilding) {
    moveTo(x+65,y+65);
    this->cameToGold = 0;
    this->goldBuilding = goldBuilding;
    this->workX = x+32;
    this->workY = y+32;
    return 0;
}

int Unit::calculate() {
    move();
    if(this->ID == 42) {
        ((Worker*)this)->work();
    }

    attack();

    for( unsigned int p = 0; p < particles.size(); p++ ) {
        if(particles[p] != NULL) {
            if(!particles[p]->isDead())
                particles[p]->show();
            else {
                delete(particles[p]);
                particles[p] = NULL;
            }
        }
    }
    return 0;
}

int Unit::attack() {
    // This method gets called every frame of game loops calculation,
    // but we only want to call it if attackB is set to true
    if(attackB && this->attackedObject != NULL && !moving) {
        // We want to stop to shoot when a building or unit we are attacking
        // hits zero hp.
        if(this->attackedObject->getCurrentLife() > 0) {

            // Tracks the distance from current position to the attacked object. This is necessary
            // to move the unit in the direction of the attacked object if it is out of range.
            float distance = getDistance(this->x, this->y, (this->attackX+attackedObject->getSize()*64/2),(this->attackY+attackedObject->getSize()*64/2));
            if(distance > range) {
                moveTo((this->attackX+attackedObject->getSize()*64/2), (this->attackY+attackedObject->getSize()*64/2), true, false, range);
                this->distance = range;
                // If the distance of the attacked object is lower than range, we are in correct
                // range to attack -> starting attack.
            } else {
                // Initialize particles creation. This is the shooting animation itself
                // created on display.
                particles.resize(5);
                for( int p = 0; p < 5; p++ ) {
                    if(particles[p] == NULL) {
                        particles[p] = new Particle(this->x, this->y, (this->attackX+attackedObject->getSize()*64/2), (this->attackY+attackedObject->getSize()*64/2), gameState );
                    }
                }

                // When the time since the last call of this part of the method is higher
                // than a second, we want the unit to damage the object by the DPS it
                // has got.
                if(this->gameState->getTimer()->get_ticks()-lastAttackCall > 1) {
                    if(this->attackedObject->setDamage(this->dps) <= 0) {
                        attackB = false;
                        attackedObject = NULL;
                        for(unsigned int p = 0; p<particles.size(); p++) {
                            if(particles[p] != NULL)
                                delete(particles[p]);
                        }
                        particles.resize(0);
                        return 0;
                    }
                    this->lastAttackCall = this->gameState->getTimer()->get_ticks();
                }
            }
        }
    }
    return 0;
}

GameObject* Unit::getAttackedObject() {
    return this->attackedObject;
}

void Unit::stopAttack() {
    this->attackB = false;
    this->attackedObject = NULL;
}

int Unit::sendTo(GameObject* attacked) {
    // When you send something it is like moving something, so we want
    // to cancel previous done stuff just in case we are doing something
    // right now.
    attackB = false;
    working = false;
    moving = false;

    // This will send the worker to work at a gold mineral because
    // of the if statement
    if(attacked->getID() == 3 && this->getID() == 42) {
        ((Worker*)this)->setWork(attacked->getX()*64, attacked->getY()*64, ((Building*)attacked));
        // This will let a unit attack the object given by method argument
    } else {
        if(attacked->getOwner() != gameState->getPlayers()->at(0)) {
            this->attackB=true;
            // Unit coordinates are per pixel
            if(attacked->getID()>=40) {
                this->attackX=attacked->getX();
                this->attackY=attacked->getY();
                // Building coordinates are per tile
            } else {
                this->attackX=attacked->getX()*64;
                this->attackY=attacked->getY()*64;
            }
            this->attackedObject=attacked;
        }
    }
    
    return 0;
}

float* Unit::getMColor() {
    return owner->getColor();
}

Unit::~Unit() {
	delete currentPath;
}

Soldier::~Soldier() {}

Worker::~Worker() {}