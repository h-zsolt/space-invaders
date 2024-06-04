//
// Created by c2-horosnyi on 21/01/2020.
//

#ifndef BREAKOUT_C2_HOROSNYI_MASTER_BALL_H
#define BREAKOUT_C2_HOROSNYI_MASTER_BALL_H
#include "../utility/pmvector.h"
#include "GameObject.h"

class Ball
{
 public:
  Ball() = default;
  ~Ball();
  bool init(ASGE::Renderer* renderer, std::string filename, float max_speed, float x, float y, float width, float height);
  ASGE::Sprite* getSprite();
  void update(float time);
  void increaseSpeed(float amount, float percentage);
  void changeXHeading(bool goal_state);
  void changeYHeading(bool goal_state);
  void setXHeading(bool);
  void setYHeading(bool);
  bool reportXHeading();
  bool reportYHeading();
  void changeState(int state_goal);
  int reportState();
  void changeAngle(float change_to);
  void changeXPosition(float target);
  void changeYPosition(float target);
  float reportX();
  float reportY();
  void reset();
  void setVisibility(bool state_goal);
  bool reportVisibility();
  int reportLives();
  void setLives(int set_to);

 private:
  GameObject* core = new GameObject;
  pmvector* math_helper = new pmvector;
  bool visibility = false;
  float position_x = 0.0f;
  float position_y = 0.0f;
  bool heading_x = false;
  bool heading_y = false;
  int state = 0;
  int lives = 1;
  float radius = 12.0f;
  float size = 25.0f;
  float velocity = 400;
  float max_velocity = 800;
  float angle = 0.0f;
};

#endif // BREAKOUT_C2_HOROSNYI_MASTER_BALL_H
