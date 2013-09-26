//
//  Building.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Building__
#define __Clash__Building__

#include <iostream>
#include <vector>
#include <queue>

#include "GameObject.h"

class GameState;
class Timer;
class Ressources;
class Unit;
class Map;
class Tooltip;
class Player;

/**
 * This struct is used to save the data of a single unit getting queue to
 * build by a building.
 */
struct BuildUnit {
    /**
     * X-coordinate of position where unit
     * should be placed after building
     */
    int positionX;
    /**
     * Y-coordinate of position where unit
     * should be placed after building
     */
    int positionY;
    /**
     * Time when unit started building. It is needed
     * for calculating it's time until done as well
     * as the percentage done.
     */
    float startTime;
    /**
     * The Time this unit needs to build
     */
    int buildTime;
    /**
     * Internal id of that unit type which gets build
     */
    int id;
};

/**
 * This class represents a building in the game. It gets constructed by the
 * Game Class and manages actions done by buildings as well as it's attributes.
 * It's inheritanced of GameObject because we need that in game class, where a
 * pointer to either buildings or units is placed.
 */
class Building: public GameObject {
protected:
    /**
     * Object to the state of the game. This object has got pointers to almost
     * everything know-worthy in the game. It's used to get information of units
     * or map from within this class.
     */
    GameState* gameState;

    /**
     * Color for buildings not owned by player
     */
    float mcolor[3];

    /**
     * Tooltip for actions
     */
    std::vector<Tooltip*> tooltips;

    /**
     * How much supply this building gives. It is important for
     * ressource class calculating the maximum supply.
     */
    int givesSupply;

    /**
     * States if the building is still constructing
     */
    bool inBuild;

    /**
     * The time when constructing this building started. It's used to
     * calculate the time until it's done.
     */
    float startBuild;

    /**
     * The time, this building needs to construct. Together with startBuild
     * it is used to calculate the remaining time until done.
     */
    int buildTime;

    /**
     * Waypoint coordinates
     */
    int waypoint[2];

public:
    /**
     * Destructor
     */
    virtual ~Building();

    /**
     * Setting waypoint to spawn units for this building
     *
     * @param mouseX coordinate to set to
     * @param mouseY coordinate to set to
     */
    void setWaypoint(int mouseX, int mouseY);

    /**
     * Getting waypoint where units are going to spawn
     *
     * @return array with x,y coordinates
     */
    int* getWaypoint();

    /**
	 * When you want to build units using this building, it get's queued here,
	 * so the building can build them one after another.
	 */
	std::queue<BuildUnit> unitBuild;

    /**
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     *
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    virtual void doAction(int clickX, int clickY) =0;

    /**
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    virtual void doMotion(int clickX, int clickY) =0;

    /**
     * Drawing the progress bars and warnings when building is still
     * in build.
     */
    void DrawInBuild();

    /**
     * Returning Minimap Color
     *
     * @return minimap color
     */
    float* getMColor();

    /**
     * Set the map to used at the coordinates where the building is
     * set. This is called from the building initialization.
     *
     * @param x coordinate of building
     * @param y coordinate of building
     */
    void createBuilding(int, int);

    /**
     * Gets called by constructor. Starts normal stuff which is done
     * by every building type like getting build timers after creation.
     */
    void initBuilding(GameState*);


    /**
     * Get supply the building might give (for CC/Depot)
     * @return how much supply building gives
     */
    int getGivesSupply();

    /**
     * Queuing a unit. Gets called by the doAction eventhandler
     * and puts the unit which is indicated by the unitID in
     * queue to build it.
     *
     * @param id of unit to put in queue
     * @return nothing
     */
    int placeUnit(int id);

    /**
     * This method is called every frame. It manages the building of the
     * units and gives back percentage of the units currently being build.
     *
     * @return percantage of current building unit
     */
    int buildUnit();

    /**
     * Checks if building is currently building a unit.
     *
     * @return bool if it does
     */
    bool isBuildingUnit();

    /**
     * This unit manages the creation of buildings and building itself up
     * after being placed. It gets called every frame.
     *
     * @return percentage of building done. -1 if it's already build up, -2 if it's already dead.
     */
    float buildBuilding();
};

/**
 * This class represents a specific building in the game.
 *
 * It's the command center. It's the starter building where you
 * can build workers. It's needed by everything else.
 */
class CommandCenter: public Building {
public:
    /**
     * Constructor of command center. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    CommandCenter(int x, int y, GameState* gameState, Player* owner, bool instant = true);
    /**
     * Destructor
     */
    ~CommandCenter();
    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);
    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};

/**
 * This class represents a specific building in the game.
 *
 * It's the barracks. It's the main place to build infantry units.
 */
class Barracks: public Building {
public:
    /**
     * Constructor of barracks. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    Barracks(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~Barracks();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);
    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};

/**
 * This class represents a specific building in the game.
 *
 * It's the gold mineral. Here, worker units can collect ressources.
 */
class Gold: public Building {
public:
    /**
     * Constructor of gold object. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     */
    Gold(int x, int y, GameState* gameState);

    /**
     * Destructor
     */
    ~Gold();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};

/**
 * This class represents a specific building in the game.
 *
 * It's the supply depot. Main building to get more supply.
 */
class SupplyDepot: public Building {
public:
    /**
     * Constructor of supply depot. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    SupplyDepot(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~SupplyDepot();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX,int clickY);

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};


/**
 * This class represents a specific building in the game.
 *
 * It's the Research lab where you can research buffs for your units.
 */
class ResearchLab: public Building {
public:
    /**
     * Constructor of supply depot. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    ResearchLab(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~ResearchLab();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);
    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};


/**
 * This class represents a specific building in the game.
 *
 * It's the military airport, where you can build air units.
 */
class MilAirport: public Building {
public:
    /**
     * Constructor of supply depot. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    MilAirport(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~MilAirport();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};


/**
 * This class represents a specific building in the game.
 *
 * It's the factory where you can build mechanical ground units.
 */
class Factory: public Building {
public:
    /**
     * Constructor of supply depot. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    Factory(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~Factory();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};


/**
 * This class represents a specific building in the game.
 *
 * It's the tower, which you can use for anti air defense.
 */
class Tower: public Building {
public:
    /**
     * Constructor of supply depot. It gets coordinates of the place where
     * it is placed together with the GameState pointer.
     *
     * It's initializing the building with setting the map as "used", the
     * place where it's set while setting it's own attributes.
     *
     * @param x x coordinate where building placed
     * @param y y coordinate where building placed
     * @param gameState pointer to game state object
     * @param owner pointer to player who owns this building
     * @param instant if building should be build instantly
     */
    Tower(int x, int y, GameState* gameState, Player* owner, bool instant = true);

    /**
     * Destructor
     */
    ~Tower();

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mouseclick as args and the method doing
     * whatever the user clicked on.
     * @param clickX x coordinate of mouseclick
     * @param clickY y coordinate of mouseclick
     */
    void doAction(int clickX, int clickY);

    /**
     * Implementation for:
     *
     * Gets called by the event handler to handle the input. You give
     * the coodinates of the mousemotion as args and the method doing
     * whatever the user moves mouse on.
     *
     * @param clickX x coordinate of mousemotion
     * @param clickY y coordinate of mousemotion
     */
    void doMotion(int clickX, int clickY);
};

#endif /* defined(__Clash__Building__) */
