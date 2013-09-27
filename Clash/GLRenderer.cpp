/*
 * File:   GLRenderer.cpp
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
 * GLRenderer class
 */
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
#endif

#include <sstream>
#include <cmath>
#include <queue>

#include "GLRenderer.h"
#include "Map.h"
#include "GameState.h"
#include "Texture.h"
#include "MapTile.h"
#include "FogOfWar.h"
#include "Building.h"
#include "Unit.h"
#include "Player.h"
#include "GameObject.h"
#include "Tooltip.h"
#include "Timer.h"
#include "GitSHA1.h"

#include "clipper.hpp"

GLRenderer::GLRenderer(int targetWidth, int targetHeight, GameState* gameState) {
	// Filling background of the start menu, RANDOMIZED TREES!
	for(int i=0; i<16; i++) {
		for(int j=0; j<20; j++) {
			int id = 0;
			if(rand()%4==3) id = 2;
			startScreenMap[i][j] = id;
		}
	}
	startScreenScroll = 0;

	mouseX = 0;
	mouseY = 0;

	this->gameState = gameState;
	this->targetWidth = targetWidth;
	this->targetHeight = targetHeight;
	this->cameraX = this->cameraY = 0;

	// creating 10 display lists
	index = glGenLists(10);

	// init opengl
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, targetWidth, targetHeight);
	glOrtho(0, targetWidth, targetHeight, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	// setting null pointers for every texture to avoid future problems
	for(int i = 0; i < 128; i++)
		textures[i] = NULL;

	// loading textures
	textures[0] = new Texture("images/tiles/grass.png");
	textures[1] = new Texture("images/tiles/water.png");
	textures[2] = new Texture("images/tiles/pine_woods.png");
	textures[3] = new Texture("images/buildings/gold.png");
	textures[4] = new Texture("images/tiles/fog.png");

	textures[21] = new Texture("images/buildings/commandcenter.png");
	textures[61] = new Texture("images/buildings/commandcenter_grayscale.png");

	textures[22] = new Texture("images/buildings/barracks.png");
	textures[62] = new Texture("images/buildings/barracks_grayscale.png");

	textures[23] = new Texture("images/buildings/supplydepot.png");
	textures[63] = new Texture("images/buildings/supplydepot_grayscale.png");

	textures[24] = new Texture("images/buildings/researchlab.png");
	textures[64] = new Texture("images/buildings/researchlab_grayscale.png");

	textures[25] = new Texture("images/buildings/factory.png");
	textures[65] = new Texture("images/buildings/factory_grayscale.png");

	textures[26] = new Texture("images/buildings/milairport.png");
	textures[66] = new Texture("images/buildings/milairport_grayscale.png");

	textures[27] = new Texture("images/buildings/tower.png");
	textures[67] = new Texture("images/buildings/tower_grayscale.png");

	textures[41] = new Texture("images/units/soldier.png");
	textures[81] = new Texture("images/units/soldier_grayscale.png");

	textures[42] = new Texture("images/units/worker.png");
	textures[82] = new Texture("images/units/worker_grayscale.png");

	textures[101] = new Texture("images/ui/gold.png");
	textures[102] = new Texture("images/ui/gas.png");
	textures[105] = new Texture("images/ui/buildmenu_button.png");
	textures[106] = new Texture("images/ui/buildmenu_cc.png");
	textures[107] = new Texture("images/ui/buildmenu_barracks.png");
	textures[108] = new Texture("images/ui/supply.png");
	textures[109] = new Texture("images/ui/time.png");

	textures[110] = new Texture("images/ui/gold.png"); // deprecated

	textures[111] = new Texture("images/ui/buildmenu_supplydepot.png");
	textures[112] = new Texture("images/ui/buildmenu_researchlab.png");
	textures[113] = new Texture("images/ui/buildmenu_factory.png");
	textures[114] = new Texture("images/ui/buildmenu_milairport.png");
	textures[115] = new Texture("images/ui/buildmenu_tower.png");
	textures[116] = new Texture("images/ui/map.png");
	textures[117] = new Texture("images/ui/action.png");
	textures[118] = new Texture("images/ui/middle.png");
	textures[119] = new Texture("images/ui/barlong.png");
	textures[120] = new Texture("images/ui/prog.png");
	textures[121] = new Texture("images/ui/paper2.png");
	textures[122] = new Texture("images/ui/start_button.png");

	textures[123] = new Texture("images/ui/buildmenu_bg.png");
	textures[124] = new Texture("images/ui/buildmenu_paper.png");

	// setting null pointers for every font to avoid future problems
	for(int i = 0; i < 10; i++)
		fonts[i] = NULL;

	fonts[0] = TTF_OpenFont("fonts/building.ttf", 14); // Ressources
	fonts[1] = TTF_OpenFont("fonts/Vera.ttf", 13); // Tooltip title
	fonts[2] = TTF_OpenFont("fonts/Vera.ttf", 9); // Tooltip text
	fonts[3] = TTF_OpenFont("fonts/building.ttf", 24); // Selected buildings/unit title and start menu
	fonts[4] = TTF_OpenFont("fonts/building.ttf", 10); // Buildmenu Button
	fonts[5] = TTF_OpenFont("fonts/ChopinScript.otf", 116); // Start menu

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

GLRenderer::~GLRenderer() {
	for ( int i = 0; i < 128; i++ ) {
		if(textures[i] != NULL)
			delete textures[i];
	}

	for ( int i = 0; i < 10; i++ ) {
		if(fonts[i] != NULL)
			TTF_CloseFont(fonts[i]);
	}
}

int GLRenderer::getCameraX() const {
	return this->cameraX;
}

int GLRenderer::getCameraY() const {
	return this->cameraY;
}

int GLRenderer::getMouseX() const {
	return (*this->mouseX);
}

int GLRenderer::getMouseY() const {
	return (*this->mouseY);
}

void GLRenderer::moveCamera(int x, int y) {
	glTranslatef(-x, -y, 0);
}

void GLRenderer::setCameraX(int x) {
	glTranslatef(cameraX, 0, 0);
	cameraX = x;
	glTranslatef(-x, 0, 0);
}
void GLRenderer::setCameraY(int y) {
	glTranslatef(0, cameraY, 0);
	cameraY = y;
	glTranslatef(0, -y, 0);
}

void GLRenderer::setMousePointer(int* mouseX, int* mouseY) {
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void GLRenderer::DrawBuildMenuButtons(int x, int y, int textureID) {
	// Paper background
	DrawTextures(x, y, 124);

	// Scale factor
	float scale = 0.3,
		  offsetX = 5.0f+(2.0f/(float)gameState->getData()->getSize(textureID))*20.0f,
		  offsetY = 10.0f+(2.0f/(float)gameState->getData()->getSize(textureID))*20.0f;

	glPushMatrix();
	glTranslatef(x+offsetX,y+offsetY,0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D,textures[textureID]->getTexture());
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(0.0f,0.0f);
		glTexCoord2f(1,0);
		glVertex2f(0.0f+textures[textureID]->getTextureWidth()*scale,0.0f);
		glTexCoord2f(1,1);
		glVertex2f(0.0f+textures[textureID]->getTextureWidth()*scale,0.0f+textures[textureID]->getTextureHeight()*scale);
		glTexCoord2f(0,1);
		glVertex2f(0.0f,0.0f+textures[textureID]->getTextureHeight()*scale);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x+offsetX,y+offsetY,0);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBindTexture(GL_TEXTURE_2D,textures[textureID+40]->getTexture());
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(0.0f,0.0f);
		glTexCoord2f(1,0);
		glVertex2f(0.0f+textures[textureID+40]->getTextureWidth()*scale,0.0f);
		glTexCoord2f(1,1);
		glVertex2f(0.0f+textures[textureID+40]->getTextureWidth()*scale,0.0f+textures[textureID+40]->getTextureHeight()*scale);
		glTexCoord2f(0,1);
		glVertex2f(0.0f,0.0f+textures[textureID+40]->getTextureHeight()*scale);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void GLRenderer::DrawTextures(int x, int y, int textureID, float r, float g, float b, int orientation) {
	glPushMatrix();

	glTranslatef(x,y,0);
	glColor3f(r,g,b);

	glBindTexture(GL_TEXTURE_2D,textures[textureID]->getTexture());
	glBegin(GL_QUADS);
	{
		if(orientation == 0) glTexCoord2f(0,0);
		else glTexCoord2f(1,0);
		glVertex2f(0.0f,0.0f);

		if(orientation == 0) glTexCoord2f(1,0);
		else glTexCoord2f(0,0);
		glVertex2f(0.0f+textures[textureID]->getTextureWidth(),0.0f);

		if(orientation == 0) glTexCoord2f(1,1);
		else glTexCoord2f(0,1);
		glVertex2f(0.0f+textures[textureID]->getTextureWidth(),0.0f+textures[textureID]->getTextureHeight());

		if(orientation == 0) glTexCoord2f(0,1);
		else glTexCoord2f(1,1);
		glVertex2f(0.0f,0.0f+textures[textureID]->getTextureHeight());
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

int GLRenderer::DrawText() {

	return 0;
}

void GLRenderer::DrawMinimapPoint(int x, int y, float color[], float size) {
	glPushMatrix();

	glTranslatef(cameraX+30+x*2,cameraY+targetHeight-250+y*2,0);
	if(this->gameState->getFOV()->isFog(x,y))
		glColor3f(color[0]*0.5, color[1]*0.5, color[2]*0.5);
	else
		glColor3f(color[0], color[1], color[2]);

	glBegin(GL_QUADS);
	{
		glVertex2f(0.0f,0.0f);
		glVertex2f(2.0f*size,0.0f);
		glVertex2f(2.0f*size,2.0f*size);
		glVertex2f(0.0f,2.0f*size);
	}
	glEnd();

	glPopMatrix();
}

int GLRenderer::DrawMinimap() {
	// Tiles
	for(int i = 0; i<110; i++) {
		for(int j = 0; j<110; j++) {
			DrawMinimapPoint(i,j,gameState->getMap()->tile_color(i, j));
		}
	}

	// Buildings
	for(unsigned int i = 0; i < gameState->getBuildings()->size(); i++) {
		DrawMinimapPoint(gameState->getBuildings()->at(i)->getX(),
						 gameState->getBuildings()->at(i)->getY(),
						 gameState->getBuildings()->at(i)->getMColor(),
						 gameState->getBuildings()->at(i)->getSize());
	}

	// Units
	for(unsigned int i = 0; i < gameState->getUnits()->size(); i++) {
		DrawMinimapPoint(gameState->getUnits()->at(i)->getX()/64,
						 gameState->getUnits()->at(i)->getY()/64,
						 gameState->getUnits()->at(i)->getMColor(),
						 gameState->getUnits()->at(i)->getSize());
	}

	// Camera
	glPushMatrix();

	glTranslatef(cameraX+30+cameraX/32,cameraY+targetHeight-250+cameraY/32,0);
	glColor3f(1.0f,1.0f,1.0f);

	glBegin(GL_LINES);
	{
		glVertex2f(0.0f,0.0f); glVertex2f(32.0f,0.0f);
		glVertex2f(32.0f,0.0f); glVertex2f(32.0f,24.0f);
		glVertex2f(32.0f,24.0f); glVertex2f(0.0f,24.0f);
		glVertex2f(0.0f,24.0f); glVertex2f(0.0f,0.0f);
	}
	glEnd();

	glPopMatrix();
	return 0;
}

int GLRenderer::DrawTiles() {
	// Tiles
	for(int i = cameraX/64; i<(cameraX+targetWidth+64)/64; i+=1) {
		for(int j = cameraY/64; j<(cameraY+targetHeight+64)/64; j+=1) {
			if(j < gameState->getMap()->get_size() && i < gameState->getMap()->get_size()) {
				DrawTextures(i*64,j*64,this->gameState->getMap()->tile_id(i,j));
			}
		}
	}

	// Buildings
	for(unsigned int k = 0; k < gameState->getBuildings()->size(); k++) {
		if( gameState->getBuildings()->at(k)->getX()*64 > cameraX-192 &&
			gameState->getBuildings()->at(k)->getX()*64 < cameraX + targetWidth &&
			gameState->getBuildings()->at(k)->getY()*64 > cameraY-192 &&
			gameState->getBuildings()->at(k)->getY()*64 < cameraY + targetHeight) {

			// getting owner ID of this building (for colorizing)
			int ownerID = 0;
			if(gameState->getBuildings()->at(k)->getOwner() != NULL) {
				ownerID = gameState->getBuildings()->at(k)->getOwner()->getID();
			}

			// normal building texture
			DrawTextures(gameState->getBuildings()->at(k)->getX()*64,
						 gameState->getBuildings()->at(k)->getY()*64,
						 gameState->getBuildings()->at(k)->getID());

			// colorized grayscale texture
			if(gameState->getBuildings()->at(k)->getOwner() != NULL) {
				float* color = gameState->getPlayers()->at(ownerID)->getColor();
				DrawTextures(gameState->getBuildings()->at(k)->getX()*64,
							 gameState->getBuildings()->at(k)->getY()*64,
							 gameState->getBuildings()->at(k)->getID()+40,
							 color[0], color[1], color[2]);
			}
		}
	}

	// Units
	for(unsigned int k = 0; k < gameState->getUnits()->size(); k++) {
		if( gameState->getUnits()->at(k)->getX() > cameraX-192 &&
			gameState->getUnits()->at(k)->getX() < cameraX + targetWidth &&
			gameState->getUnits()->at(k)->getY() > cameraY-192 &&
			gameState->getUnits()->at(k)->getY() < cameraY + targetHeight) {

			// getting owner ID of this building (for colorizing)
			int ownerID = 0;
			if(gameState->getUnits()->at(k)->getOwner() != NULL) {
				ownerID = gameState->getUnits()->at(k)->getOwner()->getID();
			}

			// this if clause permits drawing units that can't been seen.
			if(!gameState->getFOV()->isFog(gameState->getUnits()->at(k)->getX()/64,gameState->getUnits()->at(k)->getY()/64)) {
				// normal unit texture is drawn
				DrawTextures(gameState->getUnits()->at(k)->getX(),
							 gameState->getUnits()->at(k)->getY(),
							 gameState->getUnits()->at(k)->getID(),
							 1,1,1,
							 gameState->getUnits()->at(k)->getOrientation());

				// colorized unit grayscale texture is drawn on top of it
				if(gameState->getUnits()->at(k)->getOwner() != NULL) {
					float* color = gameState->getPlayers()->at(ownerID)->getColor();
					DrawTextures(gameState->getUnits()->at(k)->getX(),
								 gameState->getUnits()->at(k)->getY(),
								 gameState->getUnits()->at(k)->getID()+40,
								 color[0], color[1], color[2],
								 gameState->getUnits()->at(k)->getOrientation());
				}
			}
		}
	}

	gameState->getFOV()->UpdateFOV();

	return 0;
}


int GLRenderer::DrawUI(bool buildMenu, std::vector<GameObject*>* ao) {
	std::stringstream ss;

	// ressources icons at top right of the screen
	DrawTextures(cameraX+targetWidth-300, cameraY+10, 101);
	DrawTextures(cameraX+targetWidth-200, cameraY+10, 102);
	DrawTextures(cameraX+targetWidth-100, cameraY+10, 108);

	// gold ressources text
	ss << gameState->getPlayers()->at(0)->getRessources()->getGold();
	DrawString(cameraX+targetWidth-270, cameraY+10, ss.str(),0);
	ss.str("");
	ss.clear();

	// gas ressources text
	ss << gameState->getPlayers()->at(0)->getRessources()->getGas();
	DrawString(cameraX+targetWidth-170, cameraY+10, ss.str(),0);
	ss.str("");
	ss.clear();

	// supply ressources text
	ss << gameState->getPlayers()->at(0)->getRessources()->getCurrentSupply(gameState) << "/" << gameState->getPlayers()->at(0)->getRessources()->getMaxSupply(gameState);
	DrawString(cameraX+targetWidth-70, cameraY+10, ss.str(),0);
	ss.str("");
	ss.clear();

	// menu at bottom
	DrawTextures(cameraX,cameraY+targetHeight-280,116);
	DrawTextures(cameraX+280, cameraY+targetHeight-240, 118);
	DrawTextures(cameraX+targetWidth-240, cameraY+targetHeight-240, 117);

	// Buildmenu closed
	if(!buildMenu) {
		DrawTextures(cameraX+targetWidth-32, cameraY+targetHeight-580, 123);
		DrawString(cameraX+targetWidth-15, cameraY+targetHeight-332, "B",4);
		DrawString(cameraX+targetWidth-16, cameraY+targetHeight-319, "U",4);
		DrawString(cameraX+targetWidth-14, cameraY+targetHeight-307, "I",4);
		DrawString(cameraX+targetWidth-15, cameraY+targetHeight-294, "L",4);
		DrawString(cameraX+targetWidth-16, cameraY+targetHeight-279, "D",4);

	// Buildmenu open
	} else {
		DrawTextures(cameraX+targetWidth-282, cameraY+targetHeight-580, 123);
		DrawString(cameraX+targetWidth-265, cameraY+targetHeight-332, "B",4);
		DrawString(cameraX+targetWidth-266, cameraY+targetHeight-319, "U",4);
		DrawString(cameraX+targetWidth-264, cameraY+targetHeight-307, "I",4);
		DrawString(cameraX+targetWidth-265, cameraY+targetHeight-294, "L",4);
		DrawString(cameraX+targetWidth-266, cameraY+targetHeight-279, "D",4);

		// Buildings
		DrawBuildMenuButtons(cameraX+targetWidth-230, cameraY+targetHeight-555, 21);
		DrawBuildMenuButtons(cameraX+targetWidth-120, cameraY+targetHeight-555, 22);
		DrawBuildMenuButtons(cameraX+targetWidth-230, cameraY+targetHeight-460, 23);
		DrawBuildMenuButtons(cameraX+targetWidth-120, cameraY+targetHeight-460, 24);
		DrawBuildMenuButtons(cameraX+targetWidth-230, cameraY+targetHeight-365, 25);
		DrawBuildMenuButtons(cameraX+targetWidth-120, cameraY+targetHeight-365, 26);
	}

	// A building is selected
	if(ao->size() == 1 && ao->at(0) != NULL) {
		// Need to use two ifs, because when first element in ao is null, it
		// would throw me an exception when I am trying to use getID()
		int ID = ao->at(0)->getID();

		if(ID >= 20 && ID <= 39 && ao->at(0)->getOwner() == gameState->getPlayers()->at(0)) {
			// Name of building
			std::string text = this->gameState->getData()->getName(ID);
			DrawString(cameraX+(targetWidth/2)-30,cameraY+targetHeight-240+30, text, 3, 255, 255, 255);

			// Stuff to display when not in build
			if(((Building*)ao->at(0))->buildBuilding() == -1) {
				// Actions
				if(ID == 21) {
					DrawTextures(cameraX+targetWidth-240+22, cameraY+targetHeight-240+23, 42);
					float* color = gameState->getPlayers()->at(0)->getColor();
					DrawTextures(cameraX+targetWidth-240+22, cameraY+targetHeight-240+23, 82, color[0], color[1], color[2]);
				} else if(ID == 22) {
					DrawTextures(cameraX+targetWidth-240+22, cameraY+targetHeight-240+23, 41);
					float* color = gameState->getPlayers()->at(0)->getColor();
					DrawTextures(cameraX+targetWidth-240+22, cameraY+targetHeight-240+23, 81, color[0], color[1], color[2]);
				}
				std::queue<BuildUnit> bu = ((Building*)ao->at(0))->unitBuild;

				if(!bu.empty()) {
					int positionX = cameraX+(targetWidth/2)-180,
						positionY = cameraY+targetHeight-130;

					for(unsigned int i = 0; i < bu.size(); i++) {
						DrawTextures(positionX, positionY-40, 121);
						DrawTextures(positionX+8, positionY-20, bu.front().id);
						DrawTextures(positionX+8, positionY-20, bu.front().id+40);
						if(i==0) { positionY+=80; }
						else { positionX+=70; }
					}

					int bX = cameraX+(targetWidth/2)-110,
						bY = cameraY+targetHeight-150;

					DrawTextures(bX, bY, 119);

					float percent = (gameState->getTimer()->get_ticks()-bu.front().startTime)/bu.front().buildTime*100;

					glPushMatrix();
					glTranslatef(bX+40,bY+14,0);
					glColor3f(1.0f, 1.0f, 1.0f);

					glBindTexture(GL_TEXTURE_2D,textures[120]->getTexture());
					glBegin(GL_QUADS);
					{
						glTexCoord2f(0,0);
						glVertex2f(0.0f,0.0f);

						glTexCoord2f(1,0);
						glVertex2f(2.2f*percent, 0.0f);

						glTexCoord2f(1,1);
						glVertex2f(2.2f*percent,20.0f);

						glTexCoord2f(0,1);
						glVertex2f(0.0f,20.0f);
					}
					glEnd();
					glBindTexture(GL_TEXTURE_2D, 0);

					glPopMatrix();

				}
			} else { // Building Progress bar
				int bX = cameraX+(targetWidth/2)-130,
					bY = cameraY+targetHeight-160;

				DrawTextures(bX, bY, 119);

				float percent = ((Building*)ao->at(0))->buildBuilding();

				glPushMatrix();
				glTranslatef(bX+40,bY+14,0);
				glColor3f(1.0f, 1.0f, 1.0f);

				glBindTexture(GL_TEXTURE_2D,textures[120]->getTexture());
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0,0);
					glVertex2f(0.0f,0.0f);

					glTexCoord2f(1,0);
					glVertex2f(2.2f*percent, 0.0f);

					glTexCoord2f(1,1);
					glVertex2f(2.2f*percent,20.0f);

					glTexCoord2f(0,1);
					glVertex2f(0.0f,20.0f);
				}
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);

				glPopMatrix();

				DrawString(bX+50, bY+55, "This building is constructing..", 0, 255, 255, 255);
			}
		}
	}
	if(ao->size() >= 1 && ao->at(0) != NULL) {
		int positionX = cameraX+325,
			positionY = cameraY+targetHeight-215;
		int counter = 0;
		for(unsigned int i=0; i < ao->size(); i++) {
			int ID = ao->at(i)->getID();
			if(counter == 7) {
				counter = 0;
				positionY += 100;
				positionX = cameraX+325;
			}
			if(ID >= 40 && ID <= 59)  {
				DrawTextures(positionX, positionY, 121);
				DrawTextures(positionX+8, positionY+20, ID);
				DrawTextures(positionX+8, positionY+20, ID+40);

				float quotient = (float)ao->at(i)->getCurrentLife()/(float)ao->at(i)->getLife();

				glPushMatrix();
				glTranslatef(positionX+5.0f, positionY+64.0f, 0.0f);
				glColor3f(0.0f,1.0f,0.0f);
				glBegin(GL_QUADS);
				{
					glVertex2f(0.0f,0.0f);
					glVertex2f(39.0f*quotient,0.0f);
					glVertex2f(39.0f*quotient,3.0f);
					glVertex2f(0.0f,3.0f);
				}
				glEnd();
				glPopMatrix();

				positionX += 60;
				counter++;
			}
		}
	}

	return 0;
}

void GLRenderer::DrawString(float x, float y, std::string outputString, int fontId, int R, int G, int B) {
	SDL_Color Color;
	Color.r = R; Color.g = G; Color.b = B;
	SDL_Surface *Message = TTF_RenderText_Blended(fonts[fontId], outputString.c_str(), Color);
	unsigned Texture = 0;

	//Generate an OpenGL 2D texture from the SDL_Surface*.
	glGenTextures(1, &Texture);
	glColor3f(1.0f,1.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// In windows, GL_BGR is an extension while is ain't in other environments.
#ifdef _WIN32
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Message->pixels);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, Message->pixels);
#endif

	// Draw this texture on a quad with the given xyz coordinates.
	glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2d(x, y);
		glTexCoord2d(1, 0); glVertex2d(x+Message->w, y);
		glTexCoord2d(1, 1); glVertex2d(x+Message->w, y+Message->h);
		glTexCoord2d(0, 1); glVertex2d(x, y+Message->h);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	// Clean up.
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);
}

