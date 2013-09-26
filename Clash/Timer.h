//
//  Timer.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Timer__
#define __Clash__Timer__

/**
 * This class creates a startable, stoppable and pausable timer
 * which runs in another thread along with the games.
 */
class Timer
{
private:
    /**
     * Time when the timer starts
     */
    float startTicks;

    /**
     * Time when the timer gets paused
     */
    float pausedTicks;

    /**
     * Status of the timer, if it is paused
     */
    bool paused;

    /**
     * Status of the timer, if it is started
     */
    bool started;

public:
    /**
     * Constructor
     * does nothing beside initializing attributes with
     * (int)zero or (bool)false.
     */
    Timer();

    /**
     * Start the Timer
     */
    void start();

    /**
     * Stop the timer
     */
    void stop();

    /**
     * Pause the timer
     */
    void pause();

    /**
     * Unpause the timer
     */
    void unpause();

    /**
     * Get current time of the timer.
     *
     * @return current Time
     */
    float get_ticks() const;
};

#endif /* defined(__Clash__Timer__) */
