//
//  GameObject.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__GameObject__
#define __Clash__GameObject__

class Player;

/**
 * Parent class for Units and Buildings
 */
class GameObject {
protected:
    /**
     * Position where the object is at
     */
    int x,y;

    /**
     * The size of building. Relative to a tile (e.g. 1 = 64, 0.5 = 32)
     */
    float size;

    /**
     * ID of object, more about IDs here http://redmine.mkasu.org/documents/1
     */
    int ID;

    /**
     * Player who's owning this building
     */
    Player* owner;

    /**
     * The maximum life this building has got.
     */
    int life;

    /**
     * The current life a building has, after getting damage or something else.
     */
    int currentLife;

    /**
     * Tracking if object is "dead", that's when it's at 0 hp, indicating
     * for other classes that it can't be used anymore.
     */
    bool dead;

public:
    /**
     * Get x coordinate of position of object
     *
     * @return int
     */
    int getX() const;

    /**
     * Get y coordinate of position of object
     *
     * @return int
     */
    int getY() const;

    /**
     * Get size of the object
     *
     * @return int
     */
    float getSize() const;

    /**
     * Get ID of object
     *
     * @return int
     */
    int getID() const;

    /**
     * Get maximum life of object.
     * @return maximum life
     */
    int getLife() const;

    /**
     * Doing damage to this object
     * @param damage how much damage you did
     * @return life remaining
     */
    int setDamage(int damage);

    /**
     * Get current life the building has after getting damage (or not).
     * @return current life
     */
    int getCurrentLife() const;

    /**
     * Return pointer to owner of this building
     *
     * @return owner
     */
    Player* getOwner();
};

#endif /* defined(__Clash__GameObject__) */
