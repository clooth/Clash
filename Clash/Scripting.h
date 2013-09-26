/*
 * File:   Scripting.h
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
 * Scripting definitions
 */

#ifndef SCRIPTING_H_
#define SCRIPTING_H_

#include <list>

extern "C"
{
	#include "lua/lua.h"
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
}

class GameState;

class Scripting {
private:
	lua_State* luaState;
	int index;

	void report_errors(int status);

	static int printUnits(lua_State* L);
	static int printBuildings(lua_State* L);
	static int createUnit(lua_State* L);
	static int createBuilding(lua_State* L);
	static int moveUnit(lua_State* L);
	static int importMap(lua_State* L);
	static int createPlayer(lua_State* L);

public:
	static GameState* gameState;

	Scripting();
	virtual ~Scripting();

	void doString(std::string command);
	void doFile(std::string filename);
};

#endif /* SCRIPTING_H_ */
