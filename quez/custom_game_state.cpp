#include "custom_game_state.h"

#include <cmath>

#include "game_engine.h"
#include "glm/gtx/compatibility.hpp"

#define TINYC2_IMPL
#include <iostream>

#include "tinyc2.h"

CustomGameState CustomGameState::_instance;

void CustomGameState::init(GameEngine* engine) {
    this->camera = new Camera(engine->getScreenWidth(), engine->getScreenHeight());

    Texture2D planeTexture = ResourceManager::getTexture("Player");
    Texture2D planeShadowTexture = ResourceManager::getTexture("PlayerShadow");
    Texture2D pesawatBulletTexture = ResourceManager::getTexture("Bullet");
    Texture2D backgroundTexture = ResourceManager::getTexture("Background");
    Texture2D enemyBoxTexture = ResourceManager::getTexture("EnemyBox");
    Texture2D enemyBoxShadowTexture = ResourceManager::getTexture("EnemyBoxShadow");
    Texture2D bulletImpactTexture = ResourceManager::getTexture("BulletImpact");
    Texture2D enemyBulletTexture = ResourceManager::getTexture("EnemyBullet");
    Texture2D bulletImpactEnemyTexture = ResourceManager::getTexture("EnemyBulletImpact");
    Texture2D targetTexture = ResourceManager::getTexture("Target");
    Texture2D shieldTexture = ResourceManager::getTexture("Shield");
    Texture2D health1Filled = ResourceManager::getTexture("HealthFilled");
    Texture2D health1Empty = ResourceManager::getTexture("HealthEmpty");

    Texture2D health2Filled = ResourceManager::getTexture("HealthFilled");
    Texture2D health2Empty = ResourceManager::getTexture("HealthEmpty");

    Texture2D health3Filled = ResourceManager::getTexture("HealthFilled");
    Texture2D health3Empty = ResourceManager::getTexture("HealthEmpty");

    this->planeTexture = new Texture2D(planeTexture);
    this->planeShadowTexture = new Texture2D(planeShadowTexture);
    this->pesawatBulletTexture = new Texture2D(pesawatBulletTexture);
    this->backgroundTexture = new Texture2D(backgroundTexture);
    this->enemyBoxTexture = new Texture2D(enemyBoxTexture);
    this->enemyBoxShadowTexture = new Texture2D(enemyBoxShadowTexture);
    this->bulletImpactTexture = new Texture2D(bulletImpactTexture);
    this->enemyBulletTexture = new Texture2D(enemyBulletTexture);
    this->bulletImpactEnemyTexture = new Texture2D(bulletImpactEnemyTexture);
    this->targetTexture = new Texture2D(targetTexture);
    this->shieldTexture = new Texture2D(shieldTexture);
    this->health1Filled = new Texture2D(health1Filled);
    this->health1Empty = new Texture2D(health1Empty);

    this->health2Filled = new Texture2D(health2Filled);
    this->health2Empty = new Texture2D(health2Empty);

    this->health3Filled = new Texture2D(health3Filled);
    this->health3Empty = new Texture2D(health3Empty);

    Shader shader = ResourceManager::getShader("Sprite");
    Shader* s = new Shader(shader);

    this->renderer = new SpriteRenderer(s);

    camera->updateShaderViewProjection(shader);

    firstState = true;

    this->background = new GameObject(this->renderer, this->backgroundTexture, glm::vec3(520, 620, 0));
    this->background->position = glm::vec3(0, 0, 0);

    // Initialize GameObjects
    this->planeGameObject = new Plane(this->renderer, this->planeTexture, glm::vec3(40, 40, 0));
    this->planeGameObject->position = glm::vec3(0, 0, 0);
    this->planeShadowObject = new GameObject(this->renderer, this->planeShadowTexture, glm::vec3(20, 20, 0));

    durationBetweenSpawn = 2.0f;
    maxWave = 50;
    currentWave = 1;
    currentDuration = durationBetweenSpawn;

    enemies.clear();

    // Shiled etc
    this->shieldObject = new GameObject(this->renderer, this->shieldTexture, glm::vec3(58, 58, 0));
    this->targetObject = new GameObject(this->renderer, this->targetTexture, glm::vec3(44, 43, 0));

    this->gameOverDelay = 3.0f;

    this->health1Object = new GameObject(this->renderer, this->health1Filled, glm::vec3(19, 18, 0));
    this->health2Object = new GameObject(this->renderer, this->health2Filled, glm::vec3(19, 18, 0));
    this->health3Object = new GameObject(this->renderer, this->health3Filled, glm::vec3(19, 18, 0));

    this->health3Object->position = glm::vec3(-260 + 19, 310 - 18, 0);
    this->health2Object->position = glm::vec3(-260 + 19 + 22, 310 - 18, 0);
    this->health1Object->position = glm::vec3(-260 + 19 + 22 + 22, 310 - 18, 0);
}

