//
//  Util.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/23/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Util.h"

std::string getPathTo(std::string resource)
{
    return resourcePath() + resource;
}

bool isInRect(sf::Vector2f point, sf::Vector2f corner, sf::Vector2f oppositeCorner)
{
    bool isInXBounds = false, isInYBounds = false;
    
    if (corner.x < oppositeCorner.x) {
        isInXBounds = point.x >= corner.x && point.x <= oppositeCorner.x;
    } else if (oppositeCorner.x < corner.x) {
        isInXBounds = point.x >= oppositeCorner.x && point.x <= corner.x;
    }
    
    if (corner.y < oppositeCorner.y) {
        isInYBounds = point.y >= corner.y && point.y <= oppositeCorner.y;
    } else if (oppositeCorner.y < corner.y) {
        isInYBounds = point.y >= oppositeCorner.y && point.y <= corner.y;
    }
    
    return isInXBounds && isInYBounds;
}

sf::Vector2f getUnitVector(sf::Vector2f vector)
{
    float norm = hypotf(vector.x, vector.y);
    return sf::Vector2f{vector.x / norm, vector.y / norm};
}

int randomInt(int range)
{
    std::random_device randev;
    std::uniform_int_distribution<float> dist(0, range);
    return dist(randev);
}

float rin(int range)
{
    return randomInt(range) + randomInt(100) / 100.0f - 1;
}

float rsign(float value)
{
    return rin(100) > 50 ? -value : value;
}

float toRadians(float degrees)
{
    return (degrees / 180) * PI;
}

float toDegrees(float radians)
{
    return (radians / PI) * 180;
}

