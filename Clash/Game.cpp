/*
 * File:   Game.cpp
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
 * Game methods, main game object, managing most of the
 * inter-class communication as well as Drawings.
 */

#include <sstream>
#include <cmath>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Config.h"
#include "Game.h"

#include "GLRenderer.h"
//#include "Audio.h"
#include "Map.h"
#include "Unit.h"
#include "Timer.h"
#include "Building.h"
#include "GameObject.h"
#include "GameState.h"
#include "Ressources.h"
#include "Tooltip.h"
#include "FogOfWar.h"
#include "GitSHA1.h"
#include "Scripting.h"

//The attributes of the screen 
const int MAPSIZE = 110;

Game::Game() {
    this->mousePressed = false;
    this->mouseSelect = false;
    this->quit = false;
    this->buildGrid = false;
    this->buildMenu = false;
    this->console = false;

    this->selectStartX = 0;
    this->selectStartY = 0;
    this->FrameTime = 0;
    this->mouseX = 0;
    this->mouseY = 0;
    this->announce = 0;
    this->buildMenuID = 0;
    this->mouseMotionCounter = 0;
    this->startMenuID = 0;

    this->currPosX = -1;
    this->currPosY = -1;

    this->keystates = NULL;
    this->gameState = NULL;
    this->screen = NULL;
    this->render = NULL;
    this->activeTooltip = NULL;

    this->buildMenuTT[0] = NULL;
    this->buildMenuTT[1] = NULL;
    this->buildMenuTT[2] = NULL;
    this->buildMenuTT[3] = NULL;
    this->buildMenuTT[4] = NULL;
    this->buildMenuTT[5] = NULL;

    this->ingame = false;

    this->fogOfWar = NULL;    
}