void CustomGameState::cleanup(GameEngine* engine) {
}

void CustomGameState::pause(GameEngine* engine) {
}

void CustomGameState::resume(GameEngine* engine) {
}

void CustomGameState::handleEvents(GameEngine* engine) {
    glm::vec3 mousePos = camera->screenToWorldSpace(engine->getMousePos());
    planeGameObject->mousePositionToWorld = mousePos;

    if (engine->leftMouseDown() && canShot && planeGameObject->health > 0) {
        shoot(planeGameObject->position, planeGameObject->rotation);
        engine->playBeam();
        canShot = false;
    }

    if (engine->rightMouseDown() && planeGameObject->health > 0) {
        getAndUpdateNearestEnemyPlane();
    }
}

void CustomGameState::update(GameEngine* engine) {
    if (firstState) {
        cooldown = 0.1f;
        firstState = false;
        return;
    }

    if (!canShot) {
        if (currentTime < cooldown) {
            currentTime += engine->getDeltaReadOnly();
        } else {
            currentTime = 0.0f;
            canShot = true;
        }
    }

	updateLevelWave(engine->getDeltaReadOnly());

    planeGameObject->update(engine);
    planeShadowObject->rotation = planeGameObject->rotation;
    planeShadowObject->position.x = planeGameObject->position.x - 20.0f;
    planeShadowObject->position.y = planeGameObject->position.y - 20.0f;

    // Enemy gameObject
    for (unsigned int i = 0; i < enemies.size(); ++i) {
        EnemyPlane* enemyGameObject = enemies[i];
        GameObject* enemyShadow = enemiesShadow[i];
        enemyGameObject->update(engine);
        enemyShadow->rotation = enemyGameObject->rotation;
        enemyShadow->position.x = enemyGameObject->position.x - 20.0f;
        enemyShadow->position.y = enemyGameObject->position.y - 20.0f;
    }

    // Bullet gameObject
    for (unsigned int i = 0; i < bullets.size(); ++i) {
        Bullet* bulletGameObject = bullets[i];
        if ((bulletGameObject->position.x > 400.0f || bulletGameObject->position.x < -400.0f) ||
            (bulletGameObject->position.y > 400.0f || bulletGameObject->position.y < -400.0f)) {
            bullets.erase(bullets.begin() + i);
        }
    }

    for (unsigned int i = 0; i < bullets.size(); ++i) {
        Bullet* bulletGameObject = bullets[i];
        bulletGameObject->update(engine);
    }

    for (unsigned int i = 0; i < bulletsToPlayer.size(); ++i) {
        Bullet* bulletGameObject = bulletsToPlayer[i];
        if ((bulletGameObject->position.x > 400.0f || bulletGameObject->position.x < -400.0f) ||
            (bulletGameObject->position.y > 400.0f || bulletGameObject->position.y < -400.0f)) {
            bulletsToPlayer.erase(bulletsToPlayer.begin() + i);
        }
    }

    for (unsigned int i = 0; i < bulletsToPlayer.size(); ++i) {
        Bullet* bulletGameObject = bulletsToPlayer[i];
        bulletGameObject->update(engine);
    }

    // Collision Check Bullet to EnemyPlane
    for (unsigned int i = 0; i < enemies.size(); ++i) {
        EnemyPlane* enemyGameObject = enemies[i];
        for (unsigned int j = 0; j < bullets.size(); ++j) {
            Bullet* bulletGameObject = bullets[j];
            int collisionCheck = c2CircletoCircle(enemyGameObject->collider, bulletGameObject->collider);
            if (collisionCheck) {
                BulletImpact* impact = new BulletImpact(this->renderer, this->bulletImpactTexture, glm::vec3(12, 23, 0));
                impact->position = bulletGameObject->position;
                impact->rotation = bulletGameObject->rotation;
                bulletImpacts.push_back(impact);

                bullets.erase(bullets.begin() + j);
                engine->playHit();
                enemyGameObject->health--;
                if (enemyGameObject->health <= 0) {
                    engine->playExplosion();
                    enemyGameObject->isDead = true;
                    enemyPlaneDestroyed(enemyGameObject->position, enemyGameObject->rotation);
                    enemies.erase(enemies.begin() + i);
                    enemiesShadow.erase(enemiesShadow.begin() + i);
                }
            }
        }
    }

    // Collision check between enemy to player
    for (unsigned int i = 0; i < enemies.size(); ++i) {
        EnemyPlane* enemyGameObject = enemies[i];
        int collisionCheck = c2CircletoCircle(planeGameObject->collider, enemyGameObject->collider);
        if (collisionCheck && !planeGameObject->shieldActive) {
            engine->playHit();
            planeGameObject->resetShiled();
            planeGameObject->health--;
            if (planeGameObject->health <= 0) {
                engine->playExplosion();
            }
        }
    }

    // Collision check between enemy bullet to player
    for (unsigned int i = 0; i < bulletsToPlayer.size(); ++i) {
        Bullet* bulletToPlayer = bulletsToPlayer[i];
        int collisionCheck = c2CircletoCircle(planeGameObject->collider, bulletToPlayer->collider);
        if (collisionCheck) {
            BulletImpact* impact = new BulletImpact(this->renderer, this->bulletImpactEnemyTexture, glm::vec3(17, 10, 0));
            impact->position = bulletToPlayer->position;
            impact->rotation = bulletToPlayer->rotation;
            bulletImpacts.push_back(impact);

            bulletsToPlayer.erase(bulletsToPlayer.begin() + i);
            if (!planeGameObject->shieldActive) {
                engine->playHit();
                planeGameObject->resetShiled();
                planeGameObject->health--;
                if (planeGameObject->health <= 0) {
                    engine->playExplosion();
                }
            }
        }
    }

    for (unsigned int i = 0; i < bulletImpacts.size(); ++i) {
        BulletImpact* bulletImpact = bulletImpacts[i];
        bulletImpact->update(engine);

        if (!bulletImpact->isActive) {
            bulletImpacts.erase(bulletImpacts.begin() + i);
        }
    }

    if (planeGameObject->currentTarget != NULL) {
        EnemyPlane* enemy = (EnemyPlane*)planeGameObject->currentTarget;
        if (enemy->isDead) {
            getAndUpdateNearestEnemyPlane();
        }
    }

    // Sync shield position
    shieldObject->position = planeGameObject->position;

    // Sync target position
    if (planeGameObject->currentTarget != NULL) {
        targetObject->position = planeGameObject->currentTarget->position;
    }

    if (planeGameObject->health <= 0) {
        gameOverDelay -= engine->getDeltaReadOnly();
    }
    if (gameOverDelay <= 0.0f) {

    }

    if (planeGameObject->health == 3) {
        health1Object->setSprite(health1Filled);
        health2Object->setSprite(health2Filled);
        health3Object->setSprite(health3Filled);
    } else if (planeGameObject->health == 2) {
        health1Object->setSprite(health1Empty);
        health2Object->setSprite(health2Filled);
        health3Object->setSprite(health3Filled);
    } else if (planeGameObject->health == 1) {
        health1Object->setSprite(health1Empty);
        health2Object->setSprite(health2Empty);
        health3Object->setSprite(health3Filled);
    } else if (planeGameObject->health == 0) {
        health1Object->setSprite(health1Empty);
        health2Object->setSprite(health2Empty);
        health3Object->setSprite(health3Empty);
    } else {
        health1Object->setSprite(health1Empty);
        health2Object->setSprite(health2Empty);
        health3Object->setSprite(health3Empty);
    }
}

