//
//  EnemySoldier.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/19/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__EnemySoldier__
#define __Countdown_Chaos__EnemySoldier__

#include <iostream>
#include "Mobile.h"
#include "Launcher.h"
//#include "Projectile.h"

class EnemySoldier : public Mobile, public Launcher {
public:
    EnemySoldier();
    EnemySoldier(sf::Sprite sprite);
    ~EnemySoldier();
    
    void attack(const GameEntity &opponent);
    
    void makeWayTo(float x, float y);
    
    void update();
    
};

#endif /* defined(__Countdown_Chaos__EnemySoldier__) */