int Game::init() {
	// Init fog of war
	this->fogOfWar = new FogOfWar();

    // Gamestate Object
    gameState = new GameState(&m,&buildings,&units,&announce,&timer, &announceText,&players,activeTooltip, fogOfWar);
    
    fogOfWar->setGameState(gameState);

    // Init Map
    //m.createMap(MAPSIZE); <-- deprecated
    m.setGameState(gameState);

    // Audio, uncomment to play background sound
    //audio = new Audio();

    // keystates object serves "key x is currently pressed" events
    keystates = SDL_GetKeyState( NULL );
    
    // FPS Stuff
    int frame = 0;  
    Timer fps, update; 

    // Setting first activated object to a null pointer because nothing
    // is activated. Makes sense doesn't it? Beware of setting size() to
    // zero, because my code is crap. We do it later.
    activatedObject.resize(1);
    activatedObject[0] = NULL;

    // Initializing SDL with everything, video, audio, and I guess there
    // is other stuff, too.
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
    	std::cerr << "SDL Error" << std::endl;
        return 1;
    }


    // Intializing video device for SDL
    if(CFG_VIDEO_FULLSCREEN)
        screen = SDL_SetVideoMode(CFG_VIDEO_WIDTH,
        						  CFG_VIDEO_HEIGHT,
        						  CFG_VIDEO_BPP,
        						  SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_FULLSCREEN | SDL_ANYFORMAT );
    else
        screen = SDL_SetVideoMode(CFG_VIDEO_WIDTH,
        						  CFG_VIDEO_HEIGHT,
        						  CFG_VIDEO_BPP,
        						  SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_ANYFORMAT);

    if (TTF_Init() < 0) {
		std::cerr << "TTF Error" << std::endl;
		return 1;
    }

    // Initializing OpenGL
    this->render = new GLRenderer(CFG_VIDEO_WIDTH, CFG_VIDEO_HEIGHT, gameState);

    gameState->setScreen(screen);
    gameState->setRenderer(render);
    render->setMousePointer(&mouseX, &mouseY);

    // This handles how sensitive the application is to key repeats, we
    // set it to a third of the default repeat interval, so the keyboard
    // scrolling is not speedy gonzales.
    SDL_EnableKeyRepeat(300, SDL_DEFAULT_REPEAT_INTERVAL/3);

    // This is dirty hacky, need to outsource buildmenu very fast.
    // But at the moment, we create tooltips objects for our build
    // menu at this point.
    buildMenuTT[0] = new Tooltip(21,gameState);
    buildMenuTT[1] = new Tooltip(22,gameState);
    buildMenuTT[2] = new Tooltip(23,gameState);
    buildMenuTT[3] = new Tooltip(24,gameState);
	buildMenuTT[4] = new Tooltip(25,gameState);
	buildMenuTT[5] = new Tooltip(26,gameState);

    // Starting important Timers
    update.start(); 
    timer.start();
    
    // Start Test Building
    // I really need to redo import map method to get the ability to directly
    // code buildings into the map. But for the moment, startbuildings are
    // luaed.
	s.gameState = gameState;

	//s.doFile("scripts/custom.lua");
	//ingame = true;

	float fpsUpdate = SDL_GetTicks();

    // Main Game loop
    // While the user hasn't quit
    while( quit == false ) {
        // Pumping events as well as calculating stuff every
        // frame.
        SDL_PumpEvents();
        EventInput();

        if( update.get_ticks() > 0.005f && !ingame) {
        	frame++;
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        	render->DrawStartMenu(startMenuID);

            SDL_GL_SwapBuffers();
        	// Restart the update timer
			update.start();
        }

        if( update.get_ticks() > 0.005f && ingame ) {
    		frame++;
    		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    		this->FrameInput();
    		render->DrawTiles();
        	this->Calculate();
        	this->Update();
        	//render->DrawEnd(0);

        	SDL_GL_SwapBuffers();
            // Restart the update timer
            update.start(); 
        }

        if((SDL_GetTicks() - fpsUpdate) > 1000) {
        	std::stringstream caption;
        	caption << "yarts // fps: " << (float)(frame / (SDL_GetTicks()-fpsUpdate) * 1000);
        	SDL_WM_SetCaption( caption.str().c_str(), NULL );
        	fpsUpdate = SDL_GetTicks();
        	frame = 0;
        }

        // Update the screen with new drawn stuff
        if( SDL_Flip( screen ) == -1 ) {
            return 1;
        } 
    } 

    return 0;
} 

void Game::Calculate() {
	//fogOfWar->UpdateFOV();

    for(unsigned int k = 0; k < units.size(); k++) {
        if(units[k]->getCurrentLife() <= 0) {
            for(unsigned int l = 0; l < activatedObject.size(); l++) {
                if(activatedObject[l] == units[k]) {
                    activatedObject.erase(activatedObject.begin()+l);
                }
            }
            for(unsigned int u = 0; u < units.size(); u++) {
                if(units[u]->getAttackedObject() == units[k])
                    units[u]->stopAttack();
            }
            delete units[k];
            units.erase(units.begin()+k);
        }
        units[k]->calculate();
    }

    for(unsigned int k = 0; k < buildings.size(); k++) {
        if(buildings[k]->buildBuilding() == -2) {
            if(this->activatedObject[0] == buildings[k]) {
                this->activatedObject[0] = NULL;
            }
            if(buildings[k]->getID() != 3) {
                for(unsigned int u = 0; u < units.size(); u++) {
                    if(units[u]->getAttackedObject() == buildings[k])
                        units[u]->stopAttack();
                }
                delete buildings[k];
                buildings.erase(buildings.begin()+k);
            }
        }
        if(buildings[k]->isBuildingUnit()) {
            buildings[k]->buildUnit();
        }
    }
}

