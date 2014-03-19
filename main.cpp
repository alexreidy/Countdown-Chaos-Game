/* Countdown Chaos (TM)
** Copyright (C) 2014 Alex Reidy
*/

#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Structure.h"
#include "Projectile.h"
#include "Colonist.h"
#include "Util.h"
#include "Sentry.h"
#include "EnemySoldier.h"
#include "Explosion.h"

sf::RenderWindow window(sf::VideoMode(800, 700), "Countdown Chaos", sf::Style::Fullscreen);
float WIDTH = window.getSize().x, HEIGHT = window.getSize().y;

sf::Color terrainColor(240, 240, 240);

const sf::Vector2f INIT_TARGET_POS = sf::Vector2f{-5, -5};
sf::Vector2i selectionOrigin, selectionEndCorner;
sf::Vector2f destination = INIT_TARGET_POS;
sf::RectangleShape xSelectionLine, ySelectionLine;
const float SELECTION_LINE_THICKNESS = 1;
bool expectingMouseRelease = false;
bool rmbWasPressedLastCall = false;
sf::CircleShape target(5, 60);

sf::Font dejaVuSansMono;
sf::Text resourceMonitor;

sf::Text infoText;
const std::string DEFAULT_INFO = "Point at upper left corner to toggle guide";
int transientAlertLifespanUpdates = 250, updatesSinceAlertCalled = 0;
bool displayingAlertInfo = false;

bool itemGuideVisible = false;
const int ITEM_COUNT = 16;
sf::Text itemInfoText;
std::string itemInfo[ITEM_COUNT];

bool placingItem = false;
GameEntity *entityToBePlaced = NULL;

bool wasPressedLastCheck[256];

sf::RectangleShape hpBarGreen, hpBarRed;

sf::CircleShape explosion;

std::vector<GameEntity*> entities;
std::vector<Projectile*> projectiles;
std::vector<Explosion*> explosions;

float resources[RESOURCE_TYPE_COUNT];

int scoreCounter = 0, score = 0;

Structure *rocket = NULL;

sf::Vector2i getMousePos()
{
    return sf::Mouse::getPosition(window);
}

void alert(std::string str)
{
    infoText.setString(str);
    updatesSinceAlertCalled = 0;
    displayingAlertInfo = true;
}

void unselectPlayerUnits()
{
    for (int i = 0; i < entities.size(); i++) {
        if (entities.at(i)->isFriendly()) {
            entities.at(i)->setSelected(false);
        }
    }
}

bool isOnScreen(sf::Vector2f point)
{
    return point.x >= 0 && point.x <= WIDTH &&
    point.y >= 0 && point.y <= HEIGHT;
}

bool use(resource r, float amount)
{
    if (resources[r] >= amount) {
        resources[r] -= amount;
        return true;
    }
    return false;
}

