//
// Created by c2-horosnyi on 25/02/2020.
//

#ifndef ESDLB3_ENEMY_H
#define ESDLB3_ENEMY_H

#include "GameObject.h"
#include "math.h"
#include <Engine/DebugPrinter.h>


class Enemy
{
public:
    Enemy();
    ~Enemy();
    bool init(ASGE::Renderer* renderer, std::string filename, float max_speed, float x_loc, float y_loc, float width, float height, int hit_points);
    ASGE::Sprite* getSprite();
    void update(float time, float gravity);
    void increaseSpeed(float amount, float percentage);
    void changeXHeading();
    void changeYHeading();
    void setXHeading(bool);
    void setYHeading(bool);
    bool reportXHeading();
    bool reportYHeading();
    void changeState(int state_goal);
    int reportState();
    void changeXPosition(float target);
    void changeYPosition(float target);
    float reportX();
    float reportY();
    float reportOffset();
    void changeOffset(float change_by);
    void reset();
    bool reportVisibility();
    void setVisibility(bool goal);
    void changeLife(int change_by);
    int currentLife();

private:
    GameObject* core = new GameObject;
    float position_x = 0.0f;
    float position_y = 0.0f;
    float offset = 0.0f;
    bool heading_x = false;
    bool heading_y = false;
    bool visibility = false;
    int state = 0;
    int lives = 1;
    float sin_timer = 0.0f;
    float size = 25.0f;
    float max_velocity = 800;
};


#endif //ESDLB3_ENEMY_H