void Game::FrameInput() {
	//Scrolling at arrow keys
	int scrollspeed = CFG_INPUT_SCROLL_SPEED;
	if (render->cameraX > 0 && keystates[SDLK_LEFT]) {
		render->cameraX-= scrollspeed;
		render->moveCamera(-1*scrollspeed,0);
	}

	if (render->cameraX < 110*64-CFG_VIDEO_WIDTH && keystates[SDLK_RIGHT]) {
		render->cameraX+=scrollspeed;
		render->moveCamera(scrollspeed,0);
	}

	if (render->cameraY > 0 && keystates[SDLK_UP]) {
		render->cameraY-=scrollspeed;
		render->moveCamera(0,-1*scrollspeed);
	}

	if (render->cameraY < 110*64-CFG_VIDEO_HEIGHT+200 && keystates[SDLK_DOWN]) {
		render->cameraY+=scrollspeed;
		render->moveCamera(0,scrollspeed);
	}

	// Mouseclick is hold
	// out of mousemotion loop so it does run when you just click and don't move your mouse
	if(mousePressed) {
		mouseX = event.button.x;
		mouseY = event.button.y;

		//If mouse on Minimap
		if( (mouseX >= 20)  && (mouseX <= 280) && (mouseY >= CFG_VIDEO_HEIGHT-280) && (mouseY <= CFG_VIDEO_HEIGHT-20)) {
			mouseX -= 36; mouseY -= CFG_VIDEO_HEIGHT-260+12;
			mouseX *= 32; mouseY *= 32;

			//correction so that camera does not get out of screen
			if(mouseX > (MAPSIZE*64)-5-CFG_VIDEO_WIDTH)
				render->setCameraX((MAPSIZE*64)-5-CFG_VIDEO_WIDTH);
			else if(mouseX < 0)
				render->setCameraX(0);
			else
				render->setCameraX(mouseX);

			if(mouseY > (MAPSIZE*64)-5-CFG_VIDEO_HEIGHT+200)
				render->setCameraY((MAPSIZE*64)-5-CFG_VIDEO_HEIGHT+200);
			else if(mouseY < 0)
				render->setCameraY(0);
			else
				render->setCameraY(mouseY);
		}
	}
}

void Game::BuildMenuInput(int type) {
	int position = 0;
	// CC
	if((mouseX >= CFG_VIDEO_WIDTH-230)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-140) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-555) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-470)) {
		position = 1;
	}
	// Barracks
	if((mouseX >= CFG_VIDEO_WIDTH-120)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-30) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-555) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-470)) {
		position = 2;
	}
	// Supply depot
	if((mouseX >= CFG_VIDEO_WIDTH-230)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-140) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-460) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-375)) {
		position = 3;
	}
	// Research lab
	if((mouseX >= CFG_VIDEO_WIDTH-120)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-30) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-460) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-375)) {
		position = 4;
	}
	// Factory
	if((mouseX >= CFG_VIDEO_WIDTH-230)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-140) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-365) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-280)) {
		position = 5;
	}
	// Military Airport
	if((mouseX >= CFG_VIDEO_WIDTH-120)  &&
	   (mouseX <= CFG_VIDEO_WIDTH-30) &&
	   (mouseY >= CFG_VIDEO_HEIGHT-365) &&
	   (mouseY <= CFG_VIDEO_HEIGHT-280)) {
		position = 6;
	}
	// MOTIONS
	if(type == 1) {
		if(position == 1) {
			this->gameState->setActiveTooltip(buildMenuTT[0]);
		}
		if(position == 2) {
			this->gameState->setActiveTooltip(buildMenuTT[1]);
		}
		if(position == 3) {
			this->gameState->setActiveTooltip(buildMenuTT[2]);
		}
		if(position == 4) {
			this->gameState->setActiveTooltip(buildMenuTT[3]);
		}
		if(position == 5) {
			this->gameState->setActiveTooltip(buildMenuTT[4]);
		}
		if(position == 6) {
			this->gameState->setActiveTooltip(buildMenuTT[5]);
		}
	// CLICKS
	} else if(type == 2) {
		if(position == 1) {
			buildMenuID = 21;
			buildGrid = true;
			buildMenu = false;
		}
		if(position == 2) {
			buildMenuID = 22;
			buildGrid = true;
			buildMenu = false;
		}
		if(position == 3) {
			buildMenuID = 23;
			buildGrid = true;
			buildMenu = false;
		}
		if(position == 4) {
			buildMenuID = 24;
			buildGrid = true;
			buildMenu = false;
		}
		if(position == 5) {
			buildMenuID = 25;
			buildGrid = true;
			buildMenu = false;
		}
		if(position == 6) {
			buildMenuID = 26;
			buildGrid = true;
			buildMenu = false;
		}
	} else {
		assert(true);
	}
}

