//
//  Tile.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#import "MapTile.h"

namespace Clash
{
    /**
     * Gets ID of this tile
     * @return id
     */
    int MapTile::get_id() const {
        return m_id;
    }

    /**
     * Gets runnable type of this tile
     * @return
     */
    int MapTile::get_type() const {
        return m_in_use;
    }

    /**
     * Gets the speed modifier for units walking on this tile
     * @return speed
     */
    int MapTile::get_speed_modifier() const {
        return m_speed_modifier;
    }

    /**
     * Return tile usage status
     * @return bool true if tile in use, otherwise false
     */
    bool MapTile::get_in_use() const {
        return m_in_use;
    }

    /**
     * Gets x coordinate on map of this tile
     * @return x coordinate
     */
    int MapTile::get_coordinate_x() const {
        return m_coordinate_x;
    }

    /**
     * Gets y coordinate on map of this tile
     * @return y coordinate
     */
    int MapTile::get_coordinate_y() const {
        return m_coordinate_y;
    }

    /**
     * Gets rgb values that this tile should get on the minimap
     * @return
     */
    float* MapTile::get_minimap_color() {
        return m_minimap_color;
    }

    /**
     * Setting this tile as used.
     * Called when something is built on top of this tile.
     * @param toggle value
     */
    void MapTile::set_in_use(bool toggle) {
        m_in_use = toggle;
    }
}