int GLRenderer::DrawBars(std::vector<GameObject*>* ao) {
	// Selected buildings and units
	for(unsigned int i = 0; i < ao->size(); i++) {
		if(ao->at(i) != NULL) {
			float size = 64.0f*ao->at(i)->getSize();
			glPushMatrix();

			// We need this if clause, because building coordinates are
			// based on the map grid, while unit coordinates are based on
			// x/y coordinates in OpenGL.
			if((ao->at(i)->getID() >=20 && ao->at(i)->getID() <= 39) || ao->at(i)->getID() == 3) {
				glTranslatef(ao->at(i)->getX()*64,ao->at(i)->getY()*64,0);
			} else {
				glTranslatef(ao->at(i)->getX(),ao->at(i)->getY(),0);
			}
			glColor3f(0.0f,1.0f,1.0f);

			// Draws the box around the Game Object
			glBegin(GL_LINES);
			{
				glVertex2f(0.0f,0.0f); glVertex2f(size,0.0f);
				glVertex2f(size,0.0f); glVertex2f(size,size);
				glVertex2f(size,size); glVertex2f(0.0f,size);
				glVertex2f(0.0f,size); glVertex2f(0.0f,0.0f);
			}
			glEnd();

			glPopMatrix();
		}
	}

	// This for statement goes through each selected
	// unit or building and draws it's health bar.
	for(unsigned int i = 0; i<ao->size(); i++) {
		if(ao->at(i) != NULL) {
			int X = ao->at(i)->getX(),
				Y = ao->at(i)->getY();

			float size = 64.0f*ao->at(i)->getSize()-10.0f,
				  quotient = (float)ao->at(i)->getCurrentLife()/(float)ao->at(i)->getLife();

			// If its an building and if it's still in build, do not
			// display the health bars
			bool help = true;
			if(ao->at(i)->getID() >= 20 && ao->at(i)->getID() <= 39) {
				if(((Building*)ao->at(i))->buildBuilding() != -1) {
					help = false;
				}
			}

			if(help) {
				// This is drawing the green health bar for the selected building
				glPushMatrix();

				float thick = 0.0f;
				// We need this if clause, because building coordinates are
				// based on the map grid, while unit coordinates are based on
				// x/y coordinates in OpenGL.
				if((ao->at(i)->getID() >=20 && ao->at(i)->getID() <= 39) || ao->at(i)->getID() == 3) {
					glTranslatef(X*64+5.0f,Y*64+5.0f,0.0f);
					thick = 18.0f;
				} else {
					glTranslatef(X, Y-5.0f, 0.0f);
					thick = 3.0f;
					size = 32.0f;
				}
				glColor3f(0.0f,1.0f,0.0f);
				glBegin(GL_QUADS);
				{
					glVertex2f(0.0f,0.0f);
					glVertex2f(size*quotient,0.0f);
					glVertex2f(size*quotient,thick);
					glVertex2f(0.0f,thick);
				}
				glEnd();
				glPopMatrix();
			}

			// This is drawing the black box around the health bar.
			if((ao->at(i)->getID() >=20 && ao->at(i)->getID() <= 39) || ao->at(i)->getID() == 3) {
				glPushMatrix();
				glTranslatef(X*64+5.0f,Y*64+5.0f,0.0f);
				glColor3f(0.0f,0.0f,0.0f);
				glBegin(GL_LINES);
				{
					glVertex2f(0.0f,0.0f); glVertex2f(size,0.0f);
					glVertex2f(size,0.0f); glVertex2f(size,18.0f);
					glVertex2f(size,18.0f); glVertex2f(0.0f,18.0f);
					glVertex2f(0.0f,18.0f); glVertex2f(0.0f,0.0f);
				}
				glEnd();
				glPopMatrix();

				if(((Building*)ao->at(i))->getWaypoint()[0] != -1 && ((Building*)ao->at(i))->getWaypoint()[1] != -1) {
					glPushMatrix();
					glTranslatef(0.0f,0.0f,0.0f);
					glColor3f(1.0f,1.0f,1.0f);

					glLineWidth(2.0);
					glLineStipple(9, 0xAAAA);
					glEnable(GL_LINE_STIPPLE);

					glBegin(GL_LINES);
					{
						glVertex2f(X*64+(ao->at(i)->getSize()*64)/2,Y*64+(ao->at(i)->getSize()*64)/2);
						glVertex2f(((Building*)ao->at(i))->getWaypoint()[0], ((Building*)ao->at(i))->getWaypoint()[1]);
					}
					glEnd();

					glDisable(GL_LINE_STIPPLE);

					glPopMatrix();

					glPushMatrix();
					glTranslatef(((Building*)ao->at(i))->getWaypoint()[0], ((Building*)ao->at(i))->getWaypoint()[1], 0.0f);
					glColor3f(1.0f,1.0f,1.0f);
					glLineWidth(1.0);
					glBegin(GL_LINE_LOOP);
					{
						for (int i=0; i < 360; i+=5)
						{
							float degInRad = i*(3.14159/180);
							glVertex2f(cos(degInRad)*6.0f,sin(degInRad)*6.0f);
						}
					}
					glEnd();
					glPopMatrix();
				}
			}
		}
	}

	// This for clause goes through every building in the list
	// and draws the in-building progress bar.
	for(unsigned int i = 0; i<gameState->getBuildings()->size(); i++) {
		if(gameState->getBuildings()->at(i)->buildBuilding() != -1) {
			int bX = gameState->getBuildings()->at(i)->getX(),
				bY = gameState->getBuildings()->at(i)->getY();
			float size = 64.0f*gameState->getBuildings()->at(i)->getSize()-10.0f;
			float percent = gameState->getBuildings()->at(i)->buildBuilding()/100;

			// Drawing the progress bar itself
			glPushMatrix();
			glTranslatef(bX*64+5.0f,bY*64+5.0f,0.0f);
			glColor3f(0.0f,1.0f,1.0f);
			glBegin(GL_QUADS);
			{
				glVertex2f(0.0f,0.0f);
				glVertex2f(size*percent,0.0f);
				glVertex2f(size*percent,18.0f);
				glVertex2f(0.0f,18.0f);
			}
			glEnd();
			glPopMatrix();

			// Drawing the black box around progress bar.
			glPushMatrix();
			glTranslatef(bX*64+5.0f,bY*64+5.0f,0.0f);
			glColor3f(0.0f,0.0f,0.0f);
			glBegin(GL_LINES);
			{
				glVertex2f(0.0f,0.0f); glVertex2f(size,0.0f);
				glVertex2f(size,0.0f); glVertex2f(size,18.0f);
				glVertex2f(size,18.0f); glVertex2f(0.0f,18.0f);
				glVertex2f(0.0f,18.0f); glVertex2f(0.0f,0.0f);
			}
			glEnd();
			glPopMatrix();
		}
	}

	return 0;
}

