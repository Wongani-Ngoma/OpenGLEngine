#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord; 

in DATA {
    vec3 Normal;
    vec2 TexCoord;
    mat4 projection;
} data_in[];

void main() {
    gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    Normal = data_in[0].Normal;
    TexCoord = data_in[0].TexCoord;
    EmitVertex();

    gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    Normal = data_in[1].Normal;
    TexCoord = data_in[1].TexCoord;
    EmitVertex();

    gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    Normal = data_in[2].Normal;
    TexCoord = data_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}

/*void main() {
    vec3 vector0 = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
    vec3 vector1 = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
    vec4 surfaceNormal = vec4(normalize(cross(vector0, vector1)), 0.0f);
    
    gl_Position = data_in[0].projection * (gl_in[0].gl_Position + surfaceNormal);
    Normal = data_in[0].Normal;
    TexCoord = data_in[0].TexCoord;
    EmitVertex();

    gl_Position = data_in[1].projection * (gl_in[1].gl_Position + surfaceNormal);
    Normal = data_in[1].Normal;
    TexCoord = data_in[1].TexCoord;
    EmitVertex();

    gl_Position = data_in[2].projection * (gl_in[2].gl_Position + surfaceNormal);
    Normal = data_in[2].Normal;
    TexCoord = data_in[2].TexCoord;
    EmitVertex();

    EndPrimitive();
}*/