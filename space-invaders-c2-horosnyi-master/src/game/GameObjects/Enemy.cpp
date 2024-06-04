//
// Created by c2-horosnyi on 25/02/2020.
//

#include "Enemy.h"

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
    delete core;
}

bool Enemy::init(ASGE::Renderer *renderer, std::string filename, float max_speed, float x_loc, float y_loc, float width, float height, int hit_points)
{
    if (!core->initialiseSprite(renderer, filename))
    {
        return false;
    }
    core->getSprite()->width(width);
    core->getSprite()->height(height);
    core->getSprite()->xPos(x_loc);
    core->getSprite()->yPos(y_loc);
    position_x = x_loc;
    position_y = y_loc;
    max_velocity = max_speed;
    lives=hit_points;
    sin_timer=0.0f;
    offset=0.0f;
    heading_x=false;
    heading_y=false;
    return true;
}

ASGE::Sprite *Enemy::getSprite()
{
    return core->getSprite();
}

void Enemy::update(float time, float gravity)
{
    if(reportXHeading())
    {
        position_x+=max_velocity*time;
    }
    else
    {
        position_x-=max_velocity*time;
    }

    switch (state)
    {
        case 0:
            core->getSprite()->xPos(position_x);
            core->getSprite()->yPos(position_y);
            break;
        case 1:
            if(reportYHeading())
            {
                offset+=gravity*time;
                position_y+=offset*time;
            }
            else
            {
                offset-=gravity*time;
                position_y-=offset*time;
            }
            core->getSprite()->xPos(position_x);
            core->getSprite()->yPos(position_y);
            break;
        case 2:
            offset=0.0005f*position_x*(position_x-1240);
            core->getSprite()->xPos(position_x);
            core->getSprite()->yPos(position_y-offset);
            break;
        case 3:
            sin_timer+=time;
            offset = (50.0f * sin((2.0f*sin_timer)-(3.0f/2.0f * M_PI)))-50.0f;
            core->getSprite()->xPos(position_x);
            core->getSprite()->yPos(position_y-offset);
            break;
    }
}

void Enemy::increaseSpeed(float amount, float percentage)
{

}

void Enemy::changeXHeading()
{
    heading_x=!heading_x;
}

void Enemy::changeYHeading()
{
    heading_y=!heading_y;
}

void Enemy::setXHeading(bool set_to)
{
    heading_x=set_to;
}

void Enemy::setYHeading(bool set_to)
{
    heading_y=set_to;
}

bool Enemy::reportXHeading()
{
    return heading_x;
}

bool Enemy::reportYHeading()
{
    return heading_y;
}

void Enemy::changeState(int state_goal)
{
    state=state_goal;
}

int Enemy::reportState()
{
    return state;
}
void Enemy::changeXPosition(float target)
{
    position_x=target;
}

void Enemy::changeYPosition(float target)
{
    position_y=target;
}

float Enemy::reportX()
{
    return position_x;
}

float Enemy::reportY()
{
    return position_y;
}

float Enemy::reportOffset()
{
    return offset;
}

void Enemy::reset()
{

}

bool Enemy::reportVisibility()
{
    return visibility;
}

void Enemy::setVisibility(bool goal)
{
    visibility=goal;
}

void Enemy::changeOffset(float change_by)
{
    offset+=change_by;
}

void Enemy::changeLife(int change_by)
{
    lives+=change_by;
    if(lives<=0)
    {
        visibility=false;
    }
    ASGE::DebugPrinter{} << "My lives are now "<< lives << std::endl;
}

int Enemy::currentLife()
{
    return lives;
}
