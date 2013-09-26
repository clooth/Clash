//
//  Ressources.h
//  Clash
//
//  Created by Nico Hämäläinen on 9/26/13.
//  Copyright (c) 2013 Nico Hämäläinen. All rights reserved.
//

#ifndef __Clash__Ressources__
#define __Clash__Ressources__

class GameState;
class Player;

/**
 * Managing the Ressources of a player. Gold, Gas and Supply counts
 * get calculated and saved.
 */
class Ressources {
private:
    /**
     * One attribute for each ressource in game.
     */
    int gold, gas, currentSupply, maxSupply;

    /**
     * Pointer to player which these ressources are from.
     */
    Player* owner;

public:
    /**
     * Constructor of object. Does nothing beside setting each
     * ressource to startvalue for the player.
     *
     * @param owner owner of this ressources object
     */
    Ressources(Player* owner);

    /**
     * Getting current gold value
     *
     * @return int
     */
    int getGold() const;

    /**
     * Getting current gas value
     *
     * @return int
     */
    int getGas() const;

    /**
     * Calculating current supply and giving it back.
     *
     * It goes through the vector in GameState with pointers to each
     * unit and building to calculate how much supply is used and how much
     * you have got.
     *
     * @param gameState pointer to gamestate object
     * @return current supply used
     */
    int getCurrentSupply(GameState* gameState);

    /**
     * Calculating maximum supply and giving it back.
     *
     * It goes through the vector in GameState with pointers to each
     * unit and building to calculate how much supply is used and how much
     * you have got.
     *
     * @param gameState pointer to gamestate object
     * @return maximum supply player currently has
     */
    int getMaxSupply(GameState* gameState);

    /**
     * Change the gold you currently have (when you get something
     * from workers or build something which decreases this value)
     *
     * @param newGold value you want to change
     */
    void setGold(int newGold);

    /**
     * Change the gold you currently have (when you get something
     * from workers or build something which decreases this value)
     *
     * @param newGas value you want to change
     */
    void setGas(int newGas);
};

#endif /* defined(__Clash__Ressources__) */
