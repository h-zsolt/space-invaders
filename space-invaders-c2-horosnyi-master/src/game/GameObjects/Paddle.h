//
// Created by c2-horosnyi on 21/01/2020.
//

#ifndef BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H
#define BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H

#include "GameObject.h"

class Paddle
{
 public:
  Paddle() = default;
  ~Paddle();
  bool init(ASGE::Renderer* renderer, std::string filename, float max_speed, float, float, float, float);
  ASGE::Sprite* getSprite();
  void update(bool left, bool right, float time);
  float reportX();
  float reportMovement();
  void reset();
  void changeState(int goal_state);
  float reportTimer();
  void addTimer(float amount);
  float reportCooldown();
  void addCooldown(float amount);
  int reportShots();
  void changeShots(int amount);
  int reportLives();
  bool changeLives(int change_by);

 private:
  GameObject* core = new GameObject;
  struct saved_data
  {
  public:
      float location_x = 0.0f;
      float location_y = 0.0f;
      float width = 0.0f;
      float height = 0.0f;
      float maxspeed = 0.0f;
  };
    saved_data* save = new saved_data;
  float invulnerable_timer = 3.0f;
  float cooldown_timer = 0.0f;
  float position_x = 0.0f;
  float position_y = 0.0f;
  float time_passed = 0.0f;
  int state = 1;
  int lives = 3;
  int shot_count = 1;
  float velocity = 0;
  float max_velocity = 50;
  void boundaries();
};

#endif // BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H
