//
//  Structure.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/3/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Structure.h"

Structure::Structure()
{
    hp = 1;
    maxhp = 1000;
}

Structure::Structure(sf::Sprite sprite) : GameEntity(sprite)
{
    hp = 1;
    maxhp = 1000;
}

Structure::~Structure() {}

void Structure::build()
{
    regenerate();
}

void Structure::attack(const GameEntity &entity)
{
    
}

void Structure::update()
{
    GameEntity::update();
}