void Game::EventInput() {
    while( SDL_PollEvent( &event ) ) {
    	if( event.type == SDL_VIDEORESIZE) {
    		int w,h;
    		w = event.resize.w;
    		h = event.resize.h;
    		std::cout << w << "," << h << std::endl;

    		render->setResolution(w,h);

    		break;
    	}
        //If the user has Xed out the window 
        if( event.type == SDL_QUIT ) { 
            //Quit the program 
            quit = true; 
        } 

        //If a key was pressed
        if( event.type == SDL_KEYDOWN ) {
            // Quit at ESC
            if(event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }

            if(event.key.keysym.sym == SDLK_CARET) {
            	if(console) console = false;
            	else console = true;
		    }

            if(event.key.keysym.sym == SDLK_b) {
            	if(buildMenu) {
            		buildMenu = false;
					Update();
            	} else {
            		buildMenu = true;
					Update();
            	}
            }
        }

        //If in startmenu
		if(!ingame) {
			//If the left mouse button was pressed
			if( event.type == SDL_MOUSEBUTTONDOWN ) {
				if( event.button.button == SDL_BUTTON_LEFT ) {
					int x = event.button.x;
					int y = event.button.y;

					// Button 1
					if(x >= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2-202 && x <= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2+202 &&
					   y >= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2-130 && y <= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2-74) {
						if(startMenuID == 0) startMenuID = 1;
					}
					// Button 2
					if(x >= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2-202 && x <= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2+202 &&
					   y >= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2-50 && y <= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+6) {
						if(startMenuID == 1) {
							/*if(render->startScreenScroll > 0) {
								glTranslatef(0.0f, render->startScreenScroll, 0.0f);
								render->startScreenScroll = 0;
							}


							s.doFile("scripts/tutorial.lua");
							ingame = true;*/
						}
					}
					// Button 3
					if(x >= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2-202 && x <= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2+202 &&
					   y >= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+30 && y <= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+86) {
						if(startMenuID == 1) {
							if(render->startScreenScroll > 0) {
								glTranslatef(0.0f, render->startScreenScroll, 0.0f);
								render->startScreenScroll = 0;
							}
							s.doFile("scripts/custom.lua");
							ingame = true;
						}
						if(startMenuID == 0) startMenuID = 3;
					}
					// Button 4
					if(x >= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2-202 && x <= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2+202 &&
					   y >= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+110 && y <= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+166) {

					}
					// Button 5
					if(x >= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2-202 && x <= CFG_VIDEO_WIDTH-CFG_VIDEO_WIDTH/2+202 &&
					   y >= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+190 && y <= CFG_VIDEO_HEIGHT-CFG_VIDEO_HEIGHT/2+246) {
						if(startMenuID == 0) quit = true;
						if(startMenuID > 0) startMenuID = 0;
					}
				}
			}
			// disable most things cause we are in start menu
			return;
		}

        //If the mouse moved 
        if( event.type == SDL_MOUSEMOTION ) { 
            mouseX = event.button.x;
            mouseY = event.button.y;
            // Unset tooltip each motion
            gameState->setActiveTooltip(NULL);

            // If mouse is over the actions area of units/buildings
            if( (mouseX >= CFG_VIDEO_WIDTH-240)  &&
				(mouseX <= CFG_VIDEO_WIDTH) &&
				(mouseY >= CFG_VIDEO_HEIGHT-240) &&
				(mouseY <= CFG_VIDEO_HEIGHT)) {
                // Checks if something is activated and if it's a building
                if(activatedObject.size() == 1 && activatedObject[0] != NULL && activatedObject[0]->getID() >= 20 && activatedObject[0]->getID() <= 39) {
                    // Only if it's own building
                    if(activatedObject[0]->getOwner() == players[0]) {
                        if(((Building*)activatedObject[0])->buildBuilding() == -1) {
                            ((Building*)activatedObject[0])->doMotion(mouseX-CFG_VIDEO_WIDTH+240,mouseY-CFG_VIDEO_HEIGHT+240);
                        }
                    }
                }
            }

            // Tooltips for buildmenu
			if( buildMenu ) {
				BuildMenuInput(1);
			}
        }

        if( event.type == SDL_MOUSEBUTTONDOWN ) { 
            //If the left mouse button was pressed 
            if( event.button.button == SDL_BUTTON_LEFT ) {           
                //selecting stuff by clicking
                int x = mouseX + render->getCameraX();
                int y = mouseY + render->getCameraY();
                
                // Mouse click on top of minimap
                if( (mouseX >= 20)  &&
                    (mouseX <= 280) &&
                    (mouseY >= CFG_VIDEO_HEIGHT-280) &&
                    (mouseY <= CFG_VIDEO_HEIGHT-20)) {
                    mousePressed = true;
                }
                // click on build menu button / closed menu
                else if(!buildMenu &&
                        (mouseX >= CFG_VIDEO_WIDTH-32)  &&
                        (mouseX <= CFG_VIDEO_WIDTH) &&
                        (mouseY >= CFG_VIDEO_HEIGHT-350) &&
                        (mouseY <= CFG_VIDEO_HEIGHT-250)) {
                    buildMenu = true;
                    Update();
                }
                // build menu open actions
                else if(buildMenu) {
                	BuildMenuInput(2);
                	if( (mouseX >= CFG_VIDEO_WIDTH-282)  &&
						(mouseX <= CFG_VIDEO_WIDTH-250) &&
						(mouseY >= CFG_VIDEO_HEIGHT-350) &&
						(mouseY <= CFG_VIDEO_HEIGHT-250)) {
                		buildMenu = false;
                	}
                    Update();
                }
                // If buildgrid is activated, mouse click out of minimap indicates placing of building
                else if(buildGrid) {
                    if(placeBuilding() == 1) {
                        announceText = "You can't build here";
                        announce = timer.get_ticks();
                    }
                } 
                
                // Selecting building or unit if there is no build menu open
                else if(m.tileUsed(x/64,y/64)) {
                    // Basically goes through the array of each building on map and looks if mousepointer is
                    // on top of one of the tiles.
                    for(unsigned int k = 0; k < buildings.size(); k++) {
                        if( buildings[k]->getX()*64 <= x &&
                            buildings[k]->getX()*64 + buildings[k]->getSize()*64 >= x  &&
                            buildings[k]->getY()*64 <= y &&
                            buildings[k]->getY()*64 + buildings[k]->getSize()*64 >= y ) {

                            //something gets activated, redraw tiles
                        	activatedObject.clear();
                            activatedObject.resize(1);
                            activatedObject[0] = buildings[k];
                            Update();
                        }
                    }
                } 
                // If nothing happened yet, you clicked on free ground to unselect something or selecting unit
                else if( (mouseX <= CFG_VIDEO_WIDTH) &&
                         (mouseY <= CFG_VIDEO_HEIGHT-240) ) {
                    mouseSelect = true;
                    selectStartX = event.button.x;
                    selectStartY = event.button.y;
                    if(!keystates[ SDLK_LSHIFT ]) {
                        activatedObject.resize(0);
                    }
                    // Goes through the array of each unit
                    // @TODO: Bug: you can select same unit more than once
                    for(unsigned int k = 0; k < units.size(); k++) {
                        if( units[k]->getX() <= x &&
                            units[k]->getX() + units[k]->getSize()*64 >= x  &&
                            units[k]->getY() <= y &&
                            units[k]->getY() + units[k]->getSize()*64 >= y ) {
                            if((activatedObject.empty() || keystates[ SDLK_LSHIFT ]) && activatedObject.size() < 14) {
                                activatedObject.resize(activatedObject.size()+1);
                                activatedObject[activatedObject.size()-1] = units[k];
                            }
                        }
                    }

                    Update();
                }
                // Building/Unit actions
                else if( (mouseX >= CFG_VIDEO_WIDTH-240)  &&
                         (mouseX <= CFG_VIDEO_WIDTH) &&
                         (mouseY >= CFG_VIDEO_HEIGHT-240) &&
                         (mouseY <= CFG_VIDEO_HEIGHT)) {
                    // Checks if it's building
                    if(activatedObject.size() == 1 && activatedObject[0] != NULL && activatedObject[0]->getID() >= 20 && activatedObject[0]->getID() <= 39) {
                        // Can't build stuff if building is still building itself up
                        if(activatedObject[0]->getOwner() == players[0] && ((Building*)activatedObject[0])->buildBuilding() == -1) {
                            ((Building*)activatedObject[0])->doAction(mouseX-CFG_VIDEO_WIDTH+240,mouseY-CFG_VIDEO_HEIGHT+240);
                        }
                    }
                }
            }

            if( event.button.button == SDL_BUTTON_RIGHT ) {
                if(buildGrid) {
                    buildGrid = false;
                    Update();
                } else {
                    for(unsigned int i = 0; i < activatedObject.size(); i++) {
                        if(activatedObject[i] != NULL && activatedObject[i]->getID() >= 40 && activatedObject[i]->getOwner() == players[0]) {
                            bool helper = false;
                            // Sending unit to attack or work
                            for(unsigned int j = 0; j<buildings.size() ; j++) {
                                for(int k = 0; k<buildings[j]->getSize(); k++) {
                                    for(int l = 0; l<buildings[j]->getSize(); l++) {
                                        if( buildings[j]->getX() == (render->getCameraX()+event.button.x-(render->getCameraX()+event.button.x)%64)/64-k &&
                                            buildings[j]->getY() == (render->getCameraY()+event.button.y-(render->getCameraY()+event.button.y)%64)/64-l) {
                                            ((Unit*)activatedObject[i])->sendTo(buildings[j]);
                                            helper = true;
                                            break;
                                        }
                                    }
                                }
                            }

                            for(unsigned int j = 0; j<units.size(); j++) {
                                if( units[j]->getX() <= event.button.x + render->getCameraX() &&
                                    units[j]->getX() + units[j]->getSize()*64 >= event.button.x + render->getCameraX()  &&
                                    units[j]->getY() <= event.button.y + render->getCameraY() &&
                                    units[j]->getY() + units[j]->getSize()*64 >= event.button.y + render->getCameraY() ) {
                                    ((Unit*)activatedObject[i])->sendTo(units[j]);
                                    helper = true;
                                    break;
                                }
                            }
                            if(helper) continue;

                             // Need to do every check four times, to check borders of unit.
                             // Not on Buildings
                            if(!(m.tileUsed((render->getCameraX()+event.button.x-(render->getCameraX()+event.button.x)%64)/64,
                                    (render->getCameraY()+event.button.y-(render->getCameraY()+event.button.y)%64)/64) ||
                                 m.tileUsed((render->getCameraX()+event.button.x+32-(render->getCameraX()+event.button.x+32)%64)/64,
                                    (render->getCameraY()+event.button.y-(render->getCameraY()+event.button.y)%64)/64) ||
                                 m.tileUsed((render->getCameraX()+event.button.x-(render->getCameraX()+event.button.x)%64)/64,
                                    (render->getCameraY()+event.button.y+32-(render->getCameraY()+event.button.y+32)%64)/64) ||
                                 m.tileUsed((render->getCameraX()+event.button.x+32-(render->getCameraX()+event.button.x+32)%64)/64,
                                    (render->getCameraY()+event.button.y+32-(render->getCameraY()+event.button.y+32)%64)/64) ||
                                 // Not on Grass
                                 m.tileType((render->getCameraX()+event.button.x-(render->getCameraX()+event.button.x)%64)/64,
                                    (render->getCameraY()+event.button.y-(render->getCameraY()+event.button.y)%64)/64) != 1  ||
                                 m.tileType((render->getCameraX()+event.button.x+32-(render->getCameraX()+event.button.x+32)%64)/64,
                                    (render->getCameraY()+event.button.y-(render->getCameraY()+event.button.y)%64)/64) != 1  ||
                                 m.tileType((render->getCameraX()+event.button.x-(render->getCameraX()+event.button.x)%64)/64,
                                    (render->getCameraY()+event.button.y+32-(render->getCameraY()+event.button.y+32)%64)/64) != 1  ||
                                 m.tileType((render->getCameraX()+event.button.x+32-(render->getCameraX()+event.button.x+32)%64)/64,
                                    (render->getCameraY()+event.button.y+32-(render->getCameraY()+event.button.y+32)%64)/64) != 1)) {
                            	((Unit*)activatedObject[i])->newMove();
                                ((Unit*)activatedObject[i])->moveTo(render->getCameraX()+event.button.x,render->getCameraY()+event.button.y);
                            }
                            
                        } else if(activatedObject[i] != NULL && activatedObject[i]->getID() >= 20 && activatedObject[i]->getID() <= 39 && activatedObject[i]->getOwner() == players[0]){
                        	((Building*)activatedObject[i])->setWaypoint(render->getCameraX()+mouseX, render->getCameraY()+mouseY);
                        }

                        Update();
                    }
                }
            }
        }
        if( event.type == SDL_MOUSEBUTTONUP ) {
            //If the left mouse button was pressed
            if( event.button.button == SDL_BUTTON_LEFT ) { 
				mousePressed = false;

                if(mouseSelect) {
                    int condition1 = mouseX-selectStartX;
                    int condition2 = mouseY-selectStartY;

                    int start1 = selectStartX+render->getCameraX();
                    int end1 = mouseX+render->getCameraX();

                    if(condition1 < 0) {
                        start1 = mouseX+render->getCameraX();
                        end1 = selectStartX+render->getCameraX();
                    }

                    int start2 = selectStartY+render->getCameraY();
                    int end2 = mouseY+render->getCameraY();

                    if(condition2 < 0) {
                        start2 = mouseY+render->getCameraY();
                        end2 = selectStartY+render->getCameraY();
                    }

                    if(std::abs(condition1) >= 10 && std::abs(condition2) >= 10) {
                        // Goes through the array of each unit
                        // Selecting each which is in range of box
                        for(unsigned int k = 0; k < units.size(); k++) {
                            if( units[k]->getX() + units[k]->getSize()*32 >= start1 &&
                                units[k]->getX() + units[k]->getSize()*32 <= end1  &&
                                units[k]->getY() + units[k]->getSize()*32 >= start2 &&
                                units[k]->getY() + units[k]->getSize()*32 <= end2 &&
                                activatedObject.size() < 14) {
                                    activatedObject.resize(activatedObject.size()+1);
                                    activatedObject[activatedObject.size()-1] = units[k];
                            }
                        }
                    }
                    mouseSelect = false;
                }
            }
        }
    } 
}

