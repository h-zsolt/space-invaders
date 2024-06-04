//
// Created by c2-horosnyi on 21/01/2020.
//

#include "Paddle.h"

Paddle::~Paddle()
{
  delete core;
  delete save;
}

ASGE::Sprite* Paddle::getSprite()
{
  return core->getSprite();
}

bool Paddle::init(ASGE::Renderer* renderer,
                  std::string filename,
                  float max_speed,
                  float width,
                  float height,
                  float loc_x,
                  float loc_y)
{
  if (!core->initialiseSprite(renderer, filename))
  {
    return false;
  }
  core->getSprite()->width(width);
  core->getSprite()->height(height);
  core->getSprite()->xPos(loc_x);
  position_x = loc_x;
  core->getSprite()->yPos(loc_y);
  max_velocity = max_speed;
  save->maxspeed=max_speed;
  save->location_x=loc_x;
  save->location_y=loc_y;
  save->height=height;
  save->width=width;
  shot_count=1;
  lives=3;
  return true;
}

void Paddle::update(bool left, bool right, float time)
{
    if(reportTimer()>0)
    {
        invulnerable_timer-=time;
    }
    if(reportCooldown()>0)
    {
        cooldown_timer-=time;
    }
  time_passed = time;
  if (left)
  {
    velocity -= time * save->maxspeed*3;
    if (velocity > 0)
    {
      velocity -= time * save->maxspeed*5;
    }
  }
  if (right)
  {
    velocity += time * save->maxspeed*3;
    if (velocity < 0)
    {
      velocity += time * save->maxspeed*5;
    }
  }
  if ((left && right) || (!left && !right))
  {
    if (velocity > 0)
    {
      velocity -= time * save->maxspeed*8;
      if (velocity < 0)
      {
        velocity = 0;
      }
    }
    if (velocity < 0)
    {
      velocity += time * save->maxspeed*8;
      if (velocity > 0)
      {
        velocity = 0;
      }
    }
  }
  if (velocity > max_velocity)
  {
    velocity = max_velocity;
  }
  if (velocity < -max_velocity)
  {
    velocity = -max_velocity;
  }
  position_x += velocity * time;
  boundaries();
  core->getSprite()->xPos(position_x);
}

void Paddle::boundaries()
{
  if (position_x > 1280-core->getSprite()->width())
  {
    position_x = 1280-core->getSprite()->width();
    velocity = 0;
  }
  if (position_x < 0)
  {
    position_x = 0;
    velocity = 0;
  }
}

float Paddle::reportX()
{
  return position_x;
}

float Paddle::reportMovement()
{
  return velocity * time_passed;
}

void Paddle::reset()
{
  state = 1;
  invulnerable_timer=2;
  core->getSprite()->width(save->width);
  core->getSprite()->height(save->height);
  core->getSprite()->xPos(save->location_x);
  position_x = save->location_x;
  core->getSprite()->yPos(save->location_y);
  velocity = 0.0f;
}

void Paddle::changeState(int goal_state)
{

}

float Paddle::reportTimer()
{
    return invulnerable_timer;
}

void Paddle::addTimer(float amount)
{
    invulnerable_timer+=amount;
}

float Paddle::reportCooldown()
{
    return cooldown_timer;
}

void Paddle::addCooldown(float amount)
{
    cooldown_timer+=amount;
}

int Paddle::reportShots()
{
    return shot_count;
}

void Paddle::changeShots(int amount)
{
    shot_count+=amount;
}

int Paddle::reportLives()
{
    return lives;
}

bool Paddle::changeLives(int change_by)
{
    lives+=change_by;
    if(lives>0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
