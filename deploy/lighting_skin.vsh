#version 410

uniform mat4 u_projection;
uniform mat4 u_modelView;
uniform mat4 u_joints[50];

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in ivec4 a_index;
layout(location = 4) in vec4 a_weight;

out vec3 v_vertex;
out vec2 v_texCoord;
out vec3 v_normal;

void main()
{
    v_texCoord = a_texCoord;
    
    // Compute weighted joint transform
    mat4 joint = u_joints[a_index.x] * a_weight.x + u_joints[a_index.y] * a_weight.y
        + u_joints[a_index.z] * a_weight.z + u_joints[a_index.w] * a_weight.w;
    
    // Compute vertex and normal in eye space
    v_vertex = (u_modelView * joint * vec4(a_vertex, 1.0)).xyz;
    v_normal = normalize((u_modelView * joint * vec4(a_normal, 0.0)).xyz);
    
    // Compute vertex in projection space
    gl_Position = u_projection * vec4(v_vertex, 1.0);
}
