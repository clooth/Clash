//
//  GameObject.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "GameObject.h"

int GameObject::getX() const {
    return this->x;
}

int GameObject::getY() const {
    return this->y;
}

float GameObject::getSize() const {
    return this->size;
}

int GameObject::getID() const {
    return this->ID;
}


Player* GameObject::getOwner() {
    return owner;
}

int GameObject::setDamage(int damage) {
    this->currentLife -= damage;
    if(currentLife <= 0 ) {
        dead = true;
    }
    return currentLife;
}

int GameObject::getLife() const {
    return this->life;
}

int GameObject::getCurrentLife() const {
    return this->currentLife;
}
