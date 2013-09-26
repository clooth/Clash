/*
 * File:   AStarPath.h
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
 * Definition AStarPath class
 */

#ifndef ASTARPATH_H
#define	ASTARPATH_H

#include <vector>
#include <stack>

class Node;
class GameState;
class Unit;

/**
 * Gets euclidian distance between two given points on coordinate systems.
 *
 * @param x1 first point
 * @param y1 first point
 * @param x2 second point
 * @param y2 second point
 * @return
 */
float getDistance(int x1, int y1, int x2, int y2);

/**
 * Handles unit path finding algorithms.
 */
class AStarPath {
private:
    /**
     * State of the game object
     */
    GameState* gameState;

    /**
     * Pointer to the unit this pathfinding instance is finding the path for.
     */
    Unit* thisUnit;

    /**
     * Map of nodes to get a graph where A* can search in
     */
    //std::vector<std::vector<Node*> > map;
    Node* map[880][880];

    /**
     * Used by A* Algorithm, list of every object is should look at
     */
    std::vector<Node*> open_list;

    /**
     * Used by A* Algorithm, list of every object already looked at
     */
    std::vector<Node*> closed_list;

    /**
     * Output of A* algorithm, stack of each node the unit should move
     */
    std::stack<Node*> final_list;

    /**
     * x coordinate of destination
     */
    int startX;

    /**
     * y coordinate of destination
     */
    int startY;

    /**
     * x coordinate of start
     */
    int destX;

    /**
     * y coordinate of start
     */
    int destY;

    /**
     * distance to move to
     */
    int distance;

    /**
     * if walking on buildings is active
     */
    bool onBuilding;

    /**
     * Checking if unit can walk here, used by A* algorithm
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return bool if it can be there.
     */
    bool checkUnitBorders(int x, int y);

   /**
     * Pathfind algorithm. Based on A* Algorithm
     * More information: http://en.wikipedia.org/wiki/A*_algorithm
     *
     * @return final list
     */
    int pathFind();

    /**
     * Second method used for pathfinding algorithm. Based on A* Algorithm
     * More information: http://en.wikipedia.org/wiki/A*_algorithm
     *
     * @param currentNode node we look at
     */
    void expandNode(Node*);

public:
    /**
     * Initializing the AStarPathfinding algorithm and calculates it.
     *
     * @param thisUnit pointer to this unit
     * @param startX start position
     * @param startY start position
     * @param destX end position
     * @param destY end position
     * @param gameState state of the game object
     * @param distance distance to move to
     * @param onBuilding true if unit can move on buildings. default is false.
     */
    AStarPath(Unit* thisUnit, int startX, int startY, int destX, int destY, GameState* gameState, int distance=0,bool onBuilding=false);

    /**
     * Freeing memory after pathfinding is done.
     */
    virtual ~AStarPath();

    /**
     * Returns final list for pathfinding result
     *
     * @return stack
     */
    std::stack<Node*> getList();
};

#endif	/* ASTARPATH_H */

