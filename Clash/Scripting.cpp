/*
 * File:   Scripting.cpp
 * Created on May 25th, 2011
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
 * Scripting class
 */

#include <iostream>
#include <sstream>

#include "GLRenderer.h"
#include "Scripting.h"
#include "GameState.h"
#include "Unit.h"
#include "Building.h"
#include "Player.h"

GameState* Scripting::gameState = NULL;

Scripting::Scripting() {
	index = 0;

	luaState = lua_open();
	luaL_openlibs(luaState);

	lua_register(luaState, "printUnits", printUnits);
	lua_register(luaState, "printBuildings", printBuildings);

	lua_register(luaState, "createBuilding", createBuilding);
	lua_register(luaState, "createUnit", createUnit);

	lua_register(luaState, "moveUnit", moveUnit);

	lua_register(luaState, "importMap", importMap);
	lua_register(luaState, "createPlayer", createPlayer);
}

void Scripting::doFile(std::string filename) {
	int state = luaL_loadfile(luaState, filename.c_str());
	if ( state == 0 ) {
		state = lua_pcall(luaState, 0, LUA_MULTRET, 0);
	}

	report_errors(state);
}

void Scripting::doString(std::string command) {
	const char* strLuaInput = command.c_str();
	int state = luaL_dostring(luaState, strLuaInput);

	report_errors(state);
}

void Scripting::report_errors(int status) {
	if ( status != 0 ) {
		std::cerr << "-- " << lua_tostring(luaState, -1) << std::endl;
		lua_pop(luaState, 1);
	}
}

int Scripting::createPlayer(lua_State* L) {
	//int argc = lua_gettop(L);
	float r,g,b;

	r = lua_tonumber(L, 1);
	g = lua_tonumber(L, 2);
	b = lua_tonumber(L, 3);
	int size = gameState->getPlayers()->size();

	std::cout << "-- creating player number "
			  << size << " with color(" << r
			  << "," << g << "," << b << ")..." << std::endl;

	gameState->getPlayers()->push_back(new Player(size));
	gameState->getPlayers()->at(size)->setColor(r,g,b);

	lua_pushnumber(L, size);
	return 1;
}

int Scripting::importMap(lua_State* L) {
	//int argc = lua_gettop(L);
	std::string filename;

	filename = lua_tostring(L, 1);
	std::cout << "-- loading map " << filename << "..." << std::endl;
	gameState->getMap()->import_map_from_file(filename);

	return 0;
}

int Scripting::printBuildings(lua_State* L) { // printBuildings()
	std::cout << "x\ty\tid\tplayerid" << std::endl;

	for(unsigned int i = 0; i < gameState->getBuildings()->size(); i++) {
		int x = gameState->getBuildings()->at(i)->getX(),
			y = gameState->getBuildings()->at(i)->getY(),
			id = gameState->getBuildings()->at(i)->getID();
		if(id==3) continue; // at the moment we don't want gold displayed
							// in printBuildings(). might change in future.
		std::stringstream playerid;
		if(gameState->getBuildings()->at(i)->getOwner() != NULL)
			playerid << gameState->getBuildings()->at(i)->getOwner()->getID();
		else
			playerid << "NaN";

		std::cout << x << "\t" << y << "\t" << id << "\t" << playerid.str() << std::endl;
	}

	return 0;
}


int Scripting::printUnits(lua_State* L) { // printUnits()
	std::cout << "x\ty\tid\tplayerid" << std::endl;

	for(unsigned int i = 0; i < gameState->getUnits()->size(); i++) {
		int x = gameState->getUnits()->at(i)->getX(),
			y = gameState->getUnits()->at(i)->getY(),
			id = gameState->getUnits()->at(i)->getID(),
			playerid = gameState->getUnits()->at(i)->getOwner()->getID();

		std::cout << x << "\t" << y << "\t" << id << "\t" << playerid << std::endl;
	}

	return 0;
}

int Scripting::moveUnit(lua_State* L) { // moveUnit(id, x, y)
	int argc = lua_gettop(L);

	int id, x, y;

	std::cout << "-- moveUnit called with " << argc << " arguments:" << std::endl << "-- ";
	for ( int n=1; n<=argc; ++n ) std::cout << n << ": " << lua_tostring(L, n) << " ";
	std::cout << std::endl;

	id = lua_tonumber(L, 1);
	x = lua_tonumber(L, 2);
	y = lua_tonumber(L, 3);

	gameState->getUnits()->at(id)->moveTo(x,y);

	return 0;
}

int Scripting::createBuilding(lua_State* L) { // createBuilding(x, y, id, playerid, buildtime)
	int argc = lua_gettop(L);

	int x = 0, y = 0, id = 0, playerid = 0, buildtime = 0;

	std::cout << "-- createBuilding called with " << argc << " arguments:" << std::endl << "-- ";
	for ( int n=1; n<=argc; ++n ) std::cout << n << ": " << lua_tostring(L, n) << " ";
	std::cout << std::endl;
	if(argc < 3) { std::cerr << "-- Not enough arguments" << std::endl; }
	else if(argc > 5) { std::cerr << "-- Too many arguments" << std::endl; }

	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);
	id = lua_tonumber(L, 3);
	if(argc >= 4)
		playerid = lua_tonumber(L, 4);
	if(argc == 5)
		buildtime = lua_tonumber(L, 5);
	else buildtime = 1;

	Building* building = NULL;

	switch(id) {
		case 3:
			building = new Gold(x,y,gameState);
		case 21:
			building = new CommandCenter(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 22:
			building = new Barracks(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 23:
			building = new SupplyDepot(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 24:
			building = new ResearchLab(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 25:
			building = new Factory(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 26:
			building = new MilAirport(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
		case 27:
			building = new Tower(x,y,gameState,gameState->getPlayers()->at(playerid), buildtime);
			break;
	}

	gameState->getBuildings()->push_back(building);

	int number = 0;

	for(unsigned int i=0; i<gameState->getBuildings()->size();i++) {
		if(gameState->getBuildings()->at(i) == building) {
			number = i;
			break;
		}
	}

	lua_pushnumber( L, number );

	return 1;
}

int Scripting::createUnit(lua_State* L) { // createUnit(x, y, id, playerid)
	int argc = lua_gettop(L);

	int x, y, id, playerid;

	std::cout << "-- createUnit called with " << argc << " arguments:" << std::endl << "-- ";
	for ( int n=1; n<=argc; ++n ) std::cout << n << ": " << lua_tostring(L, n) << " ";
	std::cout << std::endl;
	if(argc < 4) { std::cerr << "-- Not enough arguments" << std::endl; }
	else if(argc > 4) { std::cerr << "-- Too many arguments" << std::endl; }

	x = lua_tonumber(L, 1);
	y = lua_tonumber(L, 2);
	id = lua_tonumber(L, 3);
	playerid = lua_tonumber(L, 4);
	Unit* unit = NULL;

	switch(id) {
		case 41:
			unit = new Soldier(x,y,gameState,gameState->getPlayers()->at(playerid));
			break;
		case 42:
			unit = new Worker(x,y,gameState,gameState->getPlayers()->at(playerid));
			break;
	}

	gameState->getUnits()->push_back(unit);

	int number = 0;

	for(unsigned int i=0; i<gameState->getUnits()->size();i++) {
		if(gameState->getUnits()->at(i) == unit) {
			number = i;
			break;
		}
	}

	lua_pushnumber( L, number );

	return 1;
}

Scripting::~Scripting() {
	lua_close( luaState );
}
