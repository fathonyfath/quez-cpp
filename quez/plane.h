#pragma once
#include "game_engine.h"
#include "game_object.h"
#include "resource_manager.h"

class Plane : public GameObject {
   public:
    Plane(SpriteRenderer* renderer, Texture2D* sprite, glm::vec3 size) : GameObject(renderer, sprite, size), shieldActive(false), cooldown(0.0f), resetCooldown(1.5f), health(3) {
        collider.p = colliderPos;
        collider.r = 20.0f;
    };
    void update(GameEngine* engine) {
        if (cooldown > 0.0f) {
            shieldActive = true;
            cooldown -= engine->getDeltaReadOnly();
        } else {
            shieldActive = false;
        }

        if (currentTarget == NULL) {
            float rotation = angleBetweenTwoVector(position, mousePositionToWorld) + 90.0f;
            this->rotation = rotation;
        } else {
            float rotation = angleBetweenTwoVector(position, currentTarget->position) + 90.0f;
            this->rotation = rotation;
        }

        float distance = glm::distance(position, mousePositionToWorld);
        float moveRotation = angleBetweenTwoVector(position, mousePositionToWorld) + 90.0f;
        if (distance > 3.0f) {
            float slowFactor = distance - 3.0f;
            if (slowFactor > 50.0f) slowFactor = 50.0f;
            slowFactor = slowFactor / 50.0f;
            float xDir = std::sin(glm::radians(moveRotation)) * engine->getDeltaReadOnly() * speed * slowFactor;
            float yDir = std::cos(glm::radians(moveRotation)) * engine->getDeltaReadOnly() * speed * slowFactor;
            glm::vec2 newPos = glm::vec2(position.x - xDir, position.y + yDir);
            position = glm::vec3(newPos.x, newPos.y, 0.0f);
        }

        collider.p.x = position.x;
        collider.p.y = position.y;
    }
    glm::vec3 mousePositionToWorld;

    void setTarget(GameObject* target) {
        this->currentTarget = target;
    }

    void resetShiled() {
        cooldown = resetCooldown;
    }

    c2Circle collider;
    c2v colliderPos;

    bool shieldActive;
    float cooldown;

    int health;

    GameObject* currentTarget = NULL;

   private:
    float angleBetweenTwoVector(glm::vec3 a, glm::vec3 b) {
        float first = a.y - b.y;
        float second = a.x - b.x;
        return glm::degrees(std::atan2f(first, second));
    }

    float resetCooldown;

    Texture2D planeSprite;
    float speed = 500.0f;
};