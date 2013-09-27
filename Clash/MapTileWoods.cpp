//
//  MapTileWoods.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/27/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "MapTileWoods.h"

namespace Clash
{
    MapTileWoods::MapTileWoods(int x, int y)
    {
        m_id = 2;

        // Place to given coordinates
        m_coordinate_x = x;
        m_coordinate_y = y;

        // Normal speed
        m_speed_modifier = 0;

        // Only air units can move on this tile
        m_type = 3;

        // By default the tile is not in use
        m_in_use = false;

        // Nice dark green color
        m_minimap_color[0] = 0.0f;
        m_minimap_color[1] = 0.4f;
        m_minimap_color[2] = 0.0f;
    }
}