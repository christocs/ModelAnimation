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


vec4 color_from_height(const float height)
{
    const float WATER_LEVEL = -2.0;
    const float GRASS_LEVEL = 0.0;
    const float SAND_LEVEL = -1.75;
    const float STONE_LEVEL = 2.0;

    const vec4 WATER_COLOR = vec4(79.0 / 255.0, 66.0 / 255.0, 181.0 / 255.0, 0.5);
    const vec4 GRASS_COLOR = vec4(96.0 / 255.0, 128.0 / 255.0, 56.0 / 255.0, 1.0);
    const vec4 SAND_COLOR = vec4(238.0 / 255.0, 214.0 / 255.0, 175.0 / 255.0, 1.0);
    const vec4 STONE_COLOR = vec4(136.0 / 255.0, 140.0 / 255.0, 141.0 / 255.0, 1.0);
    const vec4 SNOW_COLOR = vec4(255.0 / 255.0, 250.0 / 255.0, 250.0 / 255.0, 1.0);

    if (height < WATER_LEVEL) {
        return WATER_COLOR;
    } else if (height < SAND_LEVEL) {
        return SAND_COLOR;
    } else if (height < GRASS_LEVEL) {
        return GRASS_COLOR;
    } else if (height < STONE_LEVEL) {
        return STONE_COLOR;
    } else {
        return SNOW_COLOR;
    }

    return vec4(0.0, 0.0, 0.0, 1.0);
}

void main() {
    out_color = color_from_height(i.pos.y);
}
