//
//  GameData.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__GameData__
#define __Clash__GameData__

#include <vector>
#include <string>

/**
 * This class contains most of the game data.
 */
class GameData {
public:
	/**
	 * Constructor, filling object with data.
	 */
    GameData();

    /**
     * Gives back gold price of object
     *
     * @param id id of object
     * @return int
     */
    int getGoldPrices(int id) const;

    /**
     * Gives back gas price of object
     *
     * @param id id of object
     * @return int
     */
    int getGasPrices(int id) const;

    /**
     * Gives back supply of object. Either how much it gives (buildings) or how
     * much they cost (units).
     *
     * @param id id of object
     * @return int
     */
    int getSupply(int id) const;

    /**
     * Gives back build time of object
     *
     * @param id id of object
     * @return int
     */
    int getBuildTimes(int id) const;

    /**
     * Gives back maximum life of object
     *
     * @param id id of object
     * @return int
     */
    int getLife(int id) const;

    /**
     * Gives back relative (compared to tile size) size of object
     *
     * @param id id of object
     * @return int
     */
    float getSize(int id) const;

    /**
     * Gives back name of object
     *
     * @param id id of object
     * @return string
     */
    std::string getName(int id) const;

    /**
     * Gives back description texts for tooltips
     *
     * @param id id of object
     * @param n line of description text
     * @return string
     */
    std::string getDescription(int id, int n) const;
private:
    /**
     * This vector saves the names of an id
     */
    std::vector<std::string> name;

    /**
     * This vector saves the maximum life of the id
     */
    std::vector<int> life;

    /**
     * This vector saves the size of the id
     */
    std::vector<float> size;

    /**
     * This vector saves the description (for tooltip) of an id
     */
    std::vector<std::vector<std::string> > description;

    /**
     * This vector saves the gold prices of an id
     */
    std::vector<int> goldPrices;

    /**
     * This vector saves the gas prices of an id
     */
    std::vector<int> gasPrices;

    /**
     * This vector saves the supply of an id
     */
    std::vector<int> supplyCost;

    /**
     * This vector saves the build times of an id
     */
    std::vector<int> buildTimes;
};

#endif /* defined(__Clash__GameData__) */
