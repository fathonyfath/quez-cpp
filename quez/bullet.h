#pragma once
#include "game_object.h"

class Bullet : public GameObject {
   public:
    Bullet(SpriteRenderer* renderer, Texture2D* sprite, glm::vec3 size, float speed) : GameObject(renderer, sprite, size), speed(speed) {
        collider.p = colliderPos;
        collider.r = 9.0f;
    };
    void update(GameEngine* engine) {
        float currentBulletXSpeed = std::sin(glm::radians(rotation)) * engine->getDeltaReadOnly() * speed;
        float currentBulletYSpeed = std::cos(glm::radians(rotation)) * engine->getDeltaReadOnly() * speed;
        glm::vec2 newPos = glm::vec2(position.x - currentBulletXSpeed, position.y + currentBulletYSpeed);
        position = glm::vec3(newPos.x, newPos.y, 0.0f);

        collider.p.x = position.x;
        collider.p.y = position.y;
    }
    float directionX;
    float directionY;

    float speed;

    c2Circle collider;
    c2v colliderPos;
};