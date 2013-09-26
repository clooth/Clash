//
//  Particle.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Particle__
#define __Clash__Particle__

class GameState;
class SDL_Surface;

/**
 * Used to make particles, very very simple "particle" engine
 */
class Particle {
public:
    /**
     * Constructor, loads particle image and randomize start offset
     *
     * @param x x coordinate start
     * @param y y coordinate start
     * @param xDest x coordinate destination
     * @param yDest y coordinate destination
     * @param gs state of the game object
     */
    Particle(int x,int y,int xDest,int yDest, GameState* gs);

    /**
     * Destructor
     */
    virtual ~Particle();


    /**
     * Gets called every frame. Moves particles.
     */
    void show();

    /**
     * Checks if particle is at destination
     *
     * @return bool
     */
    bool isDead();
private:
    /**
     * State of the game object
     */
    GameState* gameState;

    /**
     * Position x
     */
    int x;

    /**
     * Position y
     */
    int y;

    /**
     * Destination x
     */
    int xDest;

    /**
     * Destination y
     */
    int yDest;
};

#endif /* defined(__Clash__Particle__) */
