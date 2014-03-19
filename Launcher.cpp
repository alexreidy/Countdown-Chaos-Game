//
//  Launcher.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 2/6/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Launcher.h"

void Launcher::initLaunchingSystem(std::vector<Projectile*> *projectiles, sf::Sprite projsprite)
{
    this->projectiles = projectiles; this->projsprite = projsprite;
    initialized = true;
}

void Launcher::launchProjectile(Projectile *projectile)
{
    projectiles->push_back(projectile);
}