//
//  Mobile.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/24/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__Mobile__
#define __Countdown_Chaos__Mobile__

#include <iostream>
#include "GameEntity.h"

class Mobile : public GameEntity {
public:
    Mobile(sf::Sprite sprite);
    Mobile();
    
    virtual ~Mobile();
    
    void setSpeed(float speed);
    
    virtual void face(float x, float y);
    
    virtual void makeWayTo(float x, float y);
    
    void allowMoveOnUpdate(bool canMove);
    
    bool willBeInRadiusAfterUpdate(float radius, sf::Vector2f point);
    
    bool isMoving(direction d);
    
    virtual void attack(const GameEntity &entity);
    
    virtual void update();
    
    void move(); // With velocity
    
    bool isMoving() const;
    
    bool facesAngle = false;
    
    bool explodesAtDestination = false;
    
protected:

    unsigned int updatesSinceDirected = 0;
    sf::Vector2f destination;
    
    sf::Vector2f velocity;
    
private:
    float speed = 7;
    bool canMoveOnUpdate = true;
    
};

#endif /* defined(__Countdown_Chaos__Mobile__) */
