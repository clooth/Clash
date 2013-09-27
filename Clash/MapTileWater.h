//
//  MapTileWater.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/27/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__MapTileWater__
#define __Clash__MapTileWater__

#include "MapTile.h"

namespace Clash
{
    class MapTileWater : public MapTile
    {
    public:
        /**
         * Default constructor
         */
        MapTileWater(int x, int y);
    };
}

#endif /* defined(__Clash__MapTileWater__) */
