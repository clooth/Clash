//
//  Tile.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Tile__
#define __Clash__Tile__

#include <iostream>

namespace Clash
{
    /**
     * A Single Map Tile Representation
     */
    class MapTile
    {
    public:
        /**
         * Gets ID of this tile
         * @return id
         */
        int get_id() const;

        /**
         * Gets runnable type of this tile
         * @return
         */
        int get_type() const;

        /**
         * Gets the speed modifier for units walking on this tile
         * @return speed
         */
        int get_speed_modifier() const;

        /**
         * Return tile usage status
         * @return bool true if tile in use, otherwise false
         */
        bool get_in_use() const;

        /**
         * Gets x coordinate on map of this tile
         * @return x coordinate
         */
        int get_coordinate_x() const;

        /**
         * Gets y coordinate on map of this tile
         * @return y coordinate
         */
        int get_coordinate_y() const;

        /**
         * Gets rgb values that this tile should get on the minimap
         * @return
         */
        float* get_minimap_color();

        /**
         * Setting this tile as used. Called when something is built on top of this tile.
         * @param toggle value
         */
        void set_in_use(bool toggle);

    protected:
        /**
         * ID of this tile
         */
        int m_id;

        /**
         * Type of the terrain tile
         * 1 = walkable by every unit
         * 2 = walkable by water or air
         * 3 = walkable by air
         */
        int m_type;

        /**
         * The speed modifier for the tile
         */
        int m_speed_modifier;
        
        /**
         * Is this tile in use by a building?
         */
        bool m_in_use;
        
        /**
         * Map coordinates
         */
        int m_coordinate_x;
        
        /**
         * Map coordinates
         */
        int m_coordinate_y;
        
        /**
         * The color this tile gets on the minimap
         */
        float m_minimap_color[3];
    };
}

// Import all tile types here
#include "MapTileGrass.h"

#endif /* defined(__Clash__Tile__) */