void CustomGameState::draw(GameEngine* engine) {
    glViewport(0, 0, engine->getScreenWidth(), engine->getScreenHeight());
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->background->render();

    // Render all shadow
    if (planeGameObject->health > 0) {
        planeShadowObject->render();
    }

    for (unsigned int i = 0; i < enemiesShadow.size(); ++i) {
        GameObject* enemyShadow = enemiesShadow[i];
        enemyShadow->render();
    }

    for (unsigned int i = 0; i < bullets.size(); ++i) {
        Bullet* bulletGameObject = bullets[i];
        bulletGameObject->render();
    }

    for (unsigned int i = 0; i < bulletsToPlayer.size(); ++i) {
        Bullet* bulletToPlayer = bulletsToPlayer[i];
        bulletToPlayer->render();
    }

    for (unsigned int i = 0; i < enemies.size(); ++i) {
        EnemyPlane* enemyGameObject = enemies[i];
        enemyGameObject->render();
    }

    if (planeGameObject->health > 0) {
        planeGameObject->render();
    }

    for (unsigned int i = 0; i < bulletImpacts.size(); ++i) {
        BulletImpact* bulletImpact = bulletImpacts[i];
        bulletImpact->render();
    }

    if (planeGameObject->currentTarget != NULL) {
        targetObject->render();
    }

    if (planeGameObject->shieldActive && planeGameObject->health > 0) {
        shieldObject->render();
    }

    health1Object->render();
    health2Object->render();
    health3Object->render();

    glDisable(GL_BLEND);
}

