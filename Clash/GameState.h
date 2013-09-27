//
//  GameState.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__GameState__
#define __Clash__GameState__

#include <vector>
#include <string>

#include "GameData.h"

class Player;
class Building;
class Unit;
class Ressources;
class Map;
class Timer;
class Tooltip;
class SDL_Surface;
class GLRenderer;
class FogOfWar;

/**
 * This class does a lot of communication between classes
 * Even though circular class models aren't the best, this
 * game state object does a pretty good job and makes a lot
 * of things easier.
 *
 * It contains basically a pointer to each important array, vector
 * or object which is created in Game class, so I can access them
 * from other classes to ask for existing Maps, Buildings, Units, etc.
 */
class GameState {
public:
    /**
     * Constructor. Gets called from Game class and gives GameState
     * object a pointer to everything important.
     *
     * @param m pointer to map of game
     * @param b pointer to vector of pointers to buildings
     * @param u pointer to vector of pointers to units
     * @param a pointer to starttime for announcements
     * @param t pointer to timer
     * @param aS pointer to announcement texts
     * @param p pointer to vector of pointers to players
     * @param to pointer to tooltip which is currently displayed
     * @param fov pointer to fog of war object
     */
    GameState(Map* m, std::vector<Building*>* b,
    		  std::vector<Unit*>* u, int* a, Timer* t,
    		  std::string* aS, std::vector<Player*>* p,
    		  Tooltip* to, FogOfWar* fov);

    /**
     * Destructor
     */
    ~GameState();

    /**
     * Get pointer to vector of players
     *
     * @return pointer
     */
    std::vector<Player*>* getPlayers();

    /**
     * Get pointer to map
     *
     * @return pointer
     */
    Map* getMap();

    /**
     * Get pointer to vector of buildings
     *
     * @return pointer
     */
    std::vector<Building*>* getBuildings();

    /**
     * Get pointer to vector of units
     *
     * @return pointer
     */
    std::vector<Unit*>*  getUnits();

    /**
     * Get pointer to timer
     *
     * @return pointer
     */
    Timer* getTimer();

    /**
     * Starting announcement
     */
    void updateAnnounce();

    /**
     * Setting announcement text
     *
     * @param announceText text which should displayed
     */
    void setAnnounceText(std::string announceText);

    /**
     * Getting current active Tooltip
     *
     * @return pointer active Tooltip
     */
    Tooltip* getActiveTooltip();
    /**
     * Setting current active Tooltip
     *
     * @param t new tooltip to set to
     */
    void setActiveTooltip(Tooltip* t);

    /**
     * Getting data object
     *
     * @return data object
     */
    GameData* getData();

    /**
     * Getting screen to draw on
     *
     * @return screen object
     */
    SDL_Surface* getScreen();
    /**
     * Setting screen device
     *
     * @param s surface to set screen to
     */
    void setScreen(SDL_Surface* s);
    /**
     * Setting pointer to renderer object
     *
     * @param r renderer object
     */
    void setRenderer(GLRenderer* r);

    /**
     * Getting pointer to renderer object
     *
     * @return renderer object
     */
    GLRenderer* getRenderer();

    /**
     * Gets pointer to fog of war object
     *
     * @return fog of war object
     */
    FogOfWar* getFOV();

private:
    /**
     * Screen where SDL is drawing on
     */
    SDL_Surface* screen;

    /**
     * Fog of war object
     */
    FogOfWar* fov;

    /**
     * Data object for whole game
     */
    GameData data;

    /**
     * Pointer to games map
     */
    Map* map;

    /**
     * Pointer to vector of games buildings
     */
    std::vector<Building*>* buildings;

    /**
     * Pointer to vector of games units
     */
    std::vector<Unit*>*  units;

    /**
     * Pointer to vector of games players
     */
    std::vector<Player*>* players;

    /**
     * Start time of announcements
     */
    int* announce;

    /**
     * A timer parallel to game loop
     */
    Timer* timer;

    /**
     * Pointer to announcement text
     */
    std::string* announceText;

    /**
     * Pointer to current active Tooltip
     */
    Tooltip* activeTooltip;

    /**
     * Pointer to renderer object
     */
    GLRenderer* renderer;
};

#endif /* defined(__Clash__GameState__) */
