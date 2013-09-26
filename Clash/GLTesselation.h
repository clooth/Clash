/*
 * File:   GLTesselation.h
 * Created on May 18, 2011
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
 * GLRTesselation definition
 */

#ifndef GLTESSELATION_H_
#define GLTESSELATION_H_

#include "clipper.hpp"

/**
 * Draws clipped polygons on the screen. Used by
 * fog of war class.
 *
 * @param pgs object with clipped polygons
 */
void DrawPolygon(clipper::Polygons &pgs);

#endif /* GLTESSELATION_H_ */
