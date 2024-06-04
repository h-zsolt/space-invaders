#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>
#include <GameObjects/GameObject.h>

#include "game.h"
/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */

enum
{
    SHIP_WIDTH=50,
    SHIP_HEIGHT=50,
    WINDOW_HEIGHT=720,
    WINDOW_WIDTH=1280
};

SpaceInvaders::SpaceInvaders()
{
  game_name = "ASGE Game";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
SpaceInvaders::~SpaceInvaders()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
  delete score_counter;
  delete life_signs;
  delete shield;
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool SpaceInvaders::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  //toggleFPS();

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &SpaceInvaders::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &SpaceInvaders::clickHandler, this);

    bullet_type_loc[0]="SpaceShooterRedux/PNG/Lasers/laserBlue07.png";
    bullet_type_loc[1]="SpaceShooterRedux/PNG/Lasers/laserBlue03.png";
    bullet_type_loc[2]="SpaceShooterRedux/PNG/Lasers/laserGreen10.png";
    bullet_type_loc[3]="SpaceShooterRedux/PNG/Lasers/laserGreen02.png";
    bullet_type_loc[4]="SpaceShooterRedux/PNG/Lasers/laserRed05.png";

    bullet_type_size[0][0]=9;
    bullet_type_size[0][1]=37;
    bullet_type_size[1][0]=9;
    bullet_type_size[1][1]=37;
    bullet_type_size[2][0]=13;
    bullet_type_size[2][1]=54;
    bullet_type_size[3][0]=13;
    bullet_type_size[3][1]=57;
    bullet_type_size[4][0]=14;
    bullet_type_size[4][1]=56;

    bullet_type_speed[0]=1500;
    bullet_type_speed[1]=1600;
    bullet_type_speed[2]=1800;
    bullet_type_speed[3]=2000;
    bullet_type_speed[4]=2400;

  for(int i = 0;i<150;i++)
  {
      if(i<10)
      {
          powerups[i]=new Ball;
      }
      if(i<100)
      {
          bullets[i]=new Ball;
          bullets[i]->init(renderer.get(),bullet_type_loc[0],1500,0,0,bullet_type_size[0][0],bullet_type_size[0][1]);
      }
      aliens[i]=new Enemy;
      aliens[i]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen2.png",200,10,10,40,40,1);
      aliens[i]->setVisibility(false);
  }
  if(!player->init(renderer.get(),"SpaceShooterRedux/PNG/playerShip2_orange.png",600,SHIP_WIDTH,SHIP_HEIGHT,(WINDOW_WIDTH-SHIP_WIDTH)/2.0f,WINDOW_HEIGHT-SHIP_HEIGHT-10.0f))
  {
      return false;
  }

    shield = renderer->createRawSprite();

    if (!shield->loadTexture("data/images/SpaceShooterRedux/PNG/Effects/shield2.png"))
    {
        return false;
    }
    shield->height(90);
    shield->width(90);
    shield->yPos(WINDOW_HEIGHT-SHIP_HEIGHT-30);
    return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void SpaceInvaders::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/
  // Scaling_and_MultiResolution_in_2D_Games.php

  // 720p is a pretty modest starting point, consider 1080p
  game_width = WINDOW_WIDTH;
  game_height = WINDOW_HEIGHT;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void SpaceInvaders::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }
  if(in_menu)
  {
      if(key->action==ASGE::KEYS::KEY_RELEASED)
      {
          if(key->key==ASGE::KEYS::KEY_A||key->key==ASGE::KEYS::KEY_LEFT)
          {
              menu_choice--;
              if(menu_choice<0)
              {
                  menu_choice=0;
              }
          }
          if(key->key==ASGE::KEYS::KEY_D||key->key==ASGE::KEYS::KEY_RIGHT)
          {
              menu_choice++;
              if(menu_choice>3)
              {
                  menu_choice=3;
              }
          }
          if(key->key==ASGE::KEYS::KEY_SPACE||key->key==ASGE::KEYS::KEY_ENTER)
          {
                in_menu=false;
                in_game=true;
                level_counter=1;
                levelOne(menu_choice);
          }
      }
  }
  if(in_game)
  {
      if(key->action==ASGE::KEYS::KEY_PRESSED)
      {
          if(key->key==ASGE::KEYS::KEY_A||key->key==ASGE::KEYS::KEY_LEFT)
          {
              key_controls[0]=true;
          }
          if(key->key==ASGE::KEYS::KEY_D||key->key==ASGE::KEYS::KEY_RIGHT)
          {
              key_controls[1]=true;
          }
          if(key->key==ASGE::KEYS::KEY_SPACE||key->key==ASGE::KEYS::KEY_ENTER)
          {
              key_controls[2]=true;
          }
      }
      if(key->action==ASGE::KEYS::KEY_RELEASED)
      {
          if(key->key==ASGE::KEYS::KEY_A||key->key==ASGE::KEYS::KEY_LEFT)
          {
              key_controls[0]=false;
          }
          if(key->key==ASGE::KEYS::KEY_D||key->key==ASGE::KEYS::KEY_RIGHT)
          {
              key_controls[1]=false;
          }
          if(key->key==ASGE::KEYS::KEY_SPACE||key->key==ASGE::KEYS::KEY_ENTER)
          {
              key_controls[2]=false;
          }
          if(key->key==ASGE::KEYS::KEY_1)
          {
              player->changeShots(2);
          }
          if(key->key==ASGE::KEYS::KEY_2)
          {
              player_bullet_type++;
              if(player_bullet_type>4)
              {
                  player_bullet_type=4;
              }
          }
          if(key->key==ASGE::KEYS::KEY_3)
          {
              cooldown_per_shot*=3.0f/4.0f;
          }
      }
  }
  if(in_ending)
  {
      if(key->action==ASGE::KEYS::KEY_PRESSED)
      {
            in_ending=false;
            in_menu=true;
            reset();
      }
  }

}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void SpaceInvaders::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvaders::update(const ASGE::GameTime& game_time)
{
  float dt_sec = game_time.delta.count() / 1000.0;
  // make sure you use delta time in any movement calculations!

  if(in_game)
  {
      player->update(key_controls[0],key_controls[1],dt_sec);
      if(key_controls[2] && player->reportCooldown()<=0)
      {
          player->addCooldown(cooldown_per_shot);
          shoot(player_bullet_type);
      }
      for(int i = 0 ; i < 150 ; i++)
      {
          if(i < 10 && powerups[i]->reportVisibility())
          {
              powerups[i]->update(dt_sec);
          }
          if(i < 100 && bullets[i]->reportVisibility())
          {
              bullets[i]->update(dt_sec);
              if(bullets[i]->reportY()<-74)
              {
                  bullets[i]->setVisibility(false);
              }
          }
          if(aliens[i]->reportVisibility())
          {
              aliens[i]->update(dt_sec, gravity);
          }
          collisionHandler();
      }
      if(enemyBoundsCheck())
      {
          for(int i=0;i<150;i++)
          {
              aliens[i]->changeXHeading();
              aliens[i]->changeYPosition(aliens[i]->reportY()+30);
          }
          ASGE::DebugPrinter{} << "Enemies hit a wall" << std::endl;
      }
      bool only_once = true;
      for(int i=0;i<150&&only_once;i++)
      {
          if (aliens[i]->reportVisibility() && aliens[i]->reportY() > 675)
          {
              if(menu_choice==1)
              {
                  for(int j = 0;j<150;j++)
                  {
                      aliens[j]->changeYHeading();
                  }
              }
              else
              {
                  raiseEnemies();
              }
              only_once=false;
          }
      }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvaders::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  if (in_menu)
  {
      renderer->renderText("Please choose gamemode:",(WINDOW_WIDTH/2.0f)-(23*8),(WINDOW_HEIGHT/2.0f)-50,1.2f,ASGE::COLOURS::RED);
      renderer->renderText(menu_choice==0 ?">Original":"Original",(WINDOW_WIDTH/2.0f)-250,(WINDOW_HEIGHT/2.0f)+50,1.0f,menu_choice==0 ?ASGE::COLOURS::GREENYELLOW : ASGE::COLOURS::GREEN);
      renderer->renderText(menu_choice==1 ?">Gravtity":"Gravtity",(WINDOW_WIDTH/2.0f)-120,(WINDOW_HEIGHT/2.0f)+50,1.0f,menu_choice==1 ?ASGE::COLOURS::GREENYELLOW : ASGE::COLOURS::GREEN);
      renderer->renderText(menu_choice==2 ?">Quadratic":"Quadratic",(WINDOW_WIDTH/2.0f),(WINDOW_HEIGHT/2.0f)+50,1.0f,menu_choice==2 ?ASGE::COLOURS::GREENYELLOW : ASGE::COLOURS::GREEN);
      renderer->renderText(menu_choice==3 ?">Sin":"Sin",(WINDOW_WIDTH/2.0f)+130,(WINDOW_HEIGHT/2.0f)+50,1.0f,menu_choice==3 ?ASGE::COLOURS::GREENYELLOW : ASGE::COLOURS::GREEN);
  }
  if(in_game)
  {
      renderer->renderText("Lives: "+std::to_string(player->reportLives()),10,30,1.0f,ASGE::COLOURS::WHITE);
      renderer->renderText("Score: "+std::to_string(score),1130,30,1.0f,ASGE::COLOURS::WHITE);
      renderer->renderSprite(*player->getSprite());
      for(int i = 0;i<150;i++)
      {
          if(i<10 && powerups[i]->reportVisibility())
          {
              renderer->renderSprite(*powerups[i]->getSprite());
          }
          if(i<100 && bullets[i]->reportVisibility())
          {
              renderer->renderSprite(*bullets[i]->getSprite());
          }
          if(aliens[i]->reportVisibility())
          {
              renderer->renderSprite(*aliens[i]->getSprite());
          }
      }
      if(player->reportTimer()>0.0f)
      {
          shield->xPos(player->reportX()-20);
          renderer->renderSprite(*shield);
      }
  }
  if(in_ending)
  {
      if(won)
      {
          renderer->renderText("Congratulations! You Won!",WINDOW_WIDTH/2-310,WINDOW_HEIGHT/2-20,2.0f,ASGE::COLOURS::WHITE);
      }
      else
      {
          renderer->renderText("All your base are belong to us!",WINDOW_WIDTH/2-340,WINDOW_HEIGHT/2-20,2.0f,ASGE::COLOURS::WHITE);
      }
      renderer->renderText("Your final score was "+std::to_string(score),WINDOW_WIDTH/2-270,WINDOW_HEIGHT/2+50,1.5f,ASGE::COLOURS::WHITE);
  }
}

void SpaceInvaders::shoot(int shot_type)
{
    float offset = bullet_type_size[shot_type][0]+4;
    int counter = player->reportShots();
    int middle = (counter / 2)+1;
    score-=25;
    for(int i=0 ; i<100 && counter > 0 ; i++)
    {
        if(!bullets[i]->reportVisibility())
        {
            bullets[i]->init(renderer.get(),bullet_type_loc[shot_type],bullet_type_speed[shot_type],WINDOW_HEIGHT-SHIP_HEIGHT-bullet_type_size[shot_type][1],player->reportX()+SHIP_WIDTH/2.0f+((middle-counter)*offset),bullet_type_size[shot_type][0],bullet_type_size[shot_type][1]);
            bullets[i]->setLives(shot_type+1);
            bullets[i]->setVisibility(true);
            counter--;
        }
    }
}

void SpaceInvaders::levelOne(int type)
{
    for(int i = 0; i<3;i++)
    {
        for(int j=0;j<6;j++)
        {
            aliens[i*6+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen2.png",200,800+j*60,i*50+20,40,40,1);
            aliens[i*6+j]->changeState(type);
            aliens[i*6+j]->setVisibility(true);
        }
    }
}

bool SpaceInvaders::enemyBoundsCheck()
{
    for(int i=0;i<150;i++)
    {
        if(aliens[i]->reportVisibility() && ((aliens[i]->reportX()<=0 && !aliens[i]->reportXHeading()) || (aliens[i]->reportX()>=1240 && aliens[i]->reportXHeading())))
        {
            return true;
        }
    }
    return false;
}

void SpaceInvaders::raiseEnemies()
{
    for(int i = 0; i<150;i++)
    {
        if(aliens[i]->reportVisibility())
        {
            aliens[i]->changeYPosition(aliens[i]->reportY()-30);
        }
    }
}

bool SpaceInvaders::areTheyGone()
{
    bool theres_one = false;
    for(int i = 0;i<150;i++)
    {
        if(aliens[i]->reportVisibility())
        {
            theres_one=true;
        }
    }
    return !theres_one;
}

void SpaceInvaders::collisionHandler()
{
    for(int i = 0;i<100;i++)
    {
        for(int j = 0; j <150;j++)
        {
            if(bullets[i]->reportVisibility() && aliens[j]->reportVisibility() && isInside(bullets[i]->getSprite(),aliens[j]->getSprite()))
            {
                if(aliens[j]->currentLife()>=bullets[i]->reportLives())
                {
                    score+=bullets[i]->reportLives()*100;
                    bullets[i]->setVisibility(false);
                    aliens[j]->changeLife(-bullets[i]->reportLives());
                }
                else
                {
                    score+=aliens[j]->currentLife()*100;
                    aliens[j]->setVisibility(false);
                    bullets[i]->setLives(bullets[i]->reportLives()-aliens[j]->currentLife());
                }
                if(areTheyGone())
                {
                    score+=1000;
                    if(level_counter==3)
                    {
                        won=true;
                        in_game=false;
                        in_ending=true;
                        //you won
                    }
                    else
                    {
                        for(int i = 0;i<100;i++)
                        {
                            bullets[i]->setVisibility(false);
                        }
                        level_counter++;
                        switch (level_counter)
                        {
                            case 2:
                                levelTwo(menu_choice);
                                break;
                            case 3:
                                levelThree(menu_choice);
                                break;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0;i<150;i++)
    {
        if(aliens[i]->reportVisibility() && player->reportTimer()<= 0.0f && isInside(player->getSprite(),aliens[i]->getSprite()))
        {
            player->changeLives(-1);
            if(player->reportLives()<=0)
            {
                won=false;
                in_game=false;
                in_ending=true;
                //you lost
            }
            player->addTimer(3.0f);
        }
    }
}

bool SpaceInvaders::isInside(ASGE::Sprite* box_sprite1, ASGE::Sprite* box_sprite2)
{
    bool top_x_inside = box_sprite1->xPos() > box_sprite2->xPos() && box_sprite1->xPos() < box_sprite2->xPos()+box_sprite2->width();
    bool top_y_inside = box_sprite1->yPos() > box_sprite2->yPos() && box_sprite1->yPos() < box_sprite2->yPos()+box_sprite2->height();
    bool bottom_x_inside = box_sprite1->xPos()+box_sprite1->width() > box_sprite2->xPos() && box_sprite1->xPos()+box_sprite1->width() < box_sprite2->xPos()+box_sprite2->width();
    bool bottom_y_inside = box_sprite1->yPos()+box_sprite1->height() > box_sprite2->yPos() && box_sprite1->yPos()+box_sprite1->height() < box_sprite2->yPos()+box_sprite2->height();

    if((top_x_inside || bottom_x_inside) && (top_y_inside || bottom_y_inside))
    {
        return true;
    }

    return false;
}

void SpaceInvaders::levelTwo(int type)
{
    for(int i = 0; i<3;i++)
    {
        for(int j=0;j<6;j++)
        {
            if(i==1)
            {
                aliens[i*6+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen2.png",200,800+j*60,i*50+20,40,40,1);
            }
            else
            {
                aliens[i*6+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen5.png",200,800+j*60,i*50+20,40,40,3);
            }

            aliens[i*6+j]->changeState(type);
            aliens[i*6+j]->setVisibility(true);
        }
    }
}

void SpaceInvaders::levelThree(int type)
{
    for(int i = 0; i<4;i++)
    {
        for(int j=0;j<8;j++)
        {
            switch(i)
            {
                case 0:
                    aliens[i*8+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyRed1.png",200,600+j*60,i*50+20,40,40,10);
                    break;
                case 1:
                    aliens[i*8+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyBlack3.png",200,600+j*60,i*50+20,40,40,8);
                    break;
                case 2:
                    aliens[i*8+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen4.png",200,600+j*60,i*50+20,40,40,5);
                    break;
                case 3:
                    aliens[i*8+j]->init(renderer.get(),"SpaceShooterRedux/PNG/Enemies/enemyGreen5.png",200,600+j*60,i*50+20,40,40,3);
                    break;
            }

            aliens[i*8+j]->changeState(type);
            aliens[i*8+j]->setVisibility(true);
        }
    }
}

void SpaceInvaders::reset()
{
    score=1000;
    player->init(renderer.get(),"SpaceShooterRedux/PNG/playerShip2_orange.png",600,SHIP_WIDTH,SHIP_HEIGHT,(WINDOW_WIDTH-SHIP_WIDTH)/2.0f,WINDOW_HEIGHT-SHIP_HEIGHT-10.0f);
    player_bullet_type=0;
    for(int i = 0; i<150;i++)
    {
        if(i<100)
        {
            bullets[i]->setVisibility(false);
        }
        aliens[i]->setVisibility(false);
    }
    key_controls[0]=false;
    key_controls[1]=false;
    key_controls[2]=false;
    cooldown_per_shot=1.0f;
}
