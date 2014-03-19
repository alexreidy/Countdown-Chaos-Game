//
//  GameEntity.cpp
//  Countdown Chaos
//
//  Created by Alex Reidy on 1/23/14.
//  Copyright (c) 2014 Alex Reidy. All rights reserved.
//

#include "GameEntity.h"

GameEntity::GameEntity(sf::Sprite sprite)
{
    this->sprite = sprite;
    for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
        defaultResourceUtilizationRateFor[i] = 0;
        resourceCreationRateFor[i] = 0;
        requires[i] = false;
        reqAmountOf[i] = 0;
    }
}

GameEntity::GameEntity(std::string spriteFileName)
{
    sf::Texture t; t.loadFromFile(getPathTo(spriteFileName));
    sprite = sf::Sprite(t);
}

GameEntity::GameEntity() : GameEntity(sf::Sprite{}) {}

void GameEntity::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

GameEntity::~GameEntity()
{
    
}

bool GameEntity::isSelected() const
{
    return selected;
}

void GameEntity::setSelected(bool selected)
{
    this->selected = selected;
}

void GameEntity::setPosition(sf::Vector2f position)
{
    setPosition(position.x, position.y);
}

void GameEntity::move(float xOffset, float yOffset)
{
    sprite.move(xOffset, yOffset);
}

void GameEntity::makeWayTo(float x, float y) {}

sf::Vector2f GameEntity::getPosition() const
{
    return sprite.getPosition();
}

bool GameEntity::isInRadius(float radius, sf::Vector2f point)
{
    return hypot(point.x - getPosition().x, point.y - getPosition().y) <= radius;
}

void GameEntity::rotate(float degrees)
{
    sprite.rotate(degrees);
}

void GameEntity::setAngle(float angle)
{
    sprite.setRotation(angle);
    
}

void GameEntity::setSprite(sf::Sprite sprite)
{
    this->sprite = sprite;
}

const sf::Sprite &GameEntity::getSprite() const
{
    return sprite;
}

void GameEntity::flipSprite(direction d)
{
    if (d == LEFT) sprite.setScale(-1, 1);
    else sprite.setScale(1, 1);
}

void GameEntity::face(float x, float y)
{
    sf::Vector2f pos = getPosition();
    float angle = toDegrees(atan2f(pos.y - y, pos.x - x)) - 90;
    setAngle(angle);
}

void GameEntity::lateralFace(float x, float y)
{
    if (x > getPosition().x)
        flipSprite(RIGHT);
    if (x < getPosition().x)
        flipSprite(LEFT);
}

void GameEntity::centerOrigin()
{
    sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
    spriteIsCentered = true;
}

bool GameEntity::getCentered() const { return spriteIsCentered; }

void GameEntity::upperLeftOrigin()
{
    sprite.setOrigin(0, 0);
}

void GameEntity::setCanBeDamaged(bool destructible)
{
    this->destructible = destructible;
}

bool GameEntity::canBeDamaged() const
{
    return destructible;
}

void GameEntity::setMaxHp(float maxhp)
{
    this->maxhp = maxhp;
}

void GameEntity::changeHp(float change)
{
    if (change < 0) regenerating = false;
    hp += change;
    if (hp > maxhp) hp = maxhp;
    if (hp < 0) hp = 0;
}

void GameEntity::setHp(float hp)
{
    if (hp < 0) {
        this->hp = 0;
    } else if (hp > maxhp) {
        this->hp = maxhp;
    } else {
        this->hp = hp;
    }
}

float GameEntity::getPercentHp() const
{
    return hp / maxhp;
}

void GameEntity::setDamageDealtPerAttack(float damage)
{
    damageDealtPerAttack = damage;
}

bool GameEntity::isAlive() const
{
    return hp > 0;
}

void GameEntity::regenerate()
{
    regenerating = true;
}

void GameEntity::setFriendly(bool friendly)
{
    this->friendly = friendly;
}

bool GameEntity::isFriendly() const
{
    return friendly;
}

float GameEntity::getMaxAttackRange() const
{
    return maxAttackRange;
}

void GameEntity::setMaxAttackRange(float range)
{
    maxAttackRange = range;
}

void GameEntity::setCanAttack(bool canAttack)
{
    this->canAttack = canAttack;
}

bool GameEntity::isAbleToAttack()
{
    return canAttack && updatesSinceLastAttack >= updatesBetweenAttacks;
}

float GameEntity::getDamageDealtPerAttack() const
{
    return damageDealtPerAttack;
}

void GameEntity::setRegenRate(float rate)
{
    regenRate = rate;
}

void GameEntity::useResourceArray(float *resources)
{
    this->resources = resources;
}

void GameEntity::setRequirement(resource r, float amount)
{
    defaultResourceUtilizationRateFor[r] = amount;
    requires[r] = true;
}

void GameEntity::setColor(sf::Color color)
{
    sprite.setColor(color);
}

sf::Color GameEntity::getColor() const
{
    return sprite.getColor();
}

void GameEntity::onLacking(resource r) {}

void GameEntity::planToDieAfter(int updates)
{
    lifespanUpdates = updates;
    doomed = true;
}

void GameEntity::attack(const GameEntity &entity)
{
    if (active && isAbleToAttack()) {
        updatesSinceLastAttack = -1;
    }
    updatesSinceLastAttack++;
}

void GameEntity::setActive(bool active)
{
    this->active = active;
}

bool GameEntity::isActive() const { return active; }

sf::Vector2f GameEntity::getSize() const
{
    return (sf::Vector2f) sprite.getTexture()->getSize();
}

bool GameEntity::touching(const GameEntity &entity) const
{
    float x = getPosition().x, y = getPosition().y;
    
    if (x > entity.getPosition().x + entity.getSize().x)
        return false;
    if (y > entity.getPosition().y + entity.getSize().y)
        return false;
    
    if (x + getSize().x < entity.getPosition().x)
        return false;
    if (y + getSize().y < entity.getPosition().y)
        return false;
    
    return true;
}

void GameEntity::explode()
{
    if (explodes && explosions != nullptr) {
        explosions->push_back(new Explosion(getPosition(), explosionBlastRadius));
    }
}

void GameEntity::onDeath()
{
    explode(); // if possible
}

void GameEntity::update()
{
    if (regenerating) {
        changeHp(regenRate);
        if (hp == maxhp)
            regenerating = false;
    }
    
    if (doomed) {
        if (updatesSinceBorn > lifespanUpdates) {
            hp = 0;
        }
    }
    
    if (hp <= 0) onDeath();
    
    bool lackingRequirements = false;
    
    if (active) {
        for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
            float changed = resources[i] - defaultResourceUtilizationRateFor[i];

            if (requires[i]) {
                if (changed >= 0) { // >= because you're allowed to use up everything.
                    resources[i] = changed;
                } else {
                    lackingRequirements = true;
                    onLacking((resource) i);
                    active = false;
                }
            }
        }
        
        if (active && !lackingRequirements) {
            for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
                resources[i] += resourceCreationRateFor[i];
            }
        }
    } else {
        // Iterate through all resource requirements and
        // call onLacking(RESOURCE) ??? Not sure if this is always a good idea.
        // Maybe make a virtual onInactive() function to handle this. And call it here.
    }
    
    lackingRequirements = false;
    
    if (!active) {
        for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
            if (requires[i] && resources[i] < defaultResourceUtilizationRateFor[i]) {
                lackingRequirements = true;
            }
        }
        
        if (!lackingRequirements && !disabled)
            active = true;
    }
    
    updatesSinceBorn++;
}