/*
 * File:   Settings.cpp
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
 * Settings functions
 */
#include <fstream>
#include <iostream>

#include "Settings.h"

Settings::Settings() {
	po::options_description video, audio, input;

	video.add_options() ("Video.fullscreen", po::value<bool>()->default_value(false))
					    ("Video.width", po::value<int>()->default_value(1024))
					    ("Video.height", po::value<int>()->default_value(768))
					    ("Video.bpp", po::value<int>()->default_value(16));

	audio.add_options() ("Audio.sound", po::value<bool>()->default_value(false))
						("Audio.volumeSound", po::value<int>()->default_value(100))
						("Audio.volumeMusic", po::value<int>()->default_value(100));

	input.add_options() ("Input.scroll.speed", po::value<int>()->default_value(12))
						("Input.scroll.mouse", po::value<bool>()->default_value(false))
						("Input.scroll.key.left", po::value<std::string>()->default_value("SDLK_LEFT"))
						("Input.scroll.key.right", po::value<std::string>()->default_value("SDLK_RIGHT"))
						("Input.scroll.key.up", po::value<std::string>()->default_value("SDLK_UP"))
						("Input.scroll.key.down", po::value<std::string>()->default_value("SDLK_DOWN"))
						("Input.key.quit", po::value<std::string>()->default_value("SDLK_ESCAPE"))
						("Input.key.console", po::value<std::string>()->default_value("SDLK_CARET"))
						("Input.key.buildMenu", po::value<std::string>()->default_value("SDLK_b"));

	po::options_description desc;
	desc.add(video).add(audio).add(input);
	std::ifstream mainCfg("cfg/main.cfg");
	po::store(po::parse_config_file(mainCfg, desc), vm);
	po::notify(vm);
}

Settings::~Settings() {
	// TODO Auto-generated destructor stub
}
