//
//  GameEntity.h
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/23/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#ifndef __Countdown_Chaos__GameEntity__
#define __Countdown_Chaos__GameEntity__

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Util.h"
#include "Explosion.h"

typedef enum {
    ENERGY,
    SEEDS, FOOD, WATER,
    OXYGEN,
    METAL,
    RESOURCE_TYPE_COUNT
} resource;

class GameEntity {
public:
    GameEntity(sf::Sprite sprite);
    GameEntity(std::string spriteFileName);
    GameEntity();
    
    virtual ~GameEntity();
    
    void setSprite(sf::Sprite sprite);
    const sf::Sprite &getSprite() const;
    
    void flipSprite(direction d);
    
    void centerOrigin();
    void upperLeftOrigin();
    
    bool isSelected() const;
    void setSelected(bool selected);
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    void setPosition(float x, float y);
    void move(float xOffset, float yOffset);
    
    bool isInRadius(float radius, sf::Vector2f point);
    
    void rotate(float degrees);
    void setAngle(float angle);
    
    virtual void face(float x, float y);
    void lateralFace(float x, float y);
    
    void setCanBeDamaged(bool destructible);
    bool canBeDamaged() const;
    void setMaxHp(float maxhp);
    void changeHp(float change);
    void setHp(float hp);
    bool isAlive() const;
    void regenerate();
    float getPercentHp() const;
    
    void setFriendly(bool friendly);
    bool isFriendly() const;
    
    virtual void attack(const GameEntity &entity) = 0;
    
    virtual void makeWayTo(float x, float y);
    
    float getMaxAttackRange() const;
    void setMaxAttackRange(float range);
    
    void setCanAttack(bool canAttack);
    virtual bool isAbleToAttack();
    
    virtual void update() = 0;
    
    float getDamageDealtPerAttack() const;
    
    void useResourceArray(float *resources);
    
    void setRegenRate(float rate);
    
    float defaultResourceUtilizationRateFor[RESOURCE_TYPE_COUNT];
    float resourceCreationRateFor[RESOURCE_TYPE_COUNT];
    bool requires[RESOURCE_TYPE_COUNT];
    float reqAmountOf[RESOURCE_TYPE_COUNT]; // for initial construction, maybe regen
    
    virtual void onLacking(resource r);
    
    void setRequirement(resource r, float amount);
    
    float hp = 100;
    
    void setColor(sf::Color color);
    sf::Color getColor() const;
    
    void planToDieAfter(int updates);
    
    void setDamageDealtPerAttack(float damage);
    
    sf::Vector2f getSize() const;
    
    bool touching(const GameEntity &entity) const;
    
    void setActive(bool active);
    bool isActive() const;
    
    bool disabled = false;
    
    bool getCentered() const;
    
    void explode();
    
    bool explodes = false;
    float explosionBlastRadius = 0;
    
    virtual void onDeath();
    
    std::vector<Explosion*> *explosions = nullptr;
    
    unsigned int updatesToReachDestination = 0;
    
    int updatesBetweenAttacks = 10;
    
protected:
    int updatesSinceLastAttack = 0;
    
    
    float *resources;
    
    float maxhp = 100;
    float regenRate = 5;
    
    bool active = true; // Determines whether or not entity uses resources
    
    bool doomed = false; // <- Probably my favorite variable name ever.
    int lifespanUpdates = 1000;
    int updatesSinceBorn = 0;
    
    float maxAttackRange = 250;
    
private:
    bool selected = false;
    
    sf::Sprite sprite;
    
    bool destructible = true;
    bool friendly = true;
    
    float damageDealtPerAttack = 0.5;
    
    bool canAttack = false;
    bool regenerating = false;
    
    bool spriteIsCentered = false;
    
};
#endif /* defined(__Countdown_Chaos__GameEntity__) */
