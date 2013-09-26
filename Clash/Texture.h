/*
 * File:   Texture.h
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
 * Texture header. Load textures and saves them in an object.
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

/**
 * This type contains data for Texture class
 */
typedef struct _glTex
{
	/**
	 * ID of texture in OpenGL
	 */
    GLuint data;

    /**
     * Size of this texture in OpenGL
     */
    unsigned int width;

    /**
     * Size of this texture in OpenGL
     */
    unsigned int height;
} glTex;

/**
 * This class saves textures for OpenGL Drawings
 */
class Texture {
private:
	/**
	 * Object that contains texture in sdl format
	 */
	SDL_Surface* sdl_tex;
	/**
	 * Object that contains texture
	 */
	glTex tex;

	/**
	 * Method that loads a SDL_Surface object and transforms it
	 * to an OpenGL texture.
	 *
	 * @param surf SDL_Surface to transform
	 * @return texture object
	 */
	glTex surfaceToTexture(SDL_Surface *surf);
public:
	/**
	 * Constructor, loads the file and creates texture
	 *
	 * @param filename filename to load this texture object with
	 */
	Texture(std::string filename);

	/**
	 * Destructor, free texture memory.
	 */
	virtual ~Texture();

	/**
	 * Gives back id of texture in OpenGL
	 *
	 * @return id of texture
	 */
	GLuint getTexture();

	/**
	 * Gives back width size of this texture in OpenGL
	 *
	 * @return width size of texture
	 */
	GLfloat getTextureWidth();

	/**
	 * Gives back height size of this texture in OpenGL
	 *
	 * @return height size of texture
	 */
	GLfloat getTextureHeight();
};

#endif /* TEXTURE_H_ */
