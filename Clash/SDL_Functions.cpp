//
//  SDL_Functions.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "SDL_Functions.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
const Uint32 rmask32 = 0xff000000;
const Uint32 gmask32 = 0x00ff0000;
const Uint32 bmask32 = 0x0000ff00;
const Uint32 amask32 = 0x000000ff;

const Uint32 rmask24 = 0xff0000;
const Uint32 gmask24 = 0x00ff00;
const Uint32 bmask24 = 0x0000ff;
#else
const Uint32 rmask32 = 0x000000ff;
const Uint32 gmask32 = 0x0000ff00;
const Uint32 bmask32 = 0x00ff0000;
const Uint32 amask32 = 0xff000000;

const Uint32 rmask24 = 0x0000ff;
const Uint32 gmask24 = 0x00ff00;
const Uint32 bmask24 = 0xff0000;
#endif

SDL_Surface* colorize(int red, int green, int blue) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 240, 32, rmask32, gmask32, bmask32, amask32);
    SDL_Rect * rect = new SDL_Rect();
    rect->x = rect->y = 0;
    rect->w = 320;
    rect->h = 240;
    Uint32 color = SDL_MapRGBA(surface->format, red, green, blue, 90); //100 means quite transparent
    SDL_FillRect(surface, rect, color);
    delete(rect);
    return surface;
}

SDL_Surface *load_image( std::string filename ) {
    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;
    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;
    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If nothing went wrong in loading the image
    if( loadedImage != NULL ) {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip ) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void put_pixel(SDL_Surface *surface, int x, int y, int pixel) {
    // Getting palette of surface
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to set
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    // Setting new color depending of palette
    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

Uint32 get_pixel( SDL_Surface *surface, int x, int y ) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}