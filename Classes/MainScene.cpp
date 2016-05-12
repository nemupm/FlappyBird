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
    auto ground1 = this->background->getChildByName("ground1");
    ground1->setLocalZOrder(1);
    auto ground2 = this->background->getChildByName("ground2");
    ground2->setLocalZOrder(1);
    grounds.pushBack(ground1);
    grounds.pushBack(ground2);
    
    addChild(rootNode);
    
    this->triggerReady();

    return true;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->scheduleUpdate();
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [&](Touch* touch, Event* event){
        switch (this->state) {
            case State::Ready:
                this->triggerPlaying();
            case State::Playing:
                this->character->jump();
                break;
            case State::GameOver:
                auto nextGameScene = MainScene::createScene();
                auto transition = TransitionFade::create(1.0, nextGameScene);
                Director::getInstance()->replaceScene(transition);
                break;
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::update(float dt)
{
    if (this->state != State::Playing) {
        return;
    }
    
    // Move Grounds
    for (auto ground : this->grounds) {
        ground->setPositionX(ground->getPositionX() - SCROLL_SPEED_X * dt);
    }
    // If right ground's positionX reach display's left side, left ground is to be set to right.
    if(this->grounds.back()->getPositionX() < 0)
    {
        auto ground = this->grounds.front();
        ground->setPositionX(this->grounds.back()->getPositionX() + 288.0f);
        this->grounds.erase(this->grounds.begin());
        this->grounds.pushBack(ground);
    }
    
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
                this->triggerGameOver();
            }else{
                CCLOG("no hit");
            }
        }
    }
    
    // Collision detection between Grounds and Character
    for (auto ground : this->grounds) {
        Rect groundRect = ground->getBoundingBox();
        Rect characterRect = this->character->getRect();
        bool hit = characterRect.intersectsRect(groundRect);
        if(hit){
            CCLOG("hit");
                this->triggerGameOver();
        }else{
            CCLOG("no hit");
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

void MainScene::triggerReady()
{
    this->state = State::Ready;
    this->character->stopFly();
}

void MainScene::triggerPlaying()
{
    this->state = State::Playing;
    this->character->startFly();
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle), OBSTACLE_TIME_SPAN);
}

void MainScene::triggerGameOver()
{
    this->state = State::GameOver;
    this->unschedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle));
}