/*
 * File:   GLRenderer.h
 * Created on May 16, 2011
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
 * GLRenderer definitions
 */

#ifndef GLRENDERER_H_
#define GLRENDERER_H_

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <string>
#include <vector>

class GameObject;
class GameState;
class Texture;

/**
 * Handles graphics engine of the game.
 */
class GLRenderer {
private:
	/**
	 * Object holds background of start menu
	 */
	int startScreenMap[16][20];

	/**
	 * Holds textures
	 */
	Texture* textures[128];

	/**
	 * Holds fonts
	 */
	TTF_Font* fonts[10];

	/**
	 * GameState object
	 */
	GameState* gameState;

	/**
	 * Screen resolution width
	 */
	int targetWidth;

	/**
	 * Screen resolution height
	 */
	int targetHeight;

	/**
	 * Mouse position x
	 */
	int *mouseX;

	/**
	 * Mouse position y
	 */
	int *mouseY;

	/**
	 * Index of display lists
	 */
	GLuint index;

	/**
	 * Draws a minimap tile on screen
	 *
	 * @param x position of tile
	 * @param y position of tile
	 * @param color color code block should get
	 * @param size size of tile (for buildings)
	 */
    void DrawMinimapPoint(int x, int y, float color[], float size = 1);

    /**
     * Draws buttons in buildmenu on screen
     *
     * It's needed as certain method, because the build menu buttons
     * need several DrawTexture invokes, meaning the background paper
     * image as well as the building itself and it's greyscale texture.
     *
     * @param x position
     * @param y position
     * @param textureID id of building
     */
    void DrawBuildMenuButtons(int x, int y, int textureID);
public:
    /**
     * Constructor, initializing OpenGL part of the renderer
     *
     * @param targetWidth resolution width
     * @param targetHeight resolution height
     * @param gameState state of the game object
     */
	GLRenderer(int targetWidth, int targetHeight, GameState* gameState);

	/**
	 * Destroys textures and fonts. It should only be called at the
	 * end of the application, as the GLRenderer is kind of Singleton
	 * which is used by whole application, but still I wrote it to
	 * avoid future problems and stupid messages by Valgrind.
	 */
	virtual ~GLRenderer();

	/**
	 * Camera position
	 */
	int cameraX;

	/**
	 * Camera position
	 */
	int cameraY;

	/**
	 * Scroll position used by start menu
	 */
	int startScreenScroll;

	/**
	 * Getting camera position X-wise
	 *
	 * @return camera position
	 */
	int getCameraX() const;

	/**
	 * Getting camera position Y-wise
	 *
	 * @return camera position
	 */
	int getCameraY() const;

	/**
	 * Getting mouse position X-wise
	 *
	 * @return mouse position
	 */
	int getMouseX() const;

	/**
	 * Getting mouse position Y-wise
	 *
	 * @return mouse position
	 */
	int getMouseY() const;

	/**
	 * Set the camera position to a new one. Does not work very good at the moment,
	 * or at all by that means.
	 *
	 * @param x position to set to
	 */
	void setCameraX(int x);

	/**
	 * Set the camera position to a new one. Does not work very good at the moment,
	 * or at all by that means.
	 *
	 * @param y position to set to
	 */
	void setCameraY(int y);

	/**
	 * Should only be used by game class to give GLRenderer a pointer to the mouseX,
	 * mouseY objects created by SDLs Event Handler
	 *
	 * @param mouseX pointer to mouseX
	 * @param mouseY pointer to mouseY
	 */
	void setMousePointer(int* mouseX, int* mouseY);

	/**
	 * Setting to a new screen resolution. Does not work at the moment.
	 *
	 * @param w width of new resolution
	 * @param h height of new resolution
	 */
	void setResolution(int w, int h);

	/**
	 * Used to move camera by an given absolute value. It is not designed
	 * to put the camera to the position (X/Y) on Grid but on moving it
	 * x or y pixels to the right or down (or left/up if used with
	 * negative parameters).
	 *
	 * @param x pixels to move left
	 * @param y pixels to move down
	 */
	void moveCamera(int x, int y);

	/**
	 * Draws defeat/win screen
	 *
	 * @param id id of the type
	 * @return nothing
	 */
	int DrawEnd(int id);

	/**
	 * Draws start menu at the beginning of the game
	 *
	 * @param id the id of the menu where at
	 * @return nothing
	 */
	int DrawStartMenu(int id);

	/**
	 * Draws a texture at given position
	 *
	 * @param x position
	 * @param y position
	 * @param textureID id of texture
	 * @param r red color value
	 * @param g green color value
	 * @param b blue color value
	 */
    void DrawTextures(int x, int y, int textureID, float r=1.0f, float g=1.0f, float b=1.0f, int orientation=0);

    /**
     * Drawing a string to the screen
     *
     * @TODO: This method should have a option to give a
     * position where it should be centered at.
     *
     * @param x position
     * @param y position
     * @param outputString string to write
     * @param fontId id of font
     * @param R red color value
     * @param G green color value
     * @param B blue color value
     */
    void DrawString(float x, float y, std::string outputString, int fontId, int R=255, int G=255, int B=255);

    /**
     * Drawing tiles on background of the screen aka the Map.
     *
     * @return nothing
     */
    int DrawTiles();

    /**
     * Drawing minimap in the bottom left. Copies the initialized
     * minimap from initMinimap() which holds the tiles (which don't
     * change) and places colors for buildings/units/etc. on it.
     *
     * @return nothing
     */
    int DrawMinimap();

    /**
     * Drawing the main UI of the game.
     *
     * @param buildMenu boolean if buildmenu tab is open
     * @param ao pointer to vector with selected stuff
     * @return nothing
     */
    int DrawUI(bool buildMenu, std::vector<GameObject*>* ao);

    /**
     * Drawing the building grid which colors indicating if you can
     * build here or not, together with sticking building texture on
     * your mouse pointer.
     *
     * @param id id of the building for which the grid is
     * @return nothing
     */
    int DrawBuildGrid(int id);

    /**
     * Drawing text massages on screen (called announcements on other
     * source files sometimes).
     *
     * @return nothing
     */
    int DrawText();

    /**
     * Drawing each kind of health and progress bars on the screen
     *
     * @param ao pointer to selected objects
     * @return nothing
     */
    int DrawBars(std::vector<GameObject*>* ao);

    /**
     * Drawing the box when you trying to select something with pressed mouse
     *
     * @param selectStartX coordinate of the place where mouse selection box started
     * @param selectStartY coordinate of the place where mouse selection box started
     * @return nothing
     */
    int DrawSelect(int selectStartX, int selectStartY);

    /**
     * Drawing Tooltips on the screen.
     *
     * @return nothing
     */
    int DrawTooltip();
};

#endif /* GLRENDERER_H_ */
