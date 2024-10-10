/*#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 FragPosLight;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightProjection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main() {

    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >= MAX_BONES) {
            totalPosition = vec4(aPos, 1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
        
    }

    FragPos = vec3(model * vec4(aPos, 1.0f)); //We multiply by model so that the objects's lighting is calculated in the context of the world
    Normal = mat3(transpose(inverse(model))) * aNormal; //When the object transforms using model matrix, aNormal doesent change, so we use Normal to keep track of how the normal should actually be like when the object transforms
    TexCoord = aTexCoord;
    FragPosLight = lightProjection * vec4(FragPos, 1.0);

    //gl_Position =  projection * view * vec4(FragPos, 1.0);
    gl_Position =  projection * view * model * totalPosition;

}
*/
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec4 FragPosLight;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 lightProjection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main() {
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);

    for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >= MAX_BONES) {
            totalPosition = vec4(aPos, 1.0f);
            totalNormal = aNormal;
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition * weights[i];
        
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
        totalNormal += localNormal * weights[i];
    }

    if (totalPosition == vec4(0.0f)) {
        totalPosition = vec4(aPos, 1.0f);
        totalNormal = aNormal;
    }

    vec4 worldPosition = model * totalPosition;
    FragPos = worldPosition.xyz;
    Normal = normalize(mat3(transpose(inverse(model))) * totalNormal);
    TexCoord = aTexCoord;
    FragPosLight = lightProjection * worldPosition;
    gl_Position = projection * view * worldPosition;
}