int Game::placeBuilding() {
    int size = gameState->getData()->getSize(buildMenuID);
    // Checks if building is buildable at this place
    for(int i = 0; i<size; i++) {
        for(int j = 0; j<size; j++) {
            if(m.tileUsed(  (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64+i,
                            (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64+j)) {
                return 1;
            }
            if(m.tileType(  (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64+i,
                            (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64+j) != 1) {
                return 1;
            }
        }
    }
    
    bool help = false;

    switch (buildMenuID) {
        case 21:
            if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
                buildings.push_back(new CommandCenter(  (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
                                                        (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
            } else {
                help = true;
            }
            break;
        case 22:
            if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
                buildings.push_back(new Barracks(   (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
                                                    (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
            } else {
                help = true;
            }
            break;
        case 23:
			if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
				buildings.push_back(new SupplyDepot(   (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
													   (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
			} else {
				help = true;
			}
			break;
        case 24:
			if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
				buildings.push_back(new ResearchLab(   (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
													   (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
			} else {
				help = true;
			}
			break;
        case 25:
			if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
				buildings.push_back(new Factory(   (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
												   (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
			} else {
				help = true;
			}
			break;
        case 26:
			if(gameState->getPlayers()->operator [](0)->getRessources()->getGold() >= gameState->getData()->getGoldPrices(buildMenuID)) {
				buildings.push_back(new MilAirport(   (render->getCameraX()+event.button.x-((render->getCameraX()+event.button.x)%64)-64)/64,
												   	  (render->getCameraY()+event.button.y-((render->getCameraY()+event.button.y)%64)-64)/64, gameState, players[0]));
			} else {
				help = true;
			}
			break;
    }
    if(help) {
        gameState->setAnnounceText("You don't have enough ressources.");
        gameState->updateAnnounce();
    } else {
    	// Subtract building costs from ressources.
        gameState->getPlayers()->operator [](0)->getRessources()->setGold(gameState->getPlayers()->operator [](0)->getRessources()->getGold()-gameState->getData()->getGoldPrices(buildMenuID));
        gameState->getPlayers()->operator [](0)->getRessources()->setGas(gameState->getPlayers()->operator [](0)->getRessources()->getGas()-gameState->getData()->getGasPrices(buildMenuID));
    }
    
    buildGrid = false;
    
    Update();
    return 0;
}

bool Game::checkWithinBorders(int condition1, int condition2) {
    if(condition1 >= 0 && condition1 <= CFG_VIDEO_WIDTH && condition2 >= 0 && condition2 <= CFG_VIDEO_HEIGHT) return true;
    else return false;
}

int Game::Update() {
	// Draw everything
	render->DrawBars(&activatedObject);

	if(buildGrid) {
		render->DrawBuildGrid(buildMenuID);
	}

	if(mouseSelect) {
		render->DrawSelect(selectStartX, selectStartY);
	}

	render->DrawUI(buildMenu, &activatedObject);
	render->DrawMinimap();
	render->DrawTooltip();

	//Announcement Text
	//std::cout << announceTimer.get_ticks() << std::endl;
	if(announce+2 >= timer.get_ticks()) {
		render->DrawText();
	}

	if(console) {
		std::cout << ">> ";
		std::string line;
		std::getline(std::cin,line);
		if(line == "quit" || line == "q") {
			console = false;
		} else {
			s.doString(line);
		}
		//s.drawConsole();
	}

    return 0;
}

int Game::Clean() {
    //Quit SDL
    SDL_Quit();
    return 0;
}

Game::~Game() {
    for(unsigned int i=0; i<units.size();i++) {
        delete(units.at(i));
    }

    for(unsigned int i=0; i<buildings.size();i++) {
        delete(buildings.at(i));
    }

    for(unsigned int i=0; i<players.size();i++) {
        delete(players.at(i));
    }

    for(int i=0; i<3; i++) {
    	delete(buildMenuTT[i]);
    }

    delete(fogOfWar);

    delete(gameState);
}
