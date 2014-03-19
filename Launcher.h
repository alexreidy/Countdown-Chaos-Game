//
//  Launcher.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/6/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Launcher__
#define __Countdown_Chaos__Launcher__

#include <iostream>
#include "Projectile.h"

class Launcher {
public:
    void initLaunchingSystem(std::vector<Projectile*> *projectiles, sf::Sprite projsprite);
    void launchProjectile(Projectile *projectile);
    
protected:
    sf::Sprite projsprite;
    
private:
    std::vector<Projectile*> *projectiles;
    bool initialized = false;
};

#endif /* defined(__Countdown_Chaos__Launcher__) */
