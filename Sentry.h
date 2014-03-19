//
//  Sentry.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/16/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Sentry__
#define __Countdown_Chaos__Sentry__

#include <iostream>
#include "Structure.h"
#include "Launcher.h"

class Sentry : public Structure, public Launcher {
public:
    Sentry();
    Sentry(sf::Sprite sprite);
    ~Sentry();
    
    virtual void attack(const GameEntity &opponent);
    
};

#endif /* defined(__Countdown_Chaos__Sentry__) */
