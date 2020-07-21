#pragma once
#include "game_engine.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "sprite_renderer.h"
#include "texture.h"
#include "tinyc2.h"

class GameObject {
   public:
    GameObject(SpriteRenderer* renderer, Texture2D* sprite, glm::vec3 size) : size(size),
                                                                              scale(1.0f, 1.0f, 1.0f),
                                                                              color(1.0f, 1.0f, 1.0f),
                                                                              rotation(0.0f) {
        this->_renderer = renderer;
        this->_sprite = sprite;
    };

    void setSprite(Texture2D* sprite);
    void update(GameEngine* engine);
    void render();
    glm::vec3 position, scale, size, color;
    float rotation;

   private:
    SpriteRenderer* _renderer;
    Texture2D* _sprite;
};