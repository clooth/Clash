//
//  Unit.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Unit__
#define __Clash__Unit__

#include <queue>
#include <stack>
#include <algorithm>
#include <vector>
#include <list>

#include "Tile.h"
#include "Map.h"
#include "GameObject.h"
#include "Node.h"

class GameState;
class Building;
class Particle;
class AStarPath;

/**
 * This class represents a unit in the game. It gets constructed by the
 * Building Class and manages actions done by units as well as it's attributes.
 * It's inheritanced of GameObject because we need that in game class, where a
 * pointer to either buildings or units is placed.
 */
class Unit: public GameObject {
protected:
	/**
	 * Orientation
	 */
	int orientation;

    /**
     * Pathfinding object
     */
    AStarPath* currentPath;

    /**
     * Output of A* algorithm, stack of each node the unit should move
     */
    std::stack<Node*> final_list;

    /**
     * Game state object
     */
    GameState* gameState;

    /**
     * x coordinate of destination of current move
     */
    int movementX;

    /**
     * y coordinate of destination of current move
     */
    int movementY;

    /**
     * true if unit is currently moving
     */
    bool moving;

    /**
     * true if unit is currently working (only workers)
     */
    bool working;

    /**
     * true if unit is currently attacking
     */
    bool attackB;

    /**
     * X coordinate where unit currently attacks
     */
    int attackX;

    /**
     * Y coordinate where unit currently attacks
     */
    int attackY;

    /**
     * bool variable which tracks if this unit is in collision with another unit
     */
    bool collision;

    /**
     * gets incremented each frame where this unit is in collision
     */
    int collisionWait;

    /**
     * pointer to object which is attacked
     */
    GameObject* attackedObject;

    /**
     * X coordinate where unit currently works (only workers)
     */
    int workX;

    /**
     * Y coordinate where unit currently works (only workers)
     */
    int workY;

    /**
     * ID of unit
     */
    int unitID;

    /**
     * Range of unit
     */
    int range;

    /**
     * The running speed of unit
     */
    int speed;

    /**
     * How much the unit costs
     */
    int supplyCost;

    /**
     * Particles for shooting
     */
    std::vector<Particle*> particles;

    /**
     * time last attack call
     */
    float lastAttackCall;

    /**
     * time last move call
     */
    float lastMoveCall;

    /**
     * Damage per second of unit
     */
    int dps;

    /**
     * Distance to move to
     */
    int distance;

    /**
     * Doing Constructor stuff for inherited classes (specific units)
     *
     * @param x position
     * @param y position
     * @param gameState gamestate object
     * @param owner the player who owns this unit
     */
    void initUnit(int x, int y, GameState* gameState, Player* owner);

    /**
     * Checks for collisions with other units. Gets called by move();
     */
    void collisionCheck();

public:
    /**
     * Destructor
     */
    virtual ~Unit();

    /**
     * Giving back orientation of unit
     *
     * currently:
     * 0 = left
     * 1 = right (flipped)
     *
     * @return orientation
     */
    int getOrientation();

    /**
     * Reinitializing movement variables
     * @return nothing
     */
    int newMove();

    /**
     * Calculate unit stuff
     *
     * @return nothing
     */
    int calculate();

    /**
     * getting the minimap color for this unit
     *
     * @return array with rgb values of color
     */
    float* getMColor();

    /**
     * Send unit to attack
     *
     * @return nothing
     */
    int attack();

    /**
     * Gets if unit is moving
     * @return boolean
     */
    bool isMoving();

    /**
     * Sending unit somewhere
     *
     * @param attacked pointer to object to attack
     * @return nothing
     */
    int sendTo(GameObject* attacked);

    /**
     * Setting the unit to move somewhere
     *
     * @param x coordinate of destination
     * @param y coordinate of destination
     * @param workerConst deprecated
     * @param attackConst deprecated
     * @param distance the distance to move to
     * @return nothing
     */
    int moveTo(int x, int y, bool workerConst=true, bool attackConst=true, int distance=0);

    /**
     * Handles movement of units, gets called by game loop every frame
     * to move the units depending on how much time went since last frame.
     *
     * @return nothing
     */
    int move();

    /**
     * Get the destination of movements
     *
     * @return x value of destination
     */
    int getMovementX();

    /**
     * Get the destination of movements
     *
     * @return y value of destination
     */
    int getMovementY();

    /**
     * Get the supply cost of this unit
     *
     * @return supply cost
     */
    int getSupplyCost();

    /**
     * Get a pointer to the by this unit attacked object
     *
     * @return attackedObject
     */
    GameObject* getAttackedObject();

    /**
     * Sets unit to stop all attacks
     */
    void stopAttack();
};

/**
 * This class represents a specific unit in the game.
 *
 * Small infantry unit which can attack ground or air units.
 */
class Soldier: public Unit {
public:
    /**
     * Constructor for specific unit, is initializing the attributes.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param gameState GameState object
     * @param owner the player who owns this unit
     */
    Soldier(int, int, GameState* gameState, Player* owner);

    /**
     * Destructor
     */
    ~Soldier();
};

/**
 * This class represents a specific unit in the game.
 *
 * It's the Worker unit. Used to collect ressources.
 */
class Worker: public Unit {
private:
    /**
     * Gold the unit currently has got in backpack
     */
    int gold;

    /**
     * pointer to ressource building where unit is mining
     */
    Building* goldBuilding;

    /**
     * gets true if worker is currently in the ressource building.
     */
    float cameToGold;
public:
    /**
     * Constructor for specific unit, is initializing the attributes.
     *
     * @param x x coordinate
     * @param y y coordinate
     * @param gameState GameState object
     * @param owner the player who owns this unit
     */
    Worker(int, int, GameState*, Player*);

    /**
     * Destructor
     */
    ~Worker();

    /**
     * Setting this worker to collect something
     *
     * @param x x coordinate of position where ressource is
     * @param y y coordinate of position where ressource is
     * @param goldBuilding pointer to ressource building
     * @return zero
     */
    int setWork(int x, int y, Building* goldBuilding);

    /**
     * Working loop which gets called every frame by gameloop,
     * manages the unit working to walk from ressource building
     * to command center and back while getting ressources
     *
     * @return nothing
     */
    int work();
};

#endif /* defined(__Clash__Unit__) */
