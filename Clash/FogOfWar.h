/*
 * File:   FogOfWar.h
 * Created on May 4, 2011
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
 * Defintion of FogOfWar class
 */

#ifndef FOGOFWAR_H
#define	FOGOFWAR_H

#include <vector>
#include "clipper.hpp"

class GameState;

/**
 * Class manages fog of war for client player
 */
class FogOfWar {
public:
    /**
     * Constructor, sets gamestate object and initializing FOV
     * objects as well as calculating first instance.
     */
    FogOfWar();

    /**
     * Destructor
     */
    virtual ~FogOfWar();

    /**
     * Drawing the fog of war directly on screen
     */
    void Draw();

    /**
     * Setting state of the game object
     *
     * @param gameState state of the game object
     */
    void setGameState(GameState* gameState);

    /**
     * Returns if there is fog on a tile or not
     *
     * @param x x coordinate of tile
     * @param y y coordinate of tile
     * @return bool if fog on there
     */
    bool isFog(int x, int y);

    /**
     * This method is recalculating field of view of the clients
     * player.It gets called every frame, sets everything to false
     * and looks sets the field of view to those where current
     * units and building placement can see.
     */
    void UpdateFOV();

    /**
     * Gets the clipped polygon
     *
     * @return clipped polygon
     */
    clipper::Polygons getSolution();

private:
    /**
     * Polygon base object
     */
	clipper::Polygon subject;

	/**
	 *
     * Polygon clip object, which will be substracted from subject
	 */
	clipper::Polygon clip;

	/**
	 * Polygons object contains result of clipping calculations
	 */
	clipper::Polygons solution;

	/**
	 * Clipper
	 */
	clipper::Clipper c;

    /**
     * Tracks the field of view of client user
     */
    std::vector< std::vector<bool> > fieldOfView;

    /**
     * Tracks if something is even explored
     */
    std::vector< std::vector<bool> > explored;

    /**
     * State of the game object pointer
     */
    GameState* gameState;

    /**
     * Returns if tile is unexplored
     *
     * @param x x coordinate of tile
     * @param y y coordinate of tile
     * @return bool if tile is unexplored
     */
    bool isUnexplored(int x, int y);
};

#endif	/* FOGOFWAR_H */

