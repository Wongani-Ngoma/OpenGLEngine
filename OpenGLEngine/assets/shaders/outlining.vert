#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform float outlining;

void main() {
    vec3 FragPos = vec3(model * vec4(aPos + aNormal * outlining, 1.0));
    gl_Position = projection * view * vec4(aPos, 1.0);

}