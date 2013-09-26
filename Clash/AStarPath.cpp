/*
 * File:   AStarPath.cpp
 * Created on May 3, 2011
 *
 * Copyright (C) 2011 by Marc A. Kastner <m.kastner@tu-bs.de>
 * Part of the YaRTS Project http://yarts.mkasu.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 *
 * See the COPYING file for more details.
 */

/**
 * @file
 * AStarPath class
 */

#include <cmath>

#include "AStarPath.h"
#include "Node.h"
#include "GameState.h"
#include "Map.h"
#include "Unit.h"

float getDistance(int x1, int y1, int x2, int y2) {
    int xd = x1-x2;
    xd = xd*xd;

    int yd = y1-y2;
    yd = yd*yd;

    return (std::sqrt((float)xd+yd));
}

AStarPath::AStarPath(Unit* thisUnit, int startX, int startY, int destX, int destY,GameState* gameState, int distance, bool onBuilding) {
	this->thisUnit = thisUnit;
	this->startX = startX;
    this->startY = startY;
    this->destX = destX;
    this->destY = destY;
    this->distance = distance;
    this->onBuilding = onBuilding;
    this->gameState = gameState;
	for (unsigned int i = 0; i < 880; i++) {
		for(unsigned int j = 0; j < 880; j++) {
			map[i][j] = NULL;
		}
	}
    pathFind();
}

//AStarPath::AStarPath(const AStarPath& orig) {
//}

AStarPath::~AStarPath() {
	/*for(unsigned int i=0; i<map.size();i++) {
		for ( std::vector<Node*>::iterator it = map[i].begin(); it != map[i].end(); ++it ) {
			delete * it;
		}
	}*/
	/*for( std::vector<Node*>::iterator it = closed_list.begin(); it != closed_list.end(); ++it ) {
		delete * it;
	}
	for ( std::vector<Node*>::iterator it = open_list.begin(); it != open_list.end(); ++it ) {
		delete * it;
	}*/
	for(int i = 0; i<880; i++) {
		for(int j = 0; j<880; j++) {
			if(map[i][j] != NULL) {

				//std::cout << i << " / " << j << std::endl;
				delete map[i][j];
			}
		}
	}
}

std::stack<Node*> AStarPath::getList() {
    return this->final_list;
}

