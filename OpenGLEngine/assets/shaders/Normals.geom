#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA {
    vec3 Normal;
    vec2 TexCoord;
    mat4 projection;
} in_data[];

void main() {
    gl_Position = in_data[0].projection * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = in_data[0].projection * (gl_in[0].gl_Position + vec4(in_data[0].Normal, 0.0));
    EmitVertex();
    EndPrimitive();

    gl_Position = in_data[1].projection * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = in_data[1].projection * (gl_in[1].gl_Position +  vec4(in_data[1].Normal, 0.0));
    EmitVertex();
    EndPrimitive();

    gl_Position = in_data[2].projection * gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = in_data[2].projection * (gl_in[2].gl_Position + vec4(in_data[2].Normal, 0.0));
    EmitVertex();
    EndPrimitive();
}