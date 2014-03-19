//
//  Projectile.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/27/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Projectile.h"

Projectile::Projectile() {}

Projectile::Projectile(sf::Sprite sprite) : Mobile(sprite) {}

Projectile::~Projectile() {}

Projectile::Projectile(sf::Sprite sprite, const GameEntity *launcher, sf::Vector2f target, float speed) : Mobile(sprite)
{
    setSpeed(speed);
    setPosition(launcher->getPosition());
    this->collisionDamage = launcher->getDamageDealtPerAttack();
    wasLaunchedByFriend = launcher->isFriendly();
    makeWayTo(target.x, target.y);
    
    active = false;
    planToDieAfter(100 + rsign(rin(20)));
}

void Projectile::update()
{
    move();
    active = false;
    GameEntity::update();
}

bool Projectile::isLauncherFriendly() const
{
    return wasLaunchedByFriend;
}

float Projectile::getCollisionDamage() const
{
    return collisionDamage;
}