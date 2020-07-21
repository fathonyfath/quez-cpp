#pragma once

#include <vector>

#include "bullet.h"
#include "bullet_impact.h"
#include "camera.h"
#include "enemy_plane.h"
#include "game_object.h"
#include "plane.h"
#include "sprite_renderer.h"
#include "state.h"

class CustomGameState : public State {
   public:
    void init(GameEngine* engine);
    void cleanup(GameEngine* engine);

    void pause(GameEngine* engine);
    void resume(GameEngine* engine);

    void handleEvents(GameEngine* engine);
    void update(GameEngine* engine);
    void draw(GameEngine* engine);

    static CustomGameState* instance() {
        return &_instance;
    }

    void getAndUpdateNearestEnemyPlane();
	void updateLevelWave(float deltaTime);

   private:
    static CustomGameState _instance;
    glm::vec3 mousePosInWorldSpace;

    Camera* camera;
    SpriteRenderer* renderer;

    Texture2D* planeTexture;
    Texture2D* pesawatBulletTexture;
    Texture2D* enemyBoxTexture;
    Texture2D* enemyBulletTexture;
    Texture2D* backgroundTexture;

    Texture2D* planeShadowTexture;
    Texture2D* enemyBoxShadowTexture;

    Texture2D* bulletImpactTexture;
    Texture2D* bulletImpactEnemyTexture;

    Texture2D* shieldTexture;
    Texture2D* targetTexture;

    Texture2D* health1Filled;
    Texture2D* health1Empty;

    Texture2D* health2Filled;
    Texture2D* health2Empty;

    Texture2D* health3Filled;
    Texture2D* health3Empty;

    GameObject* background;

    Plane* planeGameObject;
    GameObject* planeShadowObject;
    std::vector<Bullet*> bullets;
    std::vector<EnemyPlane*> enemies;
    std::vector<GameObject*> enemiesShadow;
    std::vector<BulletImpact*> bulletImpacts;

    std::vector<Bullet*> bulletsToPlayer;

    GameObject* targetObject;
    GameObject* shieldObject;

    GameObject* health1Object;
    GameObject* health2Object;
    GameObject* health3Object;

    bool firstState = true;

    bool canShot = true;
    bool showShield = false;

    float cooldown = 0.1f;
    float currentTime = 0.0f;

    float gameOverDelay = 3.0f;

    float currentDuration;
    float durationBetweenSpawn;
    int currentWave;
    int maxWave;

	void shoot(glm::vec3 position, float rotation);
	void enemyPlaneDestroyed(glm::vec3 position, float rotation);
};