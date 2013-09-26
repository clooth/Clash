//
//  GameData.cpp
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#include "GameData.h"

GameData::GameData() {
    name.resize(128);
    goldPrices.resize(128);
    gasPrices.resize(128);
    supplyCost.resize(128);
    buildTimes.resize(128);
    description.resize(128);
    size.resize(128);
    life.resize(128);

    name[21] = "Command Center";
    goldPrices[21] = 400;
    gasPrices[21] = 0;
    supplyCost[21] = 15;
    buildTimes[21] = 120;
    life[21] = 3000;
	size[21] = 3;
    description[21].resize(4);
    description[21][0] = "Main building, gives the ability to";
    description[21][1] = "build workers as well as 15 supply.";
    description[21][2] = "Don't lose all your Command Centers";
    description[21][3] = "or the enemy will be able to see you.";

    name[22] = "Barracks";
    goldPrices[22] = 100;
    gasPrices[22] = 0;
    supplyCost[22] = 0;
    buildTimes[22] = 3;
    life[22] = 1500;
    size[22] = 3;
    description[22].resize(4);
    description[22][0] = "The barracks are the main station";
    description[22][1] = "to build infantry units for your";
    description[22][2] = "army. It is also an dependency for";
    description[22][3] = "further higher tiered buildings.";

    name[23] = "Supply Depot";
    goldPrices[23] = 100;
    gasPrices[23] = 0;
    supplyCost[23] = 8;
    buildTimes[23] = 30;
    life[23] = 800;
	size[23] = 2;
    description[23].resize(4);
    description[23][0] = "The supply depot give you 8";
    description[23][1] = "supply to build units. Every unit";
    description[23][2] = "you build in game uses a certain";
    description[23][3] = "amount of supply while it exists.";

    name[24] = "Research Lab";
    goldPrices[24] = 500;
    gasPrices[24] = 300;
    supplyCost[24] = 0;
    buildTimes[24] = 120;
    life[24] = 2400;
	size[24] = 3;
    description[24].resize(4);
    description[24][0] = "Here you can research additional";
    description[24][1] = "actions and buffs for your units.";
    description[24][2] = "These are optional but can help";
    description[24][3] = "in fights.";

    name[25] = "Factory";
    goldPrices[25] = 400;
    gasPrices[25] = 250;
    supplyCost[25] = 0;
    buildTimes[25] = 45;
    life[25] = 1400;
	size[25] = 3;
    description[25].resize(4);
    description[25][0] = "The factory is the right place";
    description[25][1] = "to build mechanical units like";
    description[25][2] = "tanks.";
    description[25][3] = "";

    name[26] = "Military Airport";
    goldPrices[26] = 400;
    gasPrices[26] = 250;
    supplyCost[26] = 0;
    buildTimes[26] = 45;
    life[26] = 1400;
	size[26] = 3;
    description[26].resize(4);
    description[26][0] = "At the military airport, air";
    description[26][1] = "units are getting build for";
    description[26][2] = "fighting from another Z-Axis.";
    description[26][3] = "";

    name[27] = "Tower";
	goldPrices[27] = 150;
	gasPrices[27] = 50;
	supplyCost[27] = 0;
	buildTimes[27] = 45;
	life[27] = 900;
	size[27] = 1;
	description[27].resize(4);
	description[27][0] = "The tower is lonely, so you";
	description[27][1] = "should build a lot of them";
	description[27][2] = "next to each other.";
	description[27][3] = "It has no abilities though.";

    name[41] = "Soldier";
    goldPrices[41] = 30;
    gasPrices[41] = 0;
    supplyCost[41] = 1;
    buildTimes[41] = 1;
    life[41] = 150;
	size[41] = 0.5;
    description[41].resize(4);
    description[41][0] = "Light infrantry unit. Does small";
    description[41][1] = "ranged damage and has a moderate";
    description[41][2] = "amount of hp.";
    description[41][3] = "";

    name[42] = "Worker";
    goldPrices[42] = 50;
    gasPrices[42] = 0;
    supplyCost[42] = 1;
    buildTimes[42] = 2;
    life[42] = 100;
	size[42] = 0.5;
    description[42].resize(4);
    description[42][0] = "This unit is useful to collect";
    description[42][1] = "different ressources like gold";
    description[42][2] = "or gas. It could also be a meat-";
    description[42][3] = "shield in your army.";
}

std::string GameData::getName(int id) const {
    return name[id];
}


std::string GameData::getDescription(int id, int n) const {
    return description[id][n];
}

int GameData::getGoldPrices(int id) const {
    return goldPrices[id];
}

int GameData::getGasPrices(int id) const {
    return gasPrices[id];
}

int GameData::getSupply(int id) const {
    return supplyCost[id];
}

int GameData::getBuildTimes(int id) const {
    return buildTimes[id];
}

int GameData::getLife(int id) const {
    return life[id];
}

float GameData::getSize(int id) const {
    return size[id];
}
