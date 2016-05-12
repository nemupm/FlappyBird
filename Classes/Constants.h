//
//  Constants.h
//  FlappyBird
//
//  Created by Noriyuki Matsuoka on 2016/05/12.
//
//

#ifndef Constants_h
#define Constants_h

static const float GRAVITY_ACCEL_WHEN_UP = -500.0f;
static const float GRAVITY_ACCEL_WHEN_DOWN = -1000.0f;
static const float JUMP_SPEED       = 250.0f;
static const float OBSTACLE_INIT_X  = 300;
static const float OBSTACLE_MIN_Y   = 200.0f;
static const float OBSTACLE_MAX_Y   = 400.0f;
static const float OBSTACLE_TIME_SPAN = 1.35f;
static const int   OBStACLE_LIMIT   = 5;
static const float SCROLL_SPEED_X   = 120.0f;

enum State{
    Ready, Playing, GameOver
};

#endif /* Constants_h */
