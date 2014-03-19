//
//  EnemySoldier.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/19/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "EnemySoldier.h"

EnemySoldier::EnemySoldier() : Mobile() {}

EnemySoldier::EnemySoldier(sf::Sprite sprite) : Mobile(sprite)
{
    setFriendly(false);
    centerOrigin();
    updatesBetweenAttacks = 2;
    setCanAttack(true);
}

EnemySoldier::~EnemySoldier() {}

void EnemySoldier::attack(const GameEntity &opponent)
{
    if (isInRadius(getMaxAttackRange(), opponent.getPosition())) {
        if (isMoving()) {
            allowMoveOnUpdate(false);
        }
        
        lateralFace(opponent.getPosition().x, opponent.getPosition().y);
        
        if (isAbleToAttack()) {
            Projectile *p = new Projectile(projsprite, this, opponent.getPosition(), 10);
            launchProjectile(p);
        }
        
        GameEntity::attack(opponent);
    } else {
        if (!isMoving()) {
            makeWayTo(
                opponent.getPosition().x + rsign(rin(getMaxAttackRange())),
                opponent.getPosition().y + rsign(rin(getMaxAttackRange()))
            );
        }
    }
}

void EnemySoldier::makeWayTo(float x, float y)
{
    lateralFace(x, y);
    Mobile::makeWayTo(x, y);
}

void EnemySoldier::update()
{
    Mobile::update();
    allowMoveOnUpdate(true);
}