/*
 * File:   main.cpp
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
 * Main file which starts application
 */
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <iostream>
#include "Game.h"

/**
 * Gets called when program is starting. SDL automatically renames
 * it to SDLMain and calls it after initializing it own stuff.
 *
 * If we're on OSX, it changes the relative path of the programm to
 * be within the .app bundle.
 *
 * @param argc not used
 * @param argv not used
 * @return nothing
 */
int main(int argc, char*argv[])
{
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);

    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    // ----------------------------------------------------------------------------

    Game mainGame;
    mainGame.init();
    return 0;
}

