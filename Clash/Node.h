//
//  Node.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Node__
#define __Clash__Node__

/**
 * This class represents a node on the map. It is used by the pathfinding
 * algorithm where the map is interpreted as a graph.
 */
class Node {
protected:
    /**
     * Coordinates of this node
     */
    int x,y;
    /**
     * Priority of this node. As the A* Algorithm works heuristic, it
     * determines the distance to the destination with the euclid's
     * distance from each direction where you can go.
     */
    float priority;

    /**
     * How much steps you already did to get here.
     */
    int level;

    /**
     * Pointer to the object before this one.
     */
    Node * predecessor;

    /**
     * Estimates the remaining euclid distance
     *
     * @param xDest x coordinate of destination
     * @param yDest y coordinate of destination
     * @return distance
     */
    float estimate(int xDest, int yDest);

public:
    /**
     * Constructor of Node which sets basic attributes.
     *
     * @param x x coordinate position
     * @param y y coordinate position
     * @param p priority (distance to destination)
     * @param l level (distance from start)
     */
    Node(int x, int y, int p, int l);

    /**
     * Get the x coordinate
     *
     * @return int
     */
    int getX() const;

    /**
     * Get the y coordinate
     *
     * @return int
     */
    int getY() const;

    /**
     * Get the distance from start
     *
     * @return int
     */
    int getLevel() const;

    /**
     * Get the estimated complete distance from
     * start to destination over this node.
     *
     * @return int
     */
    float getPriority() const;

    /**
     * Get the predecessor node where unit comes from.
     *
     * @return node before this one on units way
     */
    Node* getPredecessor();

    /**
     * equals this node to another
     *
     * @param b pointer to other node
     * @return bool if it is the same
     */
    bool equals(Node* b) const;

    /**
     * setting the predecessor node
     *
     * @param p pointer to predecessor node
     */
    void setPredecessor(Node* p);

    /**
     * setting distance from start
     *
     * @param l distance
     */
    void setLevel(int l);

    /**
     * Updating Priority
     *
     * @param xDest destination x coordinate
     * @param yDest destination y coordinate
     */
    void updatePriority(int xDest, int yDest);
};

#endif /* defined(__Clash__Node__) */
