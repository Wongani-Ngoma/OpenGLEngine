#version 460 core

#define MAX_POINT_LIGHT 20;
#define MAX_SPOT_LIGHT = 5;

struct Material {
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct PointLight {
    //attenuation
    float k0;
    float k1;
    float k2;

    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct DirLight {
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
    //attenuation
    float k0;
    float k1;
    float k2;

    vec3 position;
    vec3 direction;

    float outerCutOff;
    float cutOff;

    vec4 diffuse; 
    vec4 ambient;
    vec4 specular;
};


uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;
uniform Material material;
uniform float farPlane;

uniform PointLight pointLights[20];
uniform SpotLight spotLights[5];
uniform DirLight dirLight;

uniform vec3 viewPos;

uniform int noSpotLights;
uniform int noPointLights;
uniform int noTex;

uniform float near;
uniform float far;

uniform int renderShadows;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLight;

out vec4 FragColor;

// function definitions
float linearlizeDepth(float depth);
float logisticDepth(float depth, float steepness, float offset);
vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcPointLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);

vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
    
    //ambient
    vec4 ambient = dirLight.ambient * diffMap;

    //diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = dirLight.diffuse * (diff * diffMap);

    //specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128);
    vec4 specular = dirLight.specular * (spec * specMap);

    float shadow = 0.0f;
    if(renderShadows == 1) {
        vec3 lightCoords = FragPosLight.xyz / FragPosLight.w; // perspective division to get it to clip space
        if(lightCoords.z < 1.0) {// only calculate shadows if were inside view frustum of the ortho projection, ie behind far plane
            lightCoords = (lightCoords + 1.0) / 2; // transform from range [-1, 1] to [0, 1], so it is in the same range as depth map
            
            float currentDepth = lightCoords.z;
            
            // a bias to preventjaggerd shadow artefacts
            //float bias = 0.005f; // regular bias
            float bias = max(0.025f * (1.0f - dot(norm, lightDir)), 0.005f); // better solution to artefacts than regular bias, the this bias depends on the angle between the lightray and fragment normal 
            
            // NOW THE ACTUAL SHADOW TEST
            /* // regular shadow test, a frag is either in complete shadow or not, hence results in jaggared edges in low poly models
            float closestDepth = texture(shadowMap, lightCoords.xy).r;
            if(currentDepth > (closestDepth + bias) ) {
                shadow = 1.0;
            }
            */

            //better shadow test, samples fragments around it to get it's shadow value, resulting in smoother shadows
            int sampleRadius = 2;
            vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
            for(int y = -sampleRadius; y <= sampleRadius; y++) {
                for(int x = -sampleRadius; x <= sampleRadius; x++) {
                    float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                    
                    if(currentDepth > closestDepth + bias) {
                        shadow += 1.0;
                    }
                }
            }
            shadow /= pow((sampleRadius * 2 + 1), 2);

        }
    }
    return vec4(ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)));
}

vec4 calcPointLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {

    //ambient
    vec4 ambient = pointLights[idx].ambient * diffMap;

    //diffuse
    vec3 lightDir = normalize(pointLights[idx].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = pointLights[idx].diffuse * (diff * diffMap);

    //specular
    vec4 specular;
    if(diff != 0.0) {
        vec3 reflectDir = reflect(-lightDir, norm);

        vec3 halfWay = normalize(viewDir + lightDir);

        float spec = pow(max(dot(norm, halfWay), 0.0), material.shininess * 128);
        specular = pointLights[idx].specular * (spec * specMap);

    }
    // attenuation, loss of intensity due to distance
    float dist = length(pointLights[idx].position - FragPos);
    float attenuation = 1.0 / (pointLights[idx].k0 + pointLights[idx].k1 * dist + pointLights[idx].k2 * (dist * dist)); 


    float shadow = 0.0;
    if(renderShadows == 1) {
        vec3 fragToLight = FragPos - pointLights[idx].position;
        float currentDepth = length(fragToLight);
        float bias = max(0.025f * (1.0f - dot(norm, lightDir)), 0.5f);

        int sampleRadius = 2;
        float pixelSize = 1.0f / 1024.0f;

        for(int z = -sampleRadius; z <= sampleRadius; z++) {
            for(int y = -sampleRadius; y <= sampleRadius; y++) {
                for(int x = -sampleRadius; x <= sampleRadius; x++) {
                    float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * pixelSize).r;
                    closestDepth *= farPlane;
                    if( currentDepth > closestDepth + bias) {
                        shadow += 1.0f;
                    }    
                }
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 3);
    }

    return vec4(ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow))) * attenuation;
}

vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {

    vec3 lightDir = normalize(spotLights[idx].position - FragPos);

    float theta = dot(lightDir, normalize(-spotLights[idx].direction));
    float epsilon = spotLights[idx].cutOff - spotLights[idx].outerCutOff;
    float intensity = clamp((theta - spotLights[idx].outerCutOff) / epsilon, 0.0, 1.0);
    
    vec4 ambient = spotLights[idx].ambient * diffMap;

    if(theta > spotLights[idx].outerCutOff) { // > cause using cosines not degeres
        //if in cutOff
        //diffuse
        float diff = max(dot(norm, lightDir), 0.0f);
        vec4 diffuse = spotLights[idx].diffuse * (diff * diffMap);

        //specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
        vec4 specular = spotLights[idx].specular * (spec * specMap);

        diffuse *= intensity;
        specular *= intensity;

        float dist = length(spotLights[idx].  position - FragPos);
        float attenuation = 1.0 / (spotLights[idx].k0 + spotLights[idx].k1 * dist + spotLights[idx].k2 * (dist * dist)); 

        float shadow = 0.0f;
        if(renderShadows == 1) {
            vec3 lightCoords = FragPosLight.xyz / FragPosLight.w; // perspective division to get it to clip space
            if(lightCoords.z < 1.0) {// only calculate shadows if were inside view frustum of the ortho projection, ie behind far plane
                lightCoords = (lightCoords + 1.0) / 2; // transform from range [-1, 1] to [0, 1], so it is in the same range as depth map
                
                float currentDepth = lightCoords.z;
                
                // a bias to preventjaggerd shadow artefacts
                //float bias = 0.005f; // regular bias
                float bias = max(0.025f * (1.0f - dot(norm, lightDir)), 0.005f); // better solution to artefacts than regular bias, the this bias depends on the angle between the lightray and fragment normal 
                
                // NOW THE ACTUAL SHADOW TEST
                /* // regular shadow test, a frag is either in complete shadow or not, hence results in jaggared edges in low poly models
                float closestDepth = texture(shadowMap, lightCoords.xy).r;
                if(currentDepth > (closestDepth + bias) ) {
                    shadow = 1.0;
                }
                */

                //better shadow test, samples fragments around it to get it's shadow value, resulting in smoother shadows
                int sampleRadius = 2;
                vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
                for(int y = -sampleRadius; y <= sampleRadius; y++) {
                    for(int x = -sampleRadius; x <= sampleRadius; x++) {
                        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                        
                        if(currentDepth > closestDepth + bias) {
                            shadow += 1.0;
                        }
                    }
                }
                shadow /= pow((sampleRadius * 2 + 1), 2);

            }
        }

        return vec4(ambient + (diffuse * (1.0 - shadow)) + (specular) * (1.0 - shadow)) * attenuation;

    } else {
        //render just ambient
        return ambient;
    }
}

float logisticDepth(float depth, float steepness, float offset) {
    float zVal = linearlizeDepth(depth);
    return 1 / (1 + exp(-steepness * (zVal - offset)));
}

// depth is just the distance of a fragment from the near plane, this fuction normalizes it with respect to near and far plane
float linearlizeDepth(float depth) {
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 diffMap;
    vec4 specMap;

    if(noTex == 1) { 
        // use materials
        diffMap = material.diffuse;
        specMap = material.specular;
    } else {
        // use textures
        diffMap = texture(diffuse0, TexCoord);
        specMap = texture(specular0, TexCoord);
    }

    //placeholder
    vec4 result;

    //directional light
    result = calcDirLight(norm, viewDir, diffMap, specMap);

    // point lights
    for(int i = 0; i < noPointLights; i++) {
        result += calcPointLight(i, norm, viewDir, diffMap, specMap);
    }

    //spot lights
    for(int i = 0; i < noSpotLights; i++) {
        result += calcSpotLight(i, norm, viewDir, diffMap, specMap);
    }

    float depth = logisticDepth(gl_FragCoord.z, 0.5, 5.0);
    //FragColor = result * (1.0 - depth) + vec4(depth * vec3(0.85, 0.85,  0.90), 1.0); // creates a fog effect
    FragColor = result;
}
