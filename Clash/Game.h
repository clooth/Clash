/*
 * File:   Game.h
 * Created on April 5, 2011
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
 * Definition of Game class. Doing most of the inter-class communitications
 * as well as the drawings.
 */

#ifndef _Game_h_
#define _Game_h_

#include "Map.h"
#include "Building.h"
#include "Unit.h"
#include "Ressources.h"
#include "Timer.h"
#include "Player.h"
#include "Scripting.h"

#include <SDL/SDL.h>

namespace Clash {
    class Map;
}

class GLRenderer;
class GameState;
class Ressources;
class GameObject;
class Building;
class Unit;
class Tooltip;
class Audio;
class FogOfWar;

/**
 * Main object of the game. It starts everything, it controlls a lot of things
 * If you're searching something or try to unterstand the games source code,
 * start here.
 */
class Game {
private:
    /**
     * The active state of the game
     */
    GameState *m_game_state;

    /**
     * Renderer Object
     */
    GLRenderer* render;

    /**
     * Fog of War Object
     */
    FogOfWar* fogOfWar;

    /**
     * Map of the current game.
     */
    Clash::Map m;

    /**
     * Vector which gets a list of each unit or building the user selects
     * by clicking on them.
     */
    std::vector<GameObject*> activatedObject;

    /**
     * Pointer to current active surface
     */
    Tooltip* activeTooltip;

    /**
     * Vector which represents a list of units in game
     */
    std::vector<Unit*> units;

    /**
     * Vector which represents a list of buildings in game
     */
    std::vector<Building*> buildings;

    /**
     * Vector which represents a list of textures in game
     */
    std::vector<std::vector<SDL_Surface* > > textures;

    /**
     * Vector which represents a list of players in game.
     */
    std::vector<Player*> players;

    /**
     * Main screen object, where everything is being drawn on.
     */
    SDL_Surface* screen;

    /**
     * Object for input event handling
     */
    SDL_Event event;

    /**
     * Timer for Frames
     */
    Timer gameTimer;

    /**
     * Timer for Announcements
     */
    Timer timer;

    /**
     * Cancels gameloop if true
     */
    bool quit;

    /**
     * If mouse is pressed (for minimap movements)
     */
    bool mousePressed;

    /**
     * If buildGrid should be displayed
     * (== you currently want to build something)
     */
    bool buildGrid;

    /**
     * If buildMenu is open
     */
    bool buildMenu;

    /**
     * Set if ingame
     */
    bool ingame;

    /**
     * Startmenu ID
     */
    int startMenuID;

    /**
     * Key states so we can do if statements with "shift pressed?" or something
     */
    Uint8 *keystates;

    /**
     * Camera positions
     */
    int currPosY, currPosX;

    /**
     * Needed for throttling mouse motions a little bit
     */
    int mouseMotionCounter;

    /**
     * Saves which building you are currently trying to build
     */
    int buildMenuID;

    /**
     * Gets time where you start announcements
     */
    int announce;

    /**
     * Position of mouse
     */
    int mouseX, mouseY;

    /**
     * Another timer, not using Timer class, for test purposes.
     */
    float FrameTime;

    /**
     * Saves the text which DrawText puts on screen. Can get
     * called from outside of this object using GameState
     */
    std::string announceText;

    /**
     * When you start a box with your left mousebotton pressed,
     * this attribute changes to true;
     */
    bool mouseSelect;

    /**
     * When you start making a selection box with your left
     * mousebutton pressed, this attributes save the position
     * where your box started.
     */
    int selectStartX, selectStartY;

    /**
     * For Buildmenu
     */
    Tooltip *buildMenuTT[6];

    /**
     * Toggle ingame console
     */
    bool console;

    /**
     * Object for scripting engine (LUA based)
     */
	Scripting s;

    /**
     * Called every loop from Game loop, calculates movements and
     * other game mechanics.
     */
    void Calculate();

    /**
     * Should clean up stuff when quitting application, does nothing yet.
     * 
     * @return nothing
     */
    int Clean();

    /**
     * Called by game loop, calls everything which has to do with every
     * frame drawings.
     * 
     * @return nothing
     */
    int Update();

    /**
     * Initializing the minimap (tile-based) and saves it in a surface.
     *
     * @return nothing
     */
    int InitMinimap();

    /**
     * Handles every input event of the game
     */
    void EventInput();

    /**
     * Handles per frame events of the game
     */
    void FrameInput();

    /**
     * Handles input in build menu
     *
     * @param type type of the call, 1=motion, 2=click
     */
    void BuildMenuInput(int type);

    /**
     * Check if a values are between screen width/height boundraries
     * @param condition1 one value
     * @param condition2 another values
     * @return bool
     */
    bool checkWithinBorders(int condition1, int condition2);

    /**
     * Placing down a unit somewhere on the map. Will be called when
     * buildgrid is on and you click on the map.
     * 
     * @return nothing
     */
    int placeBuilding();

public:
    /**
     * Constructor, initializing attributes.
     */
    Game();
    /**
     * Destructor
     */
    ~Game();
    /**
     * Initizalizing game object, also starting game loop.
     *
     * @return -1 if something occurs. Normally it should return nothing until application closed.
     */
    int init();
};

#endif /* _Game_h_ */
