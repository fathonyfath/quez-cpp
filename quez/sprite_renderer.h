#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include "texture.h"
#include "vertex_format.h"

class SpriteRenderer {
   public:
    SpriteRenderer(Shader* shader);
    ~SpriteRenderer();
    void drawSprite(Texture2D* texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color);

   private:
    Shader* shader;
    GLuint VAO;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    void initRenderData();
};