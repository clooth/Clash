/*
 * File:   FogOfWar.cpp
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
 * FogOfWar class
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <cmath>
#include "FogOfWar.h"
#include "GameState.h"
#include "Map.h"
#include "Building.h"
#include "Unit.h"
#include "GLTesselation.h"

FogOfWar::FogOfWar() {
	this->subject.resize(4);
	this->clip.resize(31);

    this->fieldOfView.resize(110);
    for(unsigned int i=0; i<this->fieldOfView.size(); i++) {
        this->fieldOfView[i].resize(110);
        for(unsigned int j=0; j<this->fieldOfView[i].size(); j++) {
            this->fieldOfView[i][j] = false;
        }
    }
}

void FogOfWar::setGameState(GameState* gameState) {
    this->gameState = gameState;
}

FogOfWar::~FogOfWar() {
}

clipper::Polygons FogOfWar::getSolution() {
	return solution;
}

void FogOfWar::UpdateFOV() {
	DrawPolygon(solution);

    // This constants are need to calculate the view circle around
    // the units.
    float DEG2RAD = 3.14159/180;
    int radius = 280;
    float PI = 3.1415926535897932;

    c.Clear();
	subject[0] = clipper::IntPoint(0,0);
	subject[1] = clipper::IntPoint(110*64,0);
	subject[2] = clipper::IntPoint(110*64,110*64);
	subject[3] = clipper::IntPoint(0,110*64);
	c.AddPolygon(subject, clipper::ptSubject);

    // This sets the view to total dark at the start of the update.
    // As we are calculating the new field of view, the old one needs
    // to be reset.
    for(unsigned int i=0; i<this->fieldOfView.size(); i++) {
        for(unsigned int j=0; j<this->fieldOfView[i].size(); j++) {
            this->fieldOfView[i][j] = false;
        }
    }

    // This loop is going throw all own buildings and sets their view radius
    for(unsigned int i=0; i<this->gameState->getBuildings()->size(); i++) {
        if(this->gameState->getBuildings()->at(i)->getOwner() == this->gameState->getPlayers()->at(0)) {
            // Position of buildings are already saved in a tile-grid form, so we doesnt
            // need a recalculating. Though, we are adding size/2 because we want the
            // middle point of the building.
            float positionX=this->gameState->getBuildings()->at(i)->getX()+this->gameState->getBuildings()->at(i)->getSize()/2;
            float positionY=this->gameState->getBuildings()->at(i)->getY()+this->gameState->getBuildings()->at(i)->getSize()/2;

            for (float i = 0.0, j = 0; i < 2.0*PI; i += 2.0*PI/30.0, j++){
				float x = radius * cos(i) + positionX*64;
				float y = radius * sin(i) + positionY*64;
				clip[j] = clipper::IntPoint((int)x,(int)y);
			}
            c.AddPolygon(clip, clipper::ptClip);
            c.Execute(clipper::ctDifference, solution);
            c.Clear();
            c.AddPolygons(solution, clipper::ptSubject);

            // Like a circle with a point each five degrees
            for(int k = 0; k < 360; k+=10) {
                float degInRad = k*DEG2RAD;

                // This is the formula for calculating the x/y coordinates at this
                // point of the circle. The mid point is set to the building middlepoint.
                float x = cos(degInRad)*(radius+50) + positionX*64;
                float y = sin(degInRad)*(radius+50) + positionY*64;

                // Next for loops are setting the tiles inside the circle as seen
                // field of view.
                for(int i = x; i>positionX*64; i--) {
                    if(i/64 >= 0 && y/64 >= 0 && i/64 < 110 && y/64 < 110) {
                        fieldOfView[i/64][y/64] = true;
                    }
                }
                
                for(int i = x; i<positionX*64; i++) {
                    if(i/64 >= 0 && y/64 >= 0 && i/64 < 110 && y/64 < 110) {
                        fieldOfView[i/64][y/64] = true;
                    }
                }
            }
        }
    }

    // Same thing for units now.
    for(unsigned int i=0; i<this->gameState->getUnits()->size(); i++) {
        if(this->gameState->getUnits()->at(i)->getOwner() == this->gameState->getPlayers()->at(0)) {
            // The units are calculated on a pixel base, so we need to recalculate
            // position to tile coordinates.
            int positionX=(this->gameState->getUnits()->at(i)->getX()-(this->gameState->getUnits()->at(i)->getX()*64)%64);
            int positionY=(this->gameState->getUnits()->at(i)->getY()-(this->gameState->getUnits()->at(i)->getY()*64)%64);


            for (float i = 0.0, j = 0; i < 2.0*PI; i += 2.0*PI/30.0, j++){
				float x = radius * cos(i) + positionX;
				float y = radius * sin(i) + positionY;
				clip[j] = clipper::IntPoint((int)x,(int)y);
			}
			c.AddPolygon(clip, clipper::ptClip);
			c.Execute(clipper::ctDifference, solution);
			c.Clear();
			c.AddPolygons(solution, clipper::ptSubject);

            for(int k = 0; k < 360; k+=10) {
                float degInRad = k*DEG2RAD;

                // This is the formula for calculating the x/y coordinates at this
                // point of the circle. The mid point is set to the coordinate where
                // unit stands on, which is the upper left corner, but it's accurate
                // enough for now.
                float x = cos(degInRad)*radius + positionX;
                float y = sin(degInRad)*radius + positionY;

                // Next two loops setting field of view in the circle
                for(int i = x; i>positionX; i--) {
                    if(i/64 >= 0 && y/64 >= 0 && i/64 < 110 && y/64 < 110) {
                        fieldOfView[i/64][y/64] = true;
                    }
                }
                
                for(int i = x; i<positionX; i++) {
                    if(i/64 >= 0 && y/64 >= 0 && i/64 < 110 && y/64 < 110) {
                        fieldOfView[i/64][y/64] = true;
                    }
                }
            }
        }
    }
    c.Execute(clipper::ctDifference, solution);
}

bool FogOfWar::isFog(int x, int y) {
    // When field of view is false, units can't see, so there is fog
    // field of view false -> fog true
    if(fieldOfView[x][y] == false) {
        return true;
    }
    return false;
}

bool FogOfWar::isUnexplored(int x, int y) {
    // When field of view is false, units can't see, so there is fog
    // field of view false -> fog true
    if(explored[x][y] == false) {
        return true;
    }
    return false;
}