void manageItemPlacement(GameEntity *item, sf::Keyboard::Key key)
{
    if (sf::Keyboard::isKeyPressed(key)) {
        if (!wasPressedLastCheck[key]) {
            if (entityToBePlaced != NULL) {
                delete entityToBePlaced;
            }
            entityToBePlaced = item;
        }
        wasPressedLastCheck[key] = true;
    } else {
        wasPressedLastCheck[key] = false;
    }
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// SPAWNING
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// Make sure these things are initialized
sf::Texture stx, atx, btx, ptx;
sf::Sprite soldierSprite, arpeggiSprite, bulletSprite, plasmaSprite;
Projectile bullet, plasma;

sf::Texture waterPumpTexture,
electrolyzerTexture, solarPanelTexture,
sentryTexture, miningFacilityTexture,
wallTexture, rocketTexture,
textureForUAV, missileTexture;

Structure *makeSolarPanel()
{
    sf::Sprite sprite(solarPanelTexture);
    Structure *s = new Structure(sprite);
    s->centerOrigin();
    s->setFriendly(true);
    s->setMaxHp(350);
    
    s->reqAmountOf[METAL] = 4;
    s->build();
    
    s->useResourceArray(resources);
    s->resourceCreationRateFor[ENERGY] = 10;
    
    s->planToDieAfter(1000);
    
    return s;
}

Structure *makeWaterPump()
{
    sf::Sprite wpspr(waterPumpTexture);
    
    Structure *wapu = new Structure(wpspr);
    wapu->useResourceArray(resources);
    wapu->setRequirement(ENERGY, 0.5);
    wapu->resourceCreationRateFor[WATER] = 10;
    wapu->centerOrigin();
    wapu->setFriendly(true);
    
    wapu->reqAmountOf[METAL] = 25;
    wapu->reqAmountOf[ENERGY] = 10000;
    wapu->build();
    
    return wapu;
}

Structure *makeElectrolyzer()
{
    sf::Sprite sprite(electrolyzerTexture);
    Structure *s = new Structure(sprite);
    s->centerOrigin();
    s->setFriendly(true);
    
    s->reqAmountOf[METAL] = 15;
    s->reqAmountOf[ENERGY] = 5000;
    s->build();
    
    s->useResourceArray(resources);
    s->setRequirement(ENERGY, 0.5);
    s->setRequirement(WATER, 0.5);
    
    s->resourceCreationRateFor[OXYGEN] = 15;
    
    s->explodes = true;
    s->explosionBlastRadius =200;
    s->explosions = &explosions;
    
    return s;
}

Sentry *makeSentry()
{
    sf::Sprite sprite(sentryTexture);
    Sentry *s = new Sentry(sprite);
    s->setFriendly(true);
    s->build();
    
    s->initLaunchingSystem(&projectiles, bulletSprite);
    
    s->useResourceArray(resources);
    
    s->explodes = true;
    s->explosionBlastRadius = 200;
    s->explosions = &explosions;
    
    return s;
}

Structure *makeMiningFacility()
{
    sf::Sprite sprite(miningFacilityTexture);
    sprite.setColor(sf::Color::Black);
    
    Structure *mfs = new Structure(sprite);
    mfs->centerOrigin();
    mfs->setFriendly(true);
    
    mfs->reqAmountOf[METAL] = 15;
    mfs->reqAmountOf[ENERGY] = 5000;
    mfs->build();
    
    mfs->useResourceArray(resources);
    mfs->setRequirement(ENERGY, 10);
    mfs->setRequirement(WATER, 0.5);
    mfs->setRequirement(OXYGEN, 0.3);
    
    mfs->resourceCreationRateFor[METAL] = 0.01;
    
    mfs->explodes = true;
    mfs->explosionBlastRadius = 150;
    mfs->explosions = &explosions;
    
    return mfs;
}

Structure *makeWall()
{
    sf::Sprite sprite(wallTexture);
    sprite.setColor(sf::Color(1,1,1));
    
    Structure *ws = new Structure(sprite);
    ws->centerOrigin();
    ws->setFriendly(true);
    ws->setRegenRate(2);
    ws->setMaxHp(3000);
    ws->build();
    
    ws->useResourceArray(resources);
    ws->reqAmountOf[METAL] = 8;
    
    return ws;
}

Structure *makeRocket()
{
    sf::Sprite sprite(rocketTexture);
    Structure *r = new Structure(sprite);
    r->centerOrigin();
    r->setPosition(WIDTH / 2, HEIGHT / 2);
    r->setFriendly(true);
    r->setMaxHp(10000);
    r->build();
    r->useResourceArray(resources);
    r->setRequirement(ENERGY, 50);
    r->setRequirement(WATER, 100);
    r->setActive(false);
    r->disabled = true;
    r->explodes = true;
    r->explosions = &explosions;
    r->explosionBlastRadius = 400;
    rocket = r;
    return r;
}

Mobile *makeUAV()
{
    sf::Sprite sprite(textureForUAV);
    Mobile *m = new Mobile(sprite);
    m->centerOrigin();
    m->facesAngle = true;
    m->setFriendly(true);
    m->useResourceArray(resources);
    m->setRequirement(ENERGY, 15);
    m->reqAmountOf[METAL] = 50;
    m->reqAmountOf[ENERGY] = 10000;
    m->explodes = true;
    m->explosions = &explosions;
    m->explosionBlastRadius = 250;
    m->setMaxAttackRange(500);
    m->setCanAttack(true);
    m->updatesBetweenAttacks = 30;
    m->setSpeed(15);
    return m;
}

Mobile *makeMissile()
{
    sf::Sprite sprite(missileTexture);
    Mobile *m = new Mobile(sprite);
    m->centerOrigin();
    m->facesAngle = true;
    m->setFriendly(true);
    m->setSpeed(30);
    m->explodes = true;
    m->explosionBlastRadius = 120;
    m->explosions = &explosions;
    m->useResourceArray(resources);
    m->explodesAtDestination = true;
    return m;
}

int countColonists()
{
    int count = 0;
    for (int i = 0; i < entities.size(); i++) {
        if (entities.at(i)->getSprite().getTexture() == soldierSprite.getTexture()) {
            count++;
        }
    }
    return count;
}

void loadAndInit()
{
    dejaVuSansMono.loadFromFile(getPathTo("Assets/DejaVuSansMono.ttf"));
    resourceMonitor.setFont(dejaVuSansMono);
    resourceMonitor.setCharacterSize(20);
    resourceMonitor.setColor(sf::Color::Black);
    resourceMonitor.setPosition(25, 15);
    
    target.setFillColor(terrainColor);
    target.setOutlineThickness(3);
    target.setOutlineColor(sf::Color::Blue);
    target.setOrigin(target.getRadius(), target.getRadius());
    
    xSelectionLine.setFillColor(sf::Color::Blue);
    ySelectionLine.setFillColor(sf::Color::Blue);
    ySelectionLine.setRotation(90);
    
    stx.loadFromFile(getPathTo("Assets/Soldier.png"));
    soldierSprite = sf::Sprite(stx);
    atx.loadFromFile(getPathTo("Assets/Arpeggi.png"));
    arpeggiSprite = sf::Sprite(atx);
    btx.loadFromFile(getPathTo("Assets/Bullet.png"));
    bulletSprite = sf::Sprite(btx);
    bulletSprite.setColor(sf::Color::Red);
    ptx.loadFromFile(getPathTo("Assets/Plasma.png"));
    plasmaSprite = sf::Sprite(ptx);
    plasmaSprite.setColor(sf::Color::Blue);
    
    solarPanelTexture.loadFromFile(getPathTo("Assets/SolarPanel.png"));
    waterPumpTexture.loadFromFile(getPathTo("Assets/WaterPump.png"));
    electrolyzerTexture.loadFromFile(getPathTo("Assets/Electrolyzer.png"));
    sentryTexture.loadFromFile(getPathTo("Assets/Sentry.png"));
    miningFacilityTexture.loadFromFile(getPathTo("Assets/MiningFacility.png"));
    wallTexture.create(30, 30);
    rocketTexture.loadFromFile(getPathTo("Assets/Rocket.png"));
    textureForUAV.loadFromFile(getPathTo("Assets/UAV.png"));
    missileTexture.loadFromFile(getPathTo("Assets/Missile.png"));
    
    hpBarGreen.setFillColor(sf::Color(60, 230, 60));
    hpBarRed.setFillColor(sf::Color(230, 60, 60));
    
    explosion.setFillColor(sf::Color(250, 100, 100, 150));
    
    infoText.setFont(dejaVuSansMono);
    infoText.setCharacterSize(20);
    infoText.setColor(sf::Color::Black);
    infoText.setPosition(25, 45);
    infoText.setString(DEFAULT_INFO);
    
    itemInfoText = infoText; // todo: remove? what's this for?
    
    itemInfo[0] = "Item              Key         Cost                  Resource(s) spent --> Product(s) (while active)";
    itemInfo[1] = "----              ---         ----                  ----------------------------------------------";
    itemInfo[2] = "Solar panels      S           2 METAL               THIN AIR --> ENERGY";
    itemInfo[3] = "Water Pump        P           25 METAL, 10K ENERGY  ENERGY --> WATER";
    itemInfo[4] = "Electrolyzer      E           15 METAL, 5K ENERGY   ENERGY, WATER --> OXYGEN";
    itemInfo[5] = "Mining Facility   M           15 METAL, 5k ENERGY   ENERGY, WATER, OXYGEN --> METAL";
    itemInfo[6] = "Sentry Turret     T           25 METAL, 5K ENERGY   ENERGY --> NOTHING";
    itemInfo[7] = "UAV               U           50 METAL, 10K ENERGY  ENERGY --> NOTHING";
    itemInfo[8] = "";
    itemInfo[9] = "Controls";
    itemInfo[10] = "Press D to enable or disable slected units";
    itemInfo[11] = "Press ESCAPE to cancel item placement";
    itemInfo[12] = "";
    itemInfo[13] = "Objective: Survive long enough to fuel your escape rocket. Launch preparation is quite costly, so you'll have ";
    itemInfo[14] = "to disable the rocket every now and then to allocate your resources elsewhere. Your rocket is";
    itemInfo[15] = "disabled by default, so make sure to power it up when you've got sufficient resources.";
    
    entities.push_back(makeRocket());
}

void rspawnsoldier(int n)
{
    sf::Vector2f pos = sf::Vector2f{rin(WIDTH), rin(HEIGHT)};
    
    for (int i = 0; i < n; i++) {
        EnemySoldier *es = new EnemySoldier(arpeggiSprite);
        es->setPosition(pos);
        es->initLaunchingSystem(&projectiles, plasmaSprite);
        es->useResourceArray(resources);
        
        entities.push_back(es);
    }
}

void initGame()
{
    for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
        resources[i] = 40000;
    }
    
    resources[METAL] = 60;
    
    for (int i = 0; i < 100; i++) {
        Colonist *c = new Colonist(soldierSprite);
        c->setPosition(WIDTH / 2, HEIGHT / 2);
        
        c->useResourceArray(resources);
        c->initLaunchingSystem(&projectiles, bulletSprite);
        
        entities.push_back(c);
    }
    
}

