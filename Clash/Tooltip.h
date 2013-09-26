/*
 * File:   Tooltip.h
 * Created on April 30, 2011
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
 * Definition of Tooltips
 */

#ifndef TOOLTIP_H
#define	TOOLTIP_H

class GameState;

class Tooltip {
public:
    /**
     * Constructor, initializing attributes and calling updateTooltip
     *
     * @param id id of the thing displayed by tooltip
     * @param gs state of the game object
     */
    Tooltip(int id, GameState* gs);

    /**
     * Destructor, freeing memory and stuff
     */
    ~Tooltip();

    /**
     * Initializing tooltip surface
     */
    void DrawGLTooltip();

    /**
     * Returning width size of tooltip
     *
     * @return int
     */
    int getSizeWidth() const;

    /**
     * Returning width size of tooltip
     *
     * @return int
     */
    int getSizeHeight() const;
private:
    /**
     * State of the game object
     */
    GameState* gameState;

    /**
     * ID of the thing which is represented by Tooltips
     */
    int id;

    /**
     * Size of Tooltip
     */
    int sizeWidth;

    /**
     * Size of Tooltip
     */
    int sizeHeight;
};

#endif	/* TOOLTIP_H */

