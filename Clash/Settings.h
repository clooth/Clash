/*
 * File:   Settings.h
 * Created on Jun 6, 2011
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
 * Settings definition
 */
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <vector>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

/**
 * Loads config files and holds the configuration state of the game.
 */
class Settings {
private:
	/**
	 * Variables map which stores content of config files
	 */
	po::variables_map vm;
public:
	/**
	 * Initializing settings, loading config files
	 */
	Settings();

	/**
	 * Destroying settings, should only be called by application exit.
	 */
	virtual ~Settings();

	/**
	 * Giving back option value of a given key
	 *
	 * Template
	 *
	 * @param key name of option
	 * @return value of option
	 */
	template < typename T >
	T getValue(std::string key) {
		return vm[key].as<T>();
	}
};

#endif /* SETTINGS_H_ */
