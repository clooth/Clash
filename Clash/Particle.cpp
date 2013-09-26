//
//  Particle.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Particle.h"
#include "GameState.h"

Particle::Particle( int x, int y, int xDest, int yDest, GameState* gs) {
    this->gameState = gs;

    //Set offsets
    this->x = x - 5 + ( rand() % 25 );
    this->y = y - 5 + ( rand() % 25 );
    this->xDest = xDest - 5 + ( rand() % 25 );
    this->yDest = yDest - 5 + ( rand() % 25 );

    //part = NULL;
}

void Particle::show() {
    //Getting animation done for particles
    if(x < xDest) x+=5;
    if(x > xDest) x-=5;
    if(y < yDest) y+=5;
    if(y > yDest) y-=5;

    glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_QUADS);
	{
		glVertex2f(0.0f,0.0f);
		glVertex2f(3.0f,0.0f);
		glVertex2f(3.0f,3.0f);
		glVertex2f(0.0f,3.0f);
	}
	glEnd();
	glPopMatrix();
}

bool Particle::isDead() {
    // It's getting deleted by gameloop when it's dead.
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			if(x+i == xDest && y+j == yDest) return 1;
		}
	}
    return 0;
}

Particle::~Particle() {
}
