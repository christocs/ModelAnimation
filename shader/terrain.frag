#version 410 core

uniform struct Textures {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
} u_textures;

in VertexData {
    vec2 uvs;
    vec3 pos;
} i;

out vec4 out_color;


vec3 color_from_height(const float height)
{
    const float WATER_LEVEL = -2.0f;
    const float GRASS_LEVEL = 0.0f;
    const float STONE_LEVEL = 2.0f;

    const vec3 WATER_COLOR = vec3(79.0 / 255.0, 66.0 / 255.0, 181.0 / 255.0);
    const vec3 GRASS_COLOR = vec3(96.0 / 255.0, 128.0 / 255.0, 56.0 / 255.0);
    const vec3 STONE_COLOR = vec3(136.0 / 255.0, 140.0 / 255.0, 141.0 / 255.0);
    const vec3 SNOW_COLOR = vec3(255.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0);

    if (height < WATER_LEVEL) {
        return WATER_COLOR;
    } else if (height < GRASS_LEVEL) {
        return GRASS_COLOR;
    } else if (height < STONE_LEVEL) {
        return STONE_COLOR;
    } else {
        return SNOW_COLOR;
    }

    return vec3(0.0, 0.0, 0.0);
}

void main() {
    out_color = vec4(color_from_height(i.pos.y), 1.0);
}
