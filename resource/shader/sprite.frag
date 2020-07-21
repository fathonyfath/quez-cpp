#version 330 core
in vec2 TexCoord;

// Texture samplers
uniform sampler2D ourTexture;
uniform vec3 spriteColor;

// Output
out vec4 FragColor;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}