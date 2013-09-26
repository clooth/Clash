//
//  Node.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <cmath>
#include <iostream>

#include "Node.h"

Node::Node(int x, int y, int p, int l) {
    this->x = x;
    this->y = y;
    this->priority = p;
    this->level = l;
    this->predecessor = NULL;
}

int Node::getX() const {
    return this->x;
}

int Node::getY() const {
    return this->y;
}

int Node::getLevel() const {
    return this->level;
}

float Node::getPriority() const {
    return this->priority;
}

void Node::setPredecessor(Node* p) {
    this->predecessor = p;
}

Node* Node::getPredecessor() {
    return this->predecessor;
}

bool Node::equals(Node* b) const {
    if((this->x == b->getX()) && (this->y == b->getY())) {
        return true;
    }
    return false;
}

void Node::setLevel(int l) {
    this->level = l;
}

void Node::updatePriority(int xDest, int yDest) {
    priority = level+estimate(xDest, yDest)*2;
}

float Node::estimate(int xDest,int yDest) {
    int xd = xDest-x;
    int yd = yDest-y;
    return(std::sqrt((float)xd*xd+yd*yd));
}
