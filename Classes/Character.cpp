//
//  Character.cpp
//  FlappyBird
//
//  Created by Yuki Kotani on 2016/05/11.
//
//

#include "Character.hpp"
using namespace cocos2d;

bool Character::init()
{
    if (!Node::init()) {
        return false;
    }
    
    this->timeline = CSLoader::createTimeline("Character.csb");
    this->timeline->retain();
    this->velocity = 0;
    this->accel = -500;
    
    return true;
}

void Character::onEnter(){
    Node::onEnter();
    this->scheduleUpdate();
}

void Character::update(float dt){
    if (this->velocity < 20 && this->velocity > -100) {
        this->velocity = -100;
    }else{
        this->velocity += accel * dt;
    }
    this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
}

void Character::jump()
{
    this->velocity = 200;
}