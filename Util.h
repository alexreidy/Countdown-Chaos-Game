//
//  Util.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/23/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Util__
#define __Countdown_Chaos__Util__

#include <iostream>
#include <sstream>
#include "ResourcePath.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>

#define PI 3.1415926535898

typedef enum { LEFT, RIGHT, UP, DOWN } direction;

// Implementation is OS-specific.
std::string getPathTo(std::string resource);

bool isInRect(sf::Vector2f point, sf::Vector2f corner, sf::Vector2f oppositeCorner);

sf::Vector2f getUnitVector(sf::Vector2f vector);

// Returns a random value in the given range
float rin(int range);

// Returns the given value with a random sign (+/-)
float rsign(float value);

float toRadians(float degrees);
float toDegrees(float radians);

#endif /* defined(__Countdown_Chaos__Util__) */
