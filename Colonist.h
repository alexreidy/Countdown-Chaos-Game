//
//  Colonist.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/27/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Colonist__
#define __Countdown_Chaos__Colonist__

#include <iostream>
#include "Projectile.h"
#include "Launcher.h"

class Colonist : public Mobile, public Launcher {
public:
    Colonist();
    Colonist(sf::Sprite sprite);
    ~Colonist();
    
    bool isAbleToAttack();
    void attack(const GameEntity &opponent);
    
    virtual void onLacking(resource r);
    
    void makeWayTo(float x, float y);
    
    void update();
    
};

#endif /* defined(__Countdown_Chaos__Colonist__) */
