//
//  MapTileWater.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/27/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "MapTileWater.h"

namespace Clash
{
    MapTileWater::MapTileWater(int x, int y)
    {
        m_id = 1;

        // Place to given coordinates
        m_coordinate_x = x;
        m_coordinate_y = y;

        // Normal speed
        m_speed_modifier = 0;

        // Only water and air units can move on this tile
        m_type = 2;

        // By default the tile is not in use
        m_in_use = false;

        // Nice blue color
        m_minimap_color[0] = 0.0f;
        m_minimap_color[1] = 0.0f;
        m_minimap_color[2] = 0.7f;
    }
}
