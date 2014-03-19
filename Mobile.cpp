//
//  Mobile.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/24/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "Mobile.h"

Mobile::Mobile(sf::Sprite sprite) : GameEntity(sprite) {}
Mobile::Mobile() : GameEntity() {}

Mobile::~Mobile() {}

void Mobile::setSpeed(float speed)
{
    this->speed = speed;
}

void Mobile::face(float x, float y)
{
    GameEntity::face(x, y);
}

void Mobile::makeWayTo(float x, float y) // This won't work properly if anti-collision AI is implemented
{
    destination = sf::Vector2f(x, y);
    
    if (facesAngle) face(x, y);
    
    float a = x - getPosition().x, b = y - getPosition().y;
    float distance = hypotf(a, b);
    velocity.x = a / distance * speed;
    velocity.y = b / distance * speed;
    
    updatesToReachDestination = distance / hypotf(velocity.x, velocity.y);
    updatesSinceDirected = 0;
}

bool Mobile::willBeInRadiusAfterUpdate(float radius, sf::Vector2f point)
{
    Mobile copyMob = *this;
    copyMob.update();
    return copyMob.isInRadius(radius, point);
}

bool Mobile::isMoving(direction d)
{
    switch (d) {
        case LEFT: return velocity.x < 0;
        case RIGHT: return velocity.x > 0;
        case UP: return velocity.y < 0;
        case DOWN: return velocity.y > 0;
    }
}

void Mobile::allowMoveOnUpdate(bool canMove)
{
    canMoveOnUpdate = canMove;
}

void Mobile::attack(const GameEntity &entity)
{
    if (isAbleToAttack()) {
        updatesSinceLastAttack = 0;
    } else {
        updatesSinceLastAttack++;
    }
}

void Mobile::move()
{
    GameEntity::move(velocity.x, velocity.y);
}

bool Mobile::isMoving() const
{
    return velocity != sf::Vector2f{};
}

void Mobile::update()
{
    GameEntity::update();
    
    if (updatesSinceDirected >= updatesToReachDestination) {
        velocity = sf::Vector2f{};
        if (explodesAtDestination)
            explode();
    } else if (!canMoveOnUpdate) {
        //makeWayTo(getPosition().x + 10, getPosition().y - 10);
    }
    
    if (canMoveOnUpdate) {
        move();
        updatesSinceDirected++;
    }
    
    // updatesSinceLastAttack++;
    
    canMoveOnUpdate = true;
}