void manageInput()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        if (entityToBePlaced != NULL) {
            delete entityToBePlaced;
            entityToBePlaced = NULL;
        } else if (!wasPressedLastCheck[sf::Keyboard::Escape]) {
            window.close();
        }
        wasPressedLastCheck[sf::Keyboard::Escape] = true;
    } else {
        wasPressedLastCheck[sf::Keyboard::Escape] = false;
    }
    
    sf::Vector2i mpos = getMousePos();
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rmbWasPressedLastCall) {
        rmbWasPressedLastCall = true;
        if (isOnScreen((sf::Vector2f) mpos)) destination = sf::Vector2f(mpos.x, mpos.y);
        int selectedCount = 0;
        for (int i = 0; i < entities.size(); i++) {
            if (entities.at(i)->isSelected()) {
                selectedCount++;
                if (selectedCount == 1) {
                    entities.at(i)->makeWayTo(mpos.x, mpos.y);
                } else {
                    entities.at(i)->makeWayTo(
                        mpos.x - 0.5 * (125 + selectedCount) + rin(125 + selectedCount),
                        mpos.y - 0.5 * (125 + selectedCount) + rin(125 + selectedCount)
                    );
                }
            }
        }
    }
    
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        rmbWasPressedLastCall = false;
    }
    
    if (!expectingMouseRelease && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        unselectPlayerUnits();
        selectionOrigin = getMousePos();
        expectingMouseRelease = true;
    }
    
    // If left mouse button was released:
    if (expectingMouseRelease && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        selectionEndCorner = getMousePos();
        
        for (int i = 0; i < entities.size(); i++) {
            if (isInRect(entities.at(i)->getPosition(), (sf::Vector2f) selectionOrigin, (sf::Vector2f) selectionEndCorner)) {
                if (entities.at(i)->isFriendly()) entities.at(i)->setSelected(true);
            }
        }
        
        expectingMouseRelease = false;
    }
    
    if (mpos.x <= 10 && mpos.y <= 10) itemGuideVisible = true;
    else itemGuideVisible = false;
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && entityToBePlaced != NULL) {
        bool canplace = true;
        
        for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
            resource r = (resource) i;
            if (resources[r] < entityToBePlaced->reqAmountOf[r]) {
                canplace = false;
            }
        }
        
        if (canplace) {
            for (int i = 0; i < entities.size(); i++) {
                if (entityToBePlaced->touching(*entities.at(i))) {
                    canplace = false;
                    break;
                }
            }
        } else {
            alert("Sorry, you currently lack the resources to build that");
        }
        
        if (canplace) {
            for (int i = 0; i < RESOURCE_TYPE_COUNT; i++) {
                resources[(resource) i] -= entityToBePlaced->reqAmountOf[(resource) i];
            }
            entities.push_back(entityToBePlaced);
            entityToBePlaced = NULL;
        }
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (!wasPressedLastCheck[sf::Keyboard::D]) {
            for (int i = 0; i < entities.size(); i++) {
                GameEntity *entity = entities.at(i);
                if (entity->isSelected() && entity->isFriendly()) {
                    if (entity->isActive()) entity->setActive(false);
                    entity->disabled = !entity->disabled;
                }
            }
        }
        wasPressedLastCheck[sf::Keyboard::D] = true;
    } else {
        wasPressedLastCheck[sf::Keyboard::D] = false;
    }
    
    manageItemPlacement(makeSentry(), sf::Keyboard::T);
    manageItemPlacement(makeElectrolyzer(), sf::Keyboard::E);
    manageItemPlacement(makeMiningFacility(), sf::Keyboard::M);
    manageItemPlacement(makeWaterPump(), sf::Keyboard::P);
    manageItemPlacement(makeSolarPanel(), sf::Keyboard::S);
    manageItemPlacement(makeWall(), sf::Keyboard::W);
    manageItemPlacement(makeUAV(), sf::Keyboard::U);
    
}

