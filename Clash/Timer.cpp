//
//  Timer.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

/**
 * @file
 * Timer methods
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL/SDL.h>
#endif

#include "Timer.h"

Timer::Timer() {
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start() {
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop() {
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause() {
    //If the timer is running and isn't already paused
    if((started == true) && (paused == false)) {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause() {
    //If the timer is paused
    if(paused == true) {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

float Timer::get_ticks() const {
    //If the timer is running
    if( started == true ) {
        //If the timer is paused
        if( paused == true ) {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        } else {
            //Return the current time minus the start time
            return (SDL_GetTicks() - startTicks) * 0.001f;;
        }
    }

    //If the timer isn't running
    return 0;
}