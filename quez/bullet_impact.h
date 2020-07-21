#pragma once
#include "game_object.h"

class BulletImpact : public GameObject {
   public:
    BulletImpact(SpriteRenderer* renderer, Texture2D* sprite, glm::vec3 size) : GameObject(renderer, sprite, size), maxDuration(0.02f), duration(0.0f), isActive(true){};
    void update(GameEngine* engine) {
        if (duration < maxDuration) {
            duration += engine->getDeltaReadOnly();
        } else {
            isActive = false;
        }
    }
    bool isActive;
    float duration;
    float maxDuration;
};