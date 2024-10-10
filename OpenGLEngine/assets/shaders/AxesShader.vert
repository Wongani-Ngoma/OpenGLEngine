#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4x4 model;
uniform mat4x4 view;

out vec3 FragPos;
// a lil hack here, this is actually a texture coord, but the axes overlay dont need textures, so the tex coord just
// contain a color R, G or B
out vec3 color;

void main() {
    gl_Position =  view * model * vec4(aPos, 1.0);
    color = aNormal;
}