int GLRenderer::DrawSelect(int selectStartX, int selectStartY) {
	glPushMatrix();

	glTranslatef(cameraX, cameraY, 0.0f);
	glColor3f(1.0f,1.0f,1.0f);

	glBegin(GL_LINES);
	{
		glVertex2f(selectStartX,selectStartY); glVertex2f((*mouseX),selectStartY);
		glVertex2f((*mouseX),selectStartY); glVertex2f((*mouseX),(*mouseY));
		glVertex2f((*mouseX),(*mouseY)); glVertex2f(selectStartX,(*mouseY));
		glVertex2f(selectStartX,(*mouseY)); glVertex2f(selectStartX,selectStartY);
	}
	glEnd();

	glPopMatrix();
	return 0;
}

int GLRenderer::DrawTooltip() {
	if(	gameState->getActiveTooltip() != NULL) {
		gameState->getActiveTooltip()->DrawGLTooltip();
	}
	return 0;
}

int GLRenderer::DrawBuildGrid(int id) {
	// Horizontal and vertical lines on screen
	glPushMatrix();
	glTranslatef(cameraX,cameraY,0.0f);
	glColor3f(0.8f, 0.8f, 0.8f);
	glBegin(GL_LINES);
	{
		for(int i = 0; i <= 16; i++) {
			glVertex2f(64.0f+i*64-cameraX%64,0.0f);
			glVertex2f(64.0f+i*64-cameraX%64,targetHeight);
			glVertex2f(0.0f,64.0f+i*64-cameraY%64);
			glVertex2f(targetWidth,64.0f+i*64-cameraY%64);
		}
	}
	glEnd();
	glPopMatrix();

	// Yellow lines where to build
	for(int i = -1; i<gameState->getData()->getSize(id)-1; i+=1) {
		for(int j = -1; j<gameState->getData()->getSize(id)-1; j+=1) {
			// Horizontal and vertical lines on screen
			glPushMatrix();
			glTranslatef((*mouseX)+cameraX-((*mouseX)+cameraX)%64+i*64,
						 (*mouseY)+cameraY-((*mouseY)+cameraY)%64+j*64, 0.0f);

			bool help = true;
			for(int k = -1; k<gameState->getData()->getSize(id)-1; k+=1) {
				for(int l = -1; l<gameState->getData()->getSize(id)-1; l+=1) {
					if(((*mouseX)+cameraX-((*mouseX)+cameraX)%64+k*64)/64 < gameState->getMap()->get_size() &&
					   ((*mouseX)+cameraX-((*mouseX)+cameraX)%64+l*64)/64 < gameState->getMap()->get_size()) {
						if(gameState->getMap()->is_tile_used(((*mouseX)+cameraX-((*mouseX)+cameraX)%64+k*64)/64,
														 ((*mouseY)+cameraY-((*mouseY)+cameraY)%64+l*64)/64) ||
						   gameState->getMap()->tile_type(((*mouseX)+cameraX-((*mouseX)+cameraX)%64+k*64)/64,
														 ((*mouseY)+cameraY-((*mouseY)+cameraY)%64+l*64)/64) != 1) {
							help = false;
						}
					}
				}
			}

			if(help)
				glColor3f(0.0f, 0.9f, 0.0f);
			else
				glColor3f(0.7f, 0.7f, 0.0f);

			glBegin(GL_LINES);
			{
				glVertex2f(0.0f,0.0f);
				glVertex2f(64.0f, 64.0f);
				glVertex2f(0.0f,64.0f);
				glVertex2f(64.0f, 0.0f);
			}
			glEnd();

			glPopMatrix();
		}
	}

	// Used tiles
	for(int i = cameraX/64; i < cameraX/64+12; i++) {
		for(int j = cameraY/64; j < cameraY/64+16; j++) {
			if(j < gameState->getMap()->get_size() && i < gameState->getMap()->get_size()) {
				if(gameState->getMap()->is_tile_used(i,j) || gameState->getMap()->tile_type(i,j) != 1) {
					// Horizontal and vertical lines on screen
					glPushMatrix();

					glTranslatef(i*64,j*64,0.0f);
					glColor3f(1.0f, 0.0f, 0.0f);

					glBegin(GL_LINES);
					{
						glVertex2f(0.0f,0.0f);
						glVertex2f(64.0f, 64.0f);
						glVertex2f(0.0f,64.0f);
						glVertex2f(64.0f, 0.0f);
					}
					glEnd();

					glPopMatrix();
				}
			}
		}
	}

	DrawTextures(cameraX+(*mouseX)-96, cameraY+(*mouseY)-96, id);
	float* color = gameState->getPlayers()->at(0)->getColor();
	DrawTextures(cameraX+(*mouseX)-96,
				 cameraY+(*mouseY)-96,
				 id+40,
				 color[0], color[1], color[2]);
	return 0;
}