void CustomGameState::getAndUpdateNearestEnemyPlane() {
    if (enemies.empty()) {
        planeGameObject->setTarget(NULL);
        return;
    }
    GameObject* nearest = enemies[0];
    for (unsigned int i = 0; i < enemies.size(); ++i) {
        EnemyPlane* enemyGameObject = enemies[i];
        float oldDistance = glm::distance(planeGameObject->position, nearest->position);
        float newDistance = glm::distance(planeGameObject->position, enemyGameObject->position);
        if (newDistance < oldDistance) {
            nearest = enemyGameObject;
        }
    }
    planeGameObject->setTarget(nearest);
}

void CustomGameState::updateLevelWave(float deltaTime) {
    if (enemies.empty()) {
        if (currentDuration > 0.0f) {
            currentDuration -= deltaTime;
        } else {
            currentDuration = durationBetweenSpawn;
            for (int i = 0; i < currentWave; ++i) {
                EnemyPlane* enemyPlane = new EnemyPlane(this->renderer, this->enemyBoxTexture, glm::vec3(20, 20, 0));
                GameObject* enemyShadow = new GameObject(this->renderer, this->enemyBoxShadowTexture, glm::vec3(10, 10, 0));
                enemies.push_back(enemyPlane);
                enemiesShadow.push_back(enemyShadow);
            }
            if (currentWave < maxWave) {
                currentWave++;
            }

            planeGameObject->resetShiled();
            getAndUpdateNearestEnemyPlane();
        }
    }
}

void CustomGameState::shoot(glm::vec3 position, float rotation) {
    Bullet* bullet = new Bullet(this->renderer, this->pesawatBulletTexture, glm::vec3(5, 9, 0), 2000.0f);
    bullet->position = position;
    bullet->rotation = rotation;
    bullets.push_back(bullet);
}

void CustomGameState::enemyPlaneDestroyed(glm::vec3 position, float rotation) {
    float dir1 = rotation;
    float dir2 = rotation + 90.0f;
    float dir3 = rotation + 180.0f;
    float dir4 = rotation + 270.0f;

    Bullet* dir1Bullet = new Bullet(this->renderer, this->enemyBulletTexture, glm::vec3(5, 9, 0), 200.0f);
    dir1Bullet->position = position;
    dir1Bullet->rotation = dir1;
    Bullet* dir2Bullet = new Bullet(this->renderer, this->enemyBulletTexture, glm::vec3(5, 9, 0), 200.0f);
    dir2Bullet->position = position;
    dir2Bullet->rotation = dir2;
    Bullet* dir3Bullet = new Bullet(this->renderer, this->enemyBulletTexture, glm::vec3(5, 9, 0), 200.0f);
    dir3Bullet->position = position;
    dir3Bullet->rotation = dir3;
    Bullet* dir4Bullet = new Bullet(this->renderer, this->enemyBulletTexture, glm::vec3(5, 9, 0), 200.0f);
    dir4Bullet->position = position;
    dir4Bullet->rotation = dir4;
    bulletsToPlayer.push_back(dir1Bullet);
    bulletsToPlayer.push_back(dir2Bullet);
    bulletsToPlayer.push_back(dir3Bullet);
    bulletsToPlayer.push_back(dir4Bullet);
}
