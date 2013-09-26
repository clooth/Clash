/*
 * File:   Texture.cpp
 * Created on May 13, 2011
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
 * Texture functions. Load textures and saves them in an object.
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <string>

#include "Texture.h"
#include "SDL_Functions.h"

Texture::Texture(std::string filename)
{
	sdl_tex = load_image(filename);
	// Loads texture into memory.
	tex = surfaceToTexture(sdl_tex);
}

GLuint Texture::getTexture()
{
	return tex.data;
}

Texture::~Texture()
{
	SDL_FreeSurface(sdl_tex);
}

glTex Texture::surfaceToTexture( SDL_Surface *surf )
{
    glTex texture;

    // save texture size, this might be needed later
    texture.width = surf->w;
    texture.height = surf->h;

    // color format of image
    // GL_BGR is an extension. OSX and Linux version of OpenGL
    // like it more without _EXT, but Windows implementation needs it.
#ifdef _WIN32
    GLenum SourceFormat = GL_BGR_EXT;
#else
	GLenum SourceFormat = GL_BGR;
#endif

	// for some reason bmps activate this,
	// when they don't have alpha channel, WTF?
    if ( surf->flags & SDL_SRCALPHA ) {
    	// GL_BGRA is an extension. OSX and Linux version of OpenGL
    	// like it more without _EXT, but Windows implementation needs it.
#ifdef _WIN32
        SourceFormat = GL_BGRA_EXT;
#else
		SourceFormat = GL_BGRA;
#endif
    } else if ( surf->format->BitsPerPixel == 8 ) {
        SourceFormat = GL_COLOR_INDEX;
    }

    // get an empty slot from texture memory
    glGenTextures( 1, &texture.data );

    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture( GL_TEXTURE_2D, texture.data );

    // Generate The Texture
    glTexImage2D( GL_TEXTURE_2D, 0, surf->format->BytesPerPixel, \
    surf->w, surf->h, 0, SourceFormat, GL_UNSIGNED_BYTE, surf->pixels );

    // Linear Filtering, 2D doesn't need very good filters I guess
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texture;
}

GLfloat Texture::getTextureWidth()
{
	return tex.width;
}

GLfloat Texture::getTextureHeight()
{
	return tex.height;
}

