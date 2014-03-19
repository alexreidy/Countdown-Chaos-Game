//
//  Sentry.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/16/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Sentry.h"

Sentry::Sentry() : Structure() {}

Sentry::Sentry(sf::Sprite sprite) : Structure(sprite)
{
    centerOrigin();
    setCanAttack(true);
    updatesBetweenAttacks = 0;
    setDamageDealtPerAttack(5);
    maxAttackRange = 400;
    
    setRequirement(ENERGY, 0.5);
    
    reqAmountOf[ENERGY] = 5000;
    reqAmountOf[METAL] = 25;
    
    active = false;
}

Sentry::~Sentry() {}

void Sentry::attack(const GameEntity &opponent)
{
    if (active && isAbleToAttack()) {
        face(opponent.getPosition().x, opponent.getPosition().y);
        Projectile *p = new Projectile(projsprite, this, opponent.getPosition(), 25);
        launchProjectile(p);
    }
    
    GameEntity::attack(opponent);
}