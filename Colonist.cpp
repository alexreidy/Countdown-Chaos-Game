//
//  Colonist.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/27/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Colonist.h"

Colonist::Colonist() : Mobile() {}

Colonist::Colonist(sf::Sprite sprite) : Mobile(sprite)
{
    setFriendly(true);
    centerOrigin();
    updatesBetweenAttacks = 2;
    setCanAttack(true);
    
    setRequirement(FOOD, 0.01);
    setRequirement(WATER, 0.05);
    setRequirement(OXYGEN, 0.09);
    
    active = true;
}

Colonist::~Colonist()
{
    
}

bool Colonist::isAbleToAttack()
{
    return GameEntity::isAbleToAttack();
}

void Colonist::attack(const GameEntity &opponent) // AI might want to save opponent reference... but this may be complicated when opponent dies. Hm. Easier if mutual opponents only
{
    lateralFace(opponent.getPosition().x, opponent.getPosition().y);
    
    if (isAbleToAttack()) {
        Projectile *p = new Projectile(projsprite, this, opponent.getPosition(), 10);
        p->explodes = true;
        p->explosionBlastRadius = 50;
        p->explosions = explosions;
        launchProjectile(p);
    }
    
    GameEntity::attack(opponent);
}

void Colonist::makeWayTo(float x, float y)
{
    lateralFace(x, y);
    Mobile::makeWayTo(x, y);
}

void Colonist::onLacking(resource r)
{
    switch (r) {
        case FOOD:
            //changeHp(-0.1); // Might want to make machines produce things on intervals... like crops growing.
            break;// So player can't turn colonists off to save resources :)
        case WATER:
            changeHp(-0.2);
            break;
        case OXYGEN:
            changeHp(-0.5);
            break;
    }
}

void Colonist::update()
{
    // So player can't turn colonists off to save resources :)
    active = true;
    Mobile::update();
}