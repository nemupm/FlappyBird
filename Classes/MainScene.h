#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "Obstacle.h"

class Character;
#include "Obstacle.h"
#include "Constants.h"

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void onEnter() override;
    void setupTouchHandling();
    void update(float dt) override;
    
    void triggerReady();
    void triggerPlaying();
    void triggerGameOver();

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
private:
    Character* character;
    cocos2d::Vector<Obstacle*> obstacles;
    cocos2d::Node* background;
    cocos2d::Vector<cocos2d::Node*> grounds;
    void createObstacle(float dt);
    
    State state;
};

#endif // __MainScene_SCENE_H__