int GLRenderer::DrawEnd(int id) {
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);

	glBegin(GL_QUADS);
	{
		glVertex2f(0.0f,0.0f);
		glVertex2f(targetWidth, 0.0f);
		glVertex2f(targetWidth,targetHeight);
		glVertex2f(0.0f, targetHeight);
	}
	glEnd();
	glPopMatrix();

	DrawTextures(280, 150, 118);
	if(id == 0) DrawString(300, 190, "You won!", 5);
	if(id == 1) DrawString(330, 190, "Defeated!", 5);

	DrawTextures(325, 420, 122);
	DrawString(510, 440, "Quit", 3);

	return 0;
}
int GLRenderer::DrawStartMenu(int id) {
	// Scroll the background of start screen
	startScreenScroll+=1;
	glTranslatef(0,-1,0);

	// Background
	for(int i = 0; i<16; i+=1) {
		for(int j = startScreenScroll/64; j<13+(startScreenScroll/64); j+=1) {
			DrawTextures(i*64,j*64,startScreenMap[i][j%20]);
		}
	}

	// Version number at bottom left
	std::stringstream str;

	// CMake created a visual studio project for windows
	// which didn't want to see the GIT_SHA1 file.
#ifndef _WIN32
	str << "Version 0.0-alpha1-" << g_GIT_SHA1;
#else
	str << "Version 0.0-alpha1";
#endif

	DrawString(0,targetHeight-15+startScreenScroll,str.str(), 1);

	// Buttons
	if(id == 0) { // Start
		// Game name
		DrawString(170, startScreenScroll+55, "Yet another Rts", 5);

		// Singleplayer
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-130,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+130, startScreenScroll+targetHeight-targetHeight/2-130+16, "Singleplayer", 3);
		// Multiplayer
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-50,122, 0.5f, 0.5f, 0.5f);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+140, startScreenScroll+targetHeight-targetHeight/2-50+16, "Multiplayer", 3, 100, 100, 100);
		// Options
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+30,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+155, startScreenScroll+targetHeight-targetHeight/2+30+16, "Options", 3);
		// Quit
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+190,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+180, startScreenScroll+targetHeight-targetHeight/2+190+16, "Quit", 3);
	} else if(id == 1) { // Start->Singleplayer
		// Category
		DrawString(230, startScreenScroll+55, "Single Player", 5);

		// Campaign
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-130,122, 0.5f, 0.5f, 0.5f);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+145, startScreenScroll+targetHeight-targetHeight/2-130+16, "Campaign", 3, 100, 100, 100);
		// Tutorial
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-50,122, 0.5f, 0.5f, 0.5f);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+155, startScreenScroll+targetHeight-targetHeight/2-50+16, "Tutorial", 3, 100, 100, 100);
		// Custom
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+30,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+155, startScreenScroll+targetHeight-targetHeight/2+30+16, "Custom", 3);
		// Back
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+190,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+180, startScreenScroll+targetHeight-targetHeight/2+190+16, "Back", 3);
	} else if(id == 2) { // Start->Multiplayer

	} else if(id == 3) { // Start->Options
		// Category
		DrawString(210, startScreenScroll+55, "Game Settings", 5);

		// Resolution
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-130,122, 0.5f, 0.5f, 0.5f);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+35, startScreenScroll+targetHeight-targetHeight/2-130+16, "Resolution:", 3, 100, 100, 100);
		std::stringstream ss;
		ss << targetWidth << "x" << targetHeight;
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+235, startScreenScroll+targetHeight-targetHeight/2-130+16, ss.str(), 3, 100, 100, 100);
		// Fullscreen
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2-50,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+35, startScreenScroll+targetHeight-targetHeight/2-50+16, "Fullscreen", 3, 100, 100, 100);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+235, startScreenScroll+targetHeight-targetHeight/2-50+16, "Windowed", 3);

		// Audio Music
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+30,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+35, startScreenScroll+targetHeight-targetHeight/2+30+16, "Audio Music:", 3);
		float percent = 100.0f;
		glPushMatrix();
		glTranslatef(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+220, startScreenScroll+targetHeight-targetHeight/2+30+16,0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D,textures[120]->getTexture());
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,0);
			glVertex2f(0.0f,0.0f);

			glTexCoord2f(1,0);
			glVertex2f(1.5f*percent, 0.0f);

			glTexCoord2f(1,1);
			glVertex2f(1.5f*percent,20.0f);

			glTexCoord2f(0,1);
			glVertex2f(0.0f,20.0f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

		// Audio Sound
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+110,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+35, startScreenScroll+targetHeight-targetHeight/2+110+16, "Audio Sound:", 3);
		percent = 100.0f;
		glPushMatrix();
		glTranslatef(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+220, startScreenScroll+targetHeight-targetHeight/2+110+16,0);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D,textures[120]->getTexture());
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,0);
			glVertex2f(0.0f,0.0f);

			glTexCoord2f(1,0);
			glVertex2f(1.5f*percent, 0.0f);

			glTexCoord2f(1,1);
			glVertex2f(1.5f*percent,20.0f);

			glTexCoord2f(0,1);
			glVertex2f(0.0f,20.0f);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();

		// Back
		DrawTextures(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2, startScreenScroll+targetHeight-targetHeight/2+190,122);
		DrawString(targetWidth-targetWidth/2-textures[122]->getTextureWidth()/2+180, startScreenScroll+targetHeight-targetHeight/2+190+16, "Back", 3);
	}

	return 0;
}

void GLRenderer::setResolution(int w, int h) {
	targetWidth = w;
	targetHeight = h;
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0, w,h , 0, 1, -1);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}
