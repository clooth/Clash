//
//  GameState.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//
#ifdef _WIN32
#include <windows.h>
#endif

#include <SDL/SDL.h>

#include "GameState.h"
#include "Building.h"
#include "Unit.h"
#include "Ressources.h"
#include "Map.h"
#include "Timer.h"
#include "GLRenderer.h"
#include "FogOfWar.h"
#include "Settings.h"

GameState::GameState(Map* m, std::vector<Building*>* b, std::vector<Unit*>* u, int* a, Timer* t, std::string* aS, std::vector<Player*>* p, Tooltip* to, FogOfWar* fov) {
    this->screen = NULL;
    this->renderer = NULL;

    this->fov = fov;
	this->map = m;
    this->buildings = b;
    this->units = u;
    this->players = p;
    this->announce = a;
    this->timer = t;
    this->announceText = aS;
    this->activeTooltip = to;

    this->settings = new Settings();
}

GameState::~GameState() {
	delete(settings);

	if(renderer != NULL)
		delete(renderer);
}

void GameState::setScreen(SDL_Surface* s) {
    this->screen = s;
}

void GameState::setRenderer(GLRenderer* r) {
    this->renderer = r;
}

GLRenderer* GameState::getRenderer() {
    return this->renderer;
}

SDL_Surface* GameState::getScreen() {
    return this->screen;
}

FogOfWar* GameState::getFOV() {
	return this->fov;
}

Tooltip* GameState::getActiveTooltip() {
    return activeTooltip;
}

void GameState::setActiveTooltip(Tooltip* t) {
    this->activeTooltip = t;
}

Map* GameState::getMap() {
    return map;
}

GameData* GameState::getData() {
    return &this->data;
}

std::vector<Building*>* GameState::getBuildings() {
    return buildings;
}

std::vector<Unit*>* GameState::getUnits() {
    return units;
}

std::vector<Player*>* GameState::getPlayers() {
    return players;
}

Timer* GameState::getTimer() {
    return timer;
}

void GameState::updateAnnounce() {
    (*this->announce) = (*timer).get_ticks();
}

void GameState::setAnnounceText(std::string announceText) {
    (*this->announceText) = announceText;
}