float wave = -1;

void update()
{
    if (score == 100 && countColonists() > 0) {
        infoText.setString("YOU WON! NICELY DONE!");
    }
    
    sf::Vector2f mpos = (sf::Vector2f) getMousePos();
    
    if (rin(100) > 98) rspawnsoldier((int) wave);
    if (wave <= 100) wave += 0.0005;
    
    xSelectionLine.setPosition(selectionOrigin.x, selectionOrigin.y);
    xSelectionLine.setSize(
        sf::Vector2f{
            mpos.x - selectionOrigin.x,
            SELECTION_LINE_THICKNESS
        }
    );
    ySelectionLine.setPosition(selectionOrigin.x, selectionOrigin.y);
    ySelectionLine.setSize(
        sf::Vector2f{
            mpos.y - selectionOrigin.y,
            SELECTION_LINE_THICKNESS
        }
    );
    
    if (entityToBePlaced != NULL) {
        entityToBePlaced->setPosition(mpos);
    }
    
    for (int i = 0; i < entities.size(); i++) {
        GameEntity *entity = entities.at(i);
        
        entity->update();
        
        if (!entity->isAlive() || !isOnScreen(entity->getPosition())) {
            entities.at(i) = NULL;
            entities.erase(entities.begin() + i);
            delete entity;
            entity = NULL;
            continue;
        }
        
        for (int n = 0; n < entities.size(); n++) {
            if (entity->isInRadius(entity->getMaxAttackRange(), entities.at(n)->getPosition())) {
                if (entity->isFriendly() != entities.at(n)->isFriendly()) {
                    entity->attack(*entities.at(n));
                    
                    if (entity->getSprite().getTexture() == &textureForUAV && entity->isAbleToAttack()) {
                        Mobile *missile = makeMissile();
                        missile->setPosition(entity->getPosition());
                        missile->makeWayTo(entities.at(n)->getPosition().x, entities.at(n)->getPosition().y);
                        entities.push_back(missile);
                    }
                    
                    break;
                }
            }
        }
        
        for (int n = 0; n < entities.size(); n++) {
            if (!entity->isFriendly() && entities.at(n)->isFriendly()) {
                // If out of range, attack method moves
                // entity towards its target
                entity->attack(*entities.at(n));
                break;
            }
        }
        
        sf::Vector2f size = (sf::Vector2f) entity->getSprite().getTexture()->getSize();
        
        for (int p = 0; p < projectiles.size(); p++) {
            if (isInRect(projectiles.at(p)->getPosition(), sf::Vector2f{entity->getPosition().x - size.x / 2, entity->getPosition().y - size.y / 2}, sf::Vector2f{entity->getPosition().x + size.x / 2, entity->getPosition().y + size.y / 2})) {
                if (projectiles.at(p)->isLauncherFriendly() != entity->isFriendly()) {
                    entity->changeHp(-projectiles.at(p)->getCollisionDamage());
                    projectiles.at(p)->setHp(0);
                }
            }
        }
        
        for (int e = 0; e < explosions.size(); e++) {
            Explosion *exp = explosions.at(e);
            if (entity->isInRadius(exp->radius, exp->position)) {
                entity->changeHp(-exp->radius / exp->blastRadius * exp->blastRadius);
            }
        }
        
    }
    
    for (int i = 0; i < projectiles.size(); i++) {
        GameEntity *p = projectiles.at(i);
        if (!isOnScreen(p->getPosition()) || !p->isAlive()) {
            projectiles.erase(projectiles.begin() + i);
            delete p;
            continue;
        }
        projectiles.at(i)->update();
    }
    
    for (int i = 0; i < explosions.size(); i++) {
        if (explosions.at(i)->radius == explosions.at(i)->blastRadius) {
            delete explosions.at(i);
            explosions.erase(explosions.begin() + i);
            continue;
        }
        explosions.at(i)->update();
    }
    
    char resourcemonstr[1000];
    sprintf(resourcemonstr, "ENERGY = %d | FOOD = %d | WATER = %d | OXYGEN = %d | SEEDS = %d | METAL = %d | Percent fueled = %d",
        (int) resources[ENERGY],
        (int) resources[FOOD],
        (int) resources[WATER],
        (int) resources[OXYGEN],
        (int) resources[SEEDS],
        (int) resources[METAL],
        score
    );
    
    resourceMonitor.setString(resourcemonstr);
    
    if (displayingAlertInfo) {
        if (updatesSinceAlertCalled >= transientAlertLifespanUpdates) {
            infoText.setString(DEFAULT_INFO);
            displayingAlertInfo = false;
        } else {
            updatesSinceAlertCalled++;
        }
    }
    
    if (rocket->isActive()) {
        scoreCounter++;
    }
    if (scoreCounter == 100) {
        scoreCounter = 0;
        score++;
    }
    
}

void render()
{
    window.clear(terrainColor);
    
    if (destination != INIT_TARGET_POS) {
        target.setPosition(destination);
        window.draw(target);
    }
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        window.draw(xSelectionLine);
        xSelectionLine.setPosition(getMousePos().x - xSelectionLine.getSize().x, getMousePos().y - SELECTION_LINE_THICKNESS);
        window.draw(xSelectionLine);
        
        window.draw(ySelectionLine);
        ySelectionLine.setPosition(getMousePos().x, getMousePos().y);
        ySelectionLine.setRotation(-90);
        window.draw(ySelectionLine);
        ySelectionLine.setRotation(90);
        
    }
    
    for (int i = 0; i < projectiles.size(); i++) {
        window.draw(projectiles.at(i)->getSprite());
    }
    
    for (int i = 0; i < explosions.size(); i++) {
        explosion.setRadius(explosions.at(i)->radius);
        explosion.setPosition(explosions.at(i)->position.x - explosions.at(i)->radius, explosions.at(i)->position.y - explosions.at(i)->radius);
        window.draw(explosion);
    }
    
    for (int i = 0; i < entities.size(); i++) {
        GameEntity *entity = entities.at(i);
        
        if (1 == 1 || entity->isInRadius(300, (sf::Vector2f) getMousePos())) {
            float hpBarHeight = 0.1 * entity->getSize().y;
            
            hpBarGreen.setSize(sf::Vector2f{entity->getSize().x * entity->getPercentHp(), hpBarHeight});
            hpBarRed.setSize(sf::Vector2f{entity->getSize().x, hpBarHeight});
            
            sf::Vector2f pos;
            if (entity->getCentered()) {
                pos = sf::Vector2f{
                    entity->getPosition().x - entity->getSize().x / 2,
                    entity->getPosition().y - entity->getSize().y / 2 - hpBarHeight
                };
            } else {
                pos = entity->getPosition();
            }
            hpBarGreen.setPosition(pos);
            hpBarRed.setPosition(pos);
            
            window.draw(hpBarRed); window.draw(hpBarGreen);
        }
        
        window.draw(entities.at(i)->getSprite());
    }
    
    if (entityToBePlaced != NULL) {
        window.draw(entityToBePlaced->getSprite());
    }
    
    if (itemGuideVisible) {
        itemInfoText.setPosition(25, 20);
        for (int i = 0; i < ITEM_COUNT; i++) {
            itemInfoText.setString(itemInfo[i]);
            itemInfoText.move(0, 25);
            window.draw(itemInfoText);
        }
    } else {
        window.draw(infoText);
    }

    window.draw(resourceMonitor);
    
    window.display();
}

int main(int argc, char **argv)
{
    if (1==2 && time(NULL) >= 1395358403) {
        return -1;
    }
    
    window.setFramerateLimit(30);
    
    loadAndInit();
    initGame();
    
    while (window.isOpen()) {
        manageInput();
        update();
        render();
    }
    
    return EXIT_SUCCESS;
}
