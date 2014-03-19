//
//  Structure.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/3/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Structure__
#define __Countdown_Chaos__Structure__

#include <iostream>
#include "GameEntity.h"

class Structure : public GameEntity {
public:
    Structure();
    Structure(sf::Sprite sprite);
    
    virtual ~Structure();
    
    virtual void build();
    
    virtual void update();
    
    virtual void attack(const GameEntity &entity);
    
};

#endif /* defined(__Countdown_Chaos__Structure__) */
