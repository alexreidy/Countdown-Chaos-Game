//
//  Projectile.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/27/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Projectile__
#define __Countdown_Chaos__Projectile__

#include <iostream>
#include "Mobile.h"

class Projectile : public Mobile {
public:
    Projectile(sf::Sprite sprite, const GameEntity *launcher, sf::Vector2f target, float speed);
    Projectile(sf::Sprite sprite);
    Projectile();
    
    ~Projectile();
    
    // void makeWayTo(float x, float y);
    
    void update();
    
    bool isLauncherFriendly() const;
    float getCollisionDamage() const;
    
private:
    float collisionDamage = 9;
    bool wasLaunchedByFriend = false;
};

#endif /* defined(__Countdown_Chaos__Projectile__) */