int AStarPath::pathFind() {
    if(!checkUnitBorders(destX/8, destY/8) && distance <=20) {
    	int newDestX = destX/8 + (rand()%3-1),
    		newDestY = destY/8 + (rand()%3-1);
    	bool help = true;
    	for(int i=0; i<distance; i++) {
    		newDestX += (rand()%3-1),
    		newDestY += (rand()%3-1);
			if(checkUnitBorders(newDestX, newDestY)) {
				this->destX = newDestX*8;
				this->destY = newDestY*8;
				help = false;
			}
    	}
    	std::cout << help << std::endl;
    	if(help) return 0;
    }

    // Creating a map object we search our path in. You move an unit from
    // a start coordinate tuple, given in pixels to a destination tuple,
    // also given in pixels. The map is designed as 64x64 pixel tiles, but
    // that's too unaccurate for a pathfinding algorithm. So I choiced 8x8
    // pixels, to be good enough.
    // As maps are currently sized at 110x110 tiles, which is not possible
    // to change. We need an graph with 880x880 nodes.
    // Because most nodes of your graph won't even be touched, these won't
    // be initialized at start.
    //map.resize(880);

    // The only two points which will be needed in each pathfinding call
    // are the start and destination nodes, so we are initializing them
    map[this->startX/8][this->startY/8] = new Node(this->startX/8,this->startY/8,0,0);
    map[this->destX/8][this->destY/8] = new Node(this->destX/8,this->destY/8,0,0);

    // The open list is a list of each object we are going to look at.
    // At the beginning, only the start node is in this list.
    open_list.push_back(map[this->startX/8][this->startY/8]);

    // Main loop for the algorithm, when this terminates, you have
    // either a found path or there is none existing.
    bool found = false;
    while(!found) {
        // If open_list is totally empty, we removed each object which is in
        // it, therefore, there is no path to destination because we looked
        // at every node.
        if(open_list.empty() || closed_list.size() >= 3000) {
            found = true;
            return 0;
        }

        // We are looking for the shortest path, so the neighbor nodes with lowest
        // distance, equals to highest priority, will get checked fist.
        // TODO: This can be optimized by remembering the point we're at and only looking for direct neighbours.
        int indexMin = 0;
        for(int i=open_list.size()-1; i>=0; i--) {
            if(open_list[indexMin]->getPriority() >= open_list[i]->getPriority()) {
                indexMin = i;
            }
        }
        //std::cout << "Current: " << open_list[indexMin]->getX() << "/" << open_list[indexMin]->getY() << std::endl;
        //std::cout << "Destination: " << this->destX << "/" << this->destY << std::endl;
        //std::cout << "openListSize: " << open_list.size() << std::endl;
        //std::cout << "closedListSize: " << closed_list.size() << std::endl;
        //std::cout << "empty? " << open_list.empty() << std::endl;

        // The current node will be set to the node which was found as the
        // one with highest priority.
        Node* currentNode = open_list[indexMin];

        // As we are looking at this node now, we will erase it from open list.
        open_list.erase(open_list.begin()+indexMin);

        // Euclid distance to destination
        // If distance arg is set, unit will only move until a range, so we
        // need to calculate range to destination to get a value to compare to.
        float toDest = getDistance(currentNode->getX()*8, currentNode->getY()*8, destX, destY);

        // If currentNode is the destination point, we're done with the algorithm.
        if(currentNode == map[destX/8][destY/8] || toDest <= distance) {
            found = true;

            Node* n = currentNode;
            bool help = true;

            // We are going back though each node we walked though while finding the path
            // using the getPredecessor() method, which provides a pointer to the previous
            // node in the path. This way, we go back from destination to the start while
            // writing the fastest path in a final_list queue.
            while(help) {
                final_list.push(n);
                n = map[n->getX()][n->getY()];
                n = n->getPredecessor();
                if(n == map[startX/8][startY/8]) {
                    final_list.push(n);
                    help = false;
                } else if(n == NULL) {
                    help = false;
                }
            }
            return 1;
        }

        // We are expanding the current node. This means, we are searching
        // for neighbour nodes which we should look at and add them to
        // the open list.
        expandNode(currentNode);

        // As we went though all things we can do with this current node,
        // we can add it to the closed list. This one won't get touched
        // any more. It's like a blacklist of nodes.
        closed_list.push_back(currentNode);
    }
    return 0; // This should never happen
}

