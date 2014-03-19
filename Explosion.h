//
//  Explosion.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 3/12/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Explosion__
#define __Countdown_Chaos__Explosion__

#include <iostream>
#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(sf::Vector2f position, float blastRadius);
    
    float blastRadius = 0, radius = 0;
    sf::Vector2f position;
    
    void update();

private:
};

#endif /* defined(__Countdown_Chaos__Explosion__) */
