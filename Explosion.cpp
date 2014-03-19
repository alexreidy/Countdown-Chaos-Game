//
//  Explosion.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 3/12/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Explosion.h"

Explosion::Explosion(sf::Vector2f position, float blastRadius)
{
    this->position = position;
    this->blastRadius = blastRadius;
}

void Explosion::update()
{
    float next = radius + 9;
    if (next <= blastRadius) radius = next;
    else radius = blastRadius;
}