#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
    vec4 pos = projection * view * vec4(aPos,1.0);
    // ive no clue what am doing...
    // this will ensure depth shall always be 1.0, hence skyboxshalll alawys be farthest from viewer
    //gl_Position = vec4(pos.x, pos.y, pos.w, pos.w); 
    gl_Position = pos; 
    // z-axis is flipped due to the coordinate sytem change above ????
}