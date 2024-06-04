#pragma once
#include "Utility/Vector2.h"
#include <Engine/OGLGame.h>
#include <string>
#include "GameObjects/Paddle.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Enemy.h"

/**
 *  An OpenGL Game based on ASGE.
 */
class SpaceInvaders : public ASGE::OGLGame
{
 public:
  SpaceInvaders();
  ~SpaceInvaders() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void setupResolution();
    void collisionHandler();
    bool isInside(ASGE::Sprite* box_sprite1, ASGE::Sprite* box_sprite2);
    void levelOne(int type);
    void levelTwo(int type);
    void levelThree(int type);
    void reset();
  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;
    void makeGem(float x_loc, float y_loc);
    void shoot(int shot_type);
    bool enemyBoundsCheck();
    void raiseEnemies();
    bool areTheyGone();

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  bool in_menu = true;
  int menu_choice = 0;
    bool in_game = false;
    bool in_ending = false;
    bool won = false;
    int level_counter = 0;
    float cooldown_per_shot = 1.0f;
    bool key_controls[3] = { false };
    int score = 1000;
    Paddle* player = new Paddle;
    Ball* bullets[100];
    Ball* powerups[10];
    Enemy* aliens[150];
    std::string bullet_type_loc[5] ={""};
    int bullet_type_size[5][2]={0};
    float bullet_type_speed[5]= {1500};
    int player_bullet_type = 0;
    ASGE::Sprite* life_signs = nullptr;
    ASGE::Sprite* score_counter = nullptr;
    ASGE::Sprite* shield = nullptr;
    const float gravity = 50.0f;
};