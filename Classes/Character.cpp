//
//  Character.cpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//

#include "Character.hpp"
#include "Constants.h"
using namespace cocos2d;

bool Character::init()
{
    if (!Node::init()) {
        return false;
    }
    
    this->timeline = CSLoader::createTimeline("Character.csb");
    this->timeline->retain();
    this->velocity = 0;
    
    return true;
}

void Character::onEnter(){
    Node::onEnter();
}

void Character::update(float dt){
    if (this->velocity > 0) {
        this->velocity += GRAVITY_ACCEL_WHEN_UP * dt;
    }else if(this->velocity > -300){
        this->velocity += GRAVITY_ACCEL_WHEN_DOWN * dt;
    }
    this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
}

void Character::jump()
{
    this->stopAllActions();
    this->velocity = JUMP_SPEED;
    this->runAction(this->timeline);
    this->timeline->play("fly", false);
}

Rect Character::getRect()
{
    auto bird = this->getChildByName<Sprite*>("bird");
    Vec2 birdAnchorPosition = bird->getPosition() + this->getPosition();
    Size birdSize = bird->getContentSize() * 0.8f;
    Vec2 originPoint = Vec2(birdAnchorPosition.x - birdSize.width / 2, birdAnchorPosition.y - birdSize.height / 2);
    return Rect(originPoint, birdSize);
}

void Character::setIsFlying(bool isFlying)
{
    this->isFlying = isFlying;
}

bool Character::getIsFlying(){
    return this->isFlying;
}

void Character::startFly()
{
    this->setIsFlying(true);
    this->scheduleUpdate();
    this->velocity = JUMP_SPEED;
}

void Character::stopFly()
{
    this->setIsFlying(false);
    this->unscheduleUpdate();
}