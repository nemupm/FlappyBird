#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "CharacterReader.hpp"
#include "Character.hpp"
#include "Obstacle.h"
#include "ObstacleReader.h"
#include "Constants.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("CharacterReader", (ObjectFactory::Instance) CharacterReader::getInstance);
    instance->registReaderObject("ObstacleReader", (ObjectFactory::Instance) ObstacleReader::getInstance);

    auto rootNode = CSLoader::createNode("MainScene.csb");

    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    this->background = rootNode->getChildByName<cocos2d::Sprite*>("back");
    this->character = this->background->getChildByName<Character*>("Character");
    this->character->setLocalZOrder(1);
    auto ground = this->background->getChildByName("ground");
    ground->setLocalZOrder(1);
    
    addChild(rootNode);

    return true;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->scheduleUpdate();
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle), OBSTACLE_TIME_SPAN);
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event){
        this->character->jump();
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::update(float dt)
{
    // Move Obstacles
    for (auto obstacle : this->obstacles) {
        obstacle->moveLeft(SCROLL_SPEED_X * dt);
    }
    
    // Collision detection between Obstacles and Character
    Rect characterRect = this->character->getRect();
    for (auto obstacle : this->obstacles){
        auto obstacleRects = obstacle->getRects();
        for (Rect obstacleRect : obstacleRects) {
            bool hit = characterRect.intersectsRect(obstacleRect);
            if(hit){
                CCLOG("hit");
                this->unscheduleAllCallbacks();
            }else{
                CCLOG("no hit");
            }
        }
    }
}

void MainScene::createObstacle(float dt)
{
    auto obstacle = dynamic_cast<Obstacle*>(CSLoader::createNode("Obstacle.csb"));
    this->obstacles.pushBack(obstacle);
    this->background->addChild(obstacle);
    float positionY = OBSTACLE_MIN_Y + CCRANDOM_0_1() * (OBSTACLE_MAX_Y - OBSTACLE_MIN_Y);
    obstacle->setPosition(OBSTACLE_INIT_X, positionY);
    
    if (this->obstacles.size() > OBStACLE_LIMIT) {
        this->obstacles.front()->removeFromParent();
        this->obstacles.erase(this->obstacles.begin());
    }
}