bool AStarPath::checkUnitBorders(int x, int y) {
    // This part checks if a building is on this coordinates. If so, it
    // will return false.
    if(onBuilding) { // Not for Worker, they should run through stuff when working.
        if(gameState->getMap()->tileUsed((x*8-((x*8)%64))/64,(y*8-((y*8)%64))/64)) return false;
        if(gameState->getMap()->tileUsed(((x+4)*8-(((x+4)*8)%64))/64,(y*8-((y*8)%64))/64)) return false;
        if(gameState->getMap()->tileUsed((x*8-((x*8)%64))/64,((y+4)*8-(((y+4)*8)%64))/64)) return false;
        if(gameState->getMap()->tileUsed(((x+4)*8-(((x+4)*8)%64))/64,((y+4)*8-(((y+4)*8)%64))/64)) return false;

        //  This part checks if another unit is on this coordinates. If so,
		// it will return false.
		/*std::cout << "=== UNIT CHECK ===" << std::endl;*/
		for(unsigned int i = 0; i<gameState->getUnits()->size(); i++) {
			if(gameState->getUnits()->at(i) != thisUnit) {
				int startUnitX = gameState->getUnits()->at(i)->getX();
				int startUnitY = gameState->getUnits()->at(i)->getY();
				int endUnitX = startUnitX + gameState->getUnits()->at(i)->getSize()*64;
				int endUnitY = startUnitY + gameState->getUnits()->at(i)->getSize()*64;

				/*std::cout << "Unit (" << i << ")" << std::endl;
				std::cout << "Unit: " << startUnitX/8 << "/" << startUnitY/8 << ", " << endUnitX/8 << "/" << endUnitY/8 << std::endl;
				std::cout << "This: " << x << "/" << y << std::endl;
				std::cout << "Destination: " << destX/8 << "/" << destY/8 << std::endl;*/

				if(x >= startUnitX/8 && x <= endUnitX/8 && y >= startUnitY/8 && y <= endUnitY/8) return false;
				if(x+4 >= startUnitX/8 && x+4 <= endUnitX/8 && y >= startUnitY/8 && y <= endUnitY/8) return false;
				if(x >= startUnitX/8 && x <= endUnitX/8 && y+4 >= startUnitY/8 && y+4 <= endUnitY/8) return false;
				if(x+4 >= startUnitX/8 && x+4 <= endUnitX/8 && y+4 >= startUnitY/8 && y+4 <= endUnitY/8) return false;
			}
		}
    }
    // This part checks if a non-movable tile is on this coordinates. If so,
    // it will return false.
    if(gameState->getMap()->tileType((x*8-((x*8)%64))/64,(y*8-((y*8)%64))/64) != 1) return false;
    if(gameState->getMap()->tileType(((x+4)*8-(((x+4)*8)%64))/64,(y*8-((y*8)%64))/64) != 1) return false;
    if(gameState->getMap()->tileType((x*8-((x*8)%64))/64,((y+4)*8-(((y+4)*8)%64))/64) != 1) return false;
    if(gameState->getMap()->tileType(((x+4)*8-(((x+4)*8)%64))/64,((y+4)*8-(((y+4)*8)%64))/64) != 1) return false;

    // Return true if method have not returned yet == nothing in the way
    return true;
}


void AStarPath::expandNode(Node* currentNode) {
    int tentative_g;
    // Get list of successors
    std::vector<Node*> successor;

    successor.resize(8);
    int i = 0;
    for(int j = -1; j <= 1; j++) {
        for(int k = -1; k <= 1; k++) {
            if(currentNode->getX()+j >= 0 && currentNode->getY()+k >= 0) {
                if(map[currentNode->getX()+j][currentNode->getY()+k] == NULL) {
                    map[currentNode->getX()+j][currentNode->getY()+k] =
                        new Node(currentNode->getX()+j,currentNode->getY()+k,0,0);
                }
                if(!(k==0 && j==0)) {
                    if(checkUnitBorders(currentNode->getX()+j,currentNode->getY()+k)) {
                        successor[i] = map[currentNode->getX()+j][currentNode->getY()+k];
                        i++;
                    }
                }
            }
        }
    }
    successor.resize(i);

    // Going through them
    for(unsigned int i = 0; i < successor.size(); i++) {
        // Continue search if node is already on closed list
        bool help = false;
        for(int j = closed_list.size()-1; j >= 0; j--) {
            if(closed_list[j]->equals(successor[i])) {
                help = true;
                break;
            }
        }
        if(help) {
            continue;
        }

        tentative_g = currentNode->getLevel()+1;

        help = false;
        for( std::vector< Node* >::iterator j = open_list.begin(); j != open_list.end(); ++j ) {
            if( ((*j)->getX() == successor[i]->getX()) &&
                ((*j)->getY() == successor[i]->getY()) &&
                tentative_g-1 <= (*j)->getPriority()) {
                help = true;
                break;
            }
        }
        if(help) {
            continue;
        }

        successor[i]->setPredecessor(currentNode);
        successor[i]->setLevel(tentative_g);
        successor[i]->updatePriority(destX/8, destY/8);

        open_list.push_back(successor[i]);
    }
}

