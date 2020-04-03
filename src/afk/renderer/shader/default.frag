#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_specular_1;
uniform sampler2D texture_normal_1;
uniform sampler2D texture_height_1;

void main() {
    FragColor = texture(texture_diffuse_1, TexCoords);
}
