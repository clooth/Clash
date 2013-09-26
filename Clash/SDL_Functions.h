//
//  SDL_Functions.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__SDL_Functions__
#define __Clash__SDL_Functions__

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#endif

/**
 * It creates a transparent surface in a color chosen with a
 * 100 Alpha Channel which can placed on other surfaces to
 * create a colorizing effect.
 *
 * @param red red color
 * @param blue blue color
 * @param green green color
 * @return transparent surface with that color in 100 alpha
 */
SDL_Surface *colorize(int red, int blue, int green);

/**
 * Loading an image from a given file string. Optimize that image
 * to the current palette and give it back as surface.
 *
 * @param filename file string
 * @return surface with image
 */
SDL_Surface *load_image(std::string filename);

/**
 * Blipping the given surface to another surface
 *
 * @param x x coordinate where to blip given surface on destination
 * @param y y coordinate where to blip given surface on destination
 * @param source given surface
 * @param destination destination surface
 * @param clip clip only if you blip a certain part of surface.
 */
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);

/**
 * Changing color of a pixel in a given Surface
 *
 * @param surface given surface
 * @param x x coordinate of pixel
 * @param y y coordinate of pixel
 * @param pixel hash code of color
 */
void put_pixel(SDL_Surface *surface, int x, int y, int pixel);

/**
 * Get the color of a pixel in a given surface
 *
 * @param surface given surface
 * @param x x coordinate to get color of pixel from
 * @param y y coordinate to get color of pixel from
 * @return Uint32 of colorcode
 */
Uint32 get_pixel( SDL_Surface *surface, int x, int y );

#endif /* defined(__Clash__SDL_Functions__) */
