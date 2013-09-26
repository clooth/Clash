/*
 * File:   GLTesselation.cpp
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
 * GLTesselation functions
 */
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "GLTesselation.h"

// Windows version does not like callback,
// so it only gets defined when _WIN32 isn't set.
// Other operating systems need it though.
#ifndef _WIN32
#define CALLBACK
#endif

struct cleanupItem {
    GLdouble *pvert;
    cleanupItem* next;
};

cleanupItem* cleanupStore = NULL;

void AddToCleanup(GLdouble *ptr)
{
    cleanupItem* temp=new cleanupItem;
    temp->pvert=ptr;
    temp->next = cleanupStore;
    cleanupStore = temp;
}

void DoCleanup()
{
    if(cleanupStore == NULL) return;
	cleanupItem* ci= cleanupStore;
    while(ci != NULL)
    {
        cleanupItem* temp = ci;
        delete[] ci->pvert;
        ci = ci->next;
        delete temp;
    };
    cleanupStore = NULL;
}

void CALLBACK BeginCallback(GLenum type)
{
    glBegin(type);
}

void CALLBACK EndCallback()
{
    glEnd();
}

void CALLBACK VertexCallback(GLvoid *vertex)
{
	glVertex3dv( (const double *)vertex );
}

void CALLBACK CombineCallback(GLdouble coords[3], GLdouble *data[4], GLfloat weight[4], GLdouble **dataOut )
{
	GLdouble *vert = new GLdouble[3];
	vert[0] = coords[0];
    vert[1] = coords[1];
    vert[2] = coords[2];
	*dataOut = vert;
	AddToCleanup(vert);
}

void CALLBACK ErrorCallback(GLenum errorCode)
{ }

void DrawPolygon(clipper::Polygons &pgs)
{
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

	GLUtesselator* tess = gluNewTess();
    gluTessCallback(tess, GLU_TESS_BEGIN, (void (CALLBACK*)())&BeginCallback);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void (CALLBACK*)())&VertexCallback);
    gluTessCallback(tess, GLU_TESS_END, (void (CALLBACK*)())&EndCallback);
    gluTessCallback(tess, GLU_TESS_COMBINE, (void (CALLBACK*)())&CombineCallback);
    gluTessCallback(tess, GLU_TESS_ERROR, (void (CALLBACK*)())&ErrorCallback);

    gluTessProperty(tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_FALSE);

	gluTessBeginPolygon(tess, NULL);
	for (clipper::Polygons::size_type i = 0; i < pgs.size(); ++i)
	{
		gluTessBeginContour(tess);
		for (clipper::Polygon::size_type j = 0; j < pgs[i].size(); ++j)
		{
			GLdouble *vert = new GLdouble[3];
			vert[0] = (GLdouble)pgs[i][j].X;
			vert[1] = (GLdouble)pgs[i][j].Y;
			vert[2] = 0;
			AddToCleanup(vert);
			gluTessVertex(tess, vert, vert);
		}
		gluTessEndContour(tess);
	}
	gluTessEndPolygon(tess);
	DoCleanup();

	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);

	glLineWidth(1.2f);
	gluTessProperty(tess, GLU_TESS_BOUNDARY_ONLY, GL_TRUE); //GL_FALSE

	gluTessBeginPolygon(tess, NULL);
	for (clipper::Polygons::size_type i = 0; i < pgs.size(); ++i)
	{
		gluTessBeginContour(tess);
		for (clipper::Polygon::size_type j = 0; j < pgs[i].size(); ++j)
		{
			GLdouble *vert = new GLdouble[3];
			vert[0] = (GLdouble)pgs[i][j].X;
			vert[1] = (GLdouble)pgs[i][j].Y;
			vert[2] = 0;
			AddToCleanup(vert);
			gluTessVertex(tess, vert, vert);
		}
		gluTessEndContour(tess);
	}
	gluTessEndPolygon(tess);

	//final cleanup ...
	gluDeleteTess(tess);
	DoCleanup();
}
