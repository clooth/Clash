//
//  MapTileGrass.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/27/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "MapTileGrass.h"

namespace Clash
{
    MapTileGrass::MapTileGrass(int x, int y)
    {
        m_id = 0;

        // Place to given coordinates
        m_coordinate_x = x;
        m_coordinate_y = y;

        // Normal speed
        m_speed_modifier = 1;

        // Everyone can walk on this tile
        m_type = 1;

        // By default the tile is not in use
        m_in_use = false;

        // Nice green color
        m_minimap_color[0] = 0.0f;
        m_minimap_color[1] = 0.6f;
        m_minimap_color[2] = 0.0f;
    }
}
