//
//  Player.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Player__
#define __Clash__Player__

#include <vector>

#include "Ressources.h"

/**
 * This class represents a single user. Managing stuff which
 * is different for different players. Like the color their
 * buildings have or the ressources they have.
 */
class Player {
public:
    /**
     * Constructor, initializing ressources
     *
     * @param i id this player gets
     */
    Player(int i);

    /**
     * Destructor, does nothing
     */
    virtual ~Player();

    /**
     * Setting the color a player has got
     *
     * @param r red color
     * @param g blue color
     * @param b green color
     */
    void setColor(float r, float g, float b);

    /**
     * Getting the players color
     *
     * @return vector of colors
     */
    float* getColor();

    /**
     * Getting pointer to players ressources object
     *
     * @return pointer to Ressources
     */
    Ressources* getRessources();

    /**
     * Get player id
     *
     * @return int
     */
    int getID() const;
private:
    /**
     * ID of this player
     */
    int id;

    /**
     * Vector which saves data for all color codes
     */
    float color[3];

    /**
     * Ressources object
     */
    Ressources* ressources;
};

#endif /* defined(__Clash__Player__) */
