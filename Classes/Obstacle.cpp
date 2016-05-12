//
//  Obstacle.cpp
//  FlappyBird
//
//  Created by Noriyuki Matsuoka on 2016/05/12.
//
//

#include "Obstacle.h"
using namespace cocos2d;

#pragma mark - Public methods

bool Obstacle::init() {
    if (!Node::init()) {
        return false;
    }

    // load the character animation timeline
    this->timeline = CSLoader::createTimeline("Obstacle.csb");
    // retain the character animation timeline so it doesn't get deallocated
    this->timeline->retain();

    return true;
}

#pragma mark - Private methods

#pragma mark View lifecycle

void Obstacle::onEnter()
{
    Node::onEnter();
}

void Obstacle::onExit()
{
    Node::onExit();
}

void Obstacle::moveLeft(float distance)
{
    this->setPosition(this->getPosition() + Vec2(-distance, 0));
}

std::vector<Rect> Obstacle::getRects()
{
    std::vector<Rect> rects;
    
    auto pipeTop = this->getChildByName<Sprite*>("pipe_top");
    Rect pipeTopBoundingBox = pipeTop->getBoundingBox();
    pipeTopBoundingBox.origin += this->getPosition();
    rects.push_back(pipeTopBoundingBox);
//    Vec2 pipeTopAnchorPoint = pipeTop->getPosition() + this->getPosition();
//    Size pipeTopSize = pipeTop->getContentSize();
//    Vec2 pipeTopOriginPoint = Vec2(pipeTopAnchorPoint.x - pipeTopSize.width / 2, pipeTopAnchorPoint.y);
//    rects.push_back(Rect(pipeTopOriginPoint.x, pipeTopOriginPoint.y, pipeTopSize.width, pipeTopSize.height));
    
    auto pipeBottom = this->getChildByName<Sprite*>("pipe_bottom");
    Rect pipeBottomBoundingBox = pipeBottom->getBoundingBox();
    pipeBottomBoundingBox.origin += this->getPosition();
    rects.push_back(pipeBottomBoundingBox);
//    Vec2 pipeBottomAnchorPoint = pipeBottom->getPosition() + this->getPosition();
//    Size pipeBottomSize = pipeBottom->getContentSize();
//    Vec2 pipeBottomOriginPoint = Vec2(pipeBottomAnchorPoint.x - pipeBottomSize.width / 2, pipeBottomAnchorPoint.y - pipeBottomSize.height);
//    Rect rectBottom = Rect(pipeBottomOriginPoint.x, pipeBottomOriginPoint.y ,pipeBottomSize.width, pipeBottomSize.height);
//    rects.push_back(rectBottom);
    
    return rects;
}