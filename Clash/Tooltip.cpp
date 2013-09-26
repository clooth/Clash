/*
 * File:   Tooltip.cpp
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
 * Tooltip class
 */
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <OpenGL/gl.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Tooltip.h"
#include "SDL_Functions.h"
#include "GameState.h"
#include "GLRenderer.h"

Tooltip::Tooltip(int id, GameState* gs) {
    this->gameState = gs;
    this->id = id;
    this->sizeWidth = 200;
    this->sizeHeight = 150;
}

int Tooltip::getSizeWidth() const {
    return this->sizeWidth;
}

int Tooltip::getSizeHeight() const {
    return this->sizeHeight;
}

void Tooltip::DrawGLTooltip() {
	int positionX = gameState->getRenderer()->getCameraX()+gameState->getRenderer()->getMouseX()-sizeWidth,
		positionY = gameState->getRenderer()->getCameraY()+gameState->getRenderer()->getMouseY()-sizeHeight;

	glPushMatrix();
	glTranslatef(positionX, positionY,0.0f);
	glColor4f(0.0f,0.0f,0.0f, 0.6f);
	glBegin(GL_QUADS);
		{
		glVertex2f(0.0f, 0.0f);
		glVertex2f(sizeWidth, 0.0f);
		glVertex2f(sizeWidth, sizeHeight);
		glVertex2f(0.0f, sizeHeight);
	}
	glEnd();
	glPopMatrix();

    std::string nameText = "Build " + gameState->getData()->getName(id);

    char goldText[256], gasText[256], supplyText[256], timeText[256];
    sprintf(goldText, "%d",gameState->getData()->getGoldPrices(id));
    sprintf(gasText, "%d",gameState->getData()->getGasPrices(id));
    sprintf(supplyText, "%d", gameState->getData()->getSupply(id));
    sprintf(timeText, "%d", gameState->getData()->getBuildTimes(id));

    std::string descriptionText1 = gameState->getData()->getDescription(id,0);
    std::string descriptionText2 = gameState->getData()->getDescription(id,1);
    std::string descriptionText3 = gameState->getData()->getDescription(id,2);
    std::string descriptionText4 = gameState->getData()->getDescription(id,3);

    gameState->getRenderer()->DrawString(positionX+10, positionY+10, nameText, 1, 255, 255, 255);
    gameState->getRenderer()->DrawString(positionX+30, positionY+32, goldText, 1, 255, 255, 255);
    gameState->getRenderer()->DrawString(positionX+80, positionY+32, gasText, 1, 255, 255, 255);
    gameState->getRenderer()->DrawString(positionX+130, positionY+32, supplyText, 1, 255, 255, 255);
    gameState->getRenderer()->DrawString(positionX+170, positionY+32, timeText, 1, 255, 255, 255);

    if(descriptionText1 != "") {
    	gameState->getRenderer()->DrawString(positionX+10, positionY+70, descriptionText1, 2, 255, 255, 255);
    }
    if(descriptionText2 != "") {
    	gameState->getRenderer()->DrawString(positionX+10, positionY+90, descriptionText2, 2, 255, 255, 255);
    }
    if(descriptionText3 != "") {
    	gameState->getRenderer()->DrawString(positionX+10, positionY+110, descriptionText3, 2, 255, 255, 255);
    }
    if(descriptionText4 != "") {
    	gameState->getRenderer()->DrawString(positionX+10, positionY+130, descriptionText4, 2, 255, 255, 255);
    }

    gameState->getRenderer()->DrawTextures(positionX+10, positionY+30, 101);
    gameState->getRenderer()->DrawTextures(positionX+60, positionY+30, 102);
    gameState->getRenderer()->DrawTextures(positionX+110, positionY+30, 108);
    gameState->getRenderer()->DrawTextures(positionX+150, positionY+30, 109);

    glPushMatrix();
	glTranslatef(positionX, positionY,0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	{
		glVertex2f(0.0f, 0.0f); glVertex2f(sizeWidth, 0.0f);
		glVertex2f(sizeWidth, 0.0f); glVertex2f(sizeWidth, sizeHeight);
		glVertex2f(sizeWidth, sizeHeight); glVertex2f(0.0f, sizeHeight);
		glVertex2f(0.0f, sizeHeight); glVertex2f(0.0f, 0.0f);
	}
	glEnd();
	glPopMatrix();
}

Tooltip::~Tooltip() {
}


