#pragma once
#include "Components/SpriteComponent.h"
#include "Utility/Vector2.h"
#include <Engine/Renderer.h>
#include <Engine/Sprite.h>
#include <string>

/**
 *  Objects used throughout the game.
 *  Provides a nice solid base class for objects in this game world.
 *  They currently support only sprite components, but this could easily
 *  be extended to include things like rigid bodies or input systems.
 *  @see SpriteComponent
 */
class GameObject
{
  /**
   *  Default constructor.
   */
public:
    GameObject();
    ~GameObject();
    ASGE::Sprite* getSprite();
    void updatelocation(float change_x, float change_y);
    bool initialiseSprite(ASGE::Renderer* renderer, std::string filename);

  /**
   *  Destructor. Frees dynamic memory.
   */

  /**
   *  Allocates and attaches a sprite component to the object.
   *  Part of this process will attempt to load a texture file.
   *  If this fails this function will return false and the memory
   *  allocated, freed.
   *  @param [in] renderer The renderer used to perform the allocations
   *  @param [in] texture_file_name The file path to the the texture to load
   *  @return true if the component is successfully added
   */

  /**
   *  Returns the sprite componenent.
   *  IT IS HIGHLY RECOMMENDED THAT YOU CHECK THE STATUS OF THE POINTER
   *  IS IS VALID FOR A COMPONENT TO BE NULL AS THEY ARE OPTIONAL!
   *  @return a pointer to the objects sprite component (if any)
   */
  SpriteComponent* spriteComponent();


private:
    ASGE::Sprite* sprite = nullptr;
    float location_x = 0.0f;
    float location_y = 0.0f;
  void free();
  SpriteComponent* sprite_component = nullptr;
};
