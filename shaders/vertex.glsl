#version 330 core
struct Vertex
{
	vec3 position;
};

layout (location = 0) in vec3 p;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
// uniform mat4 view;
// uniform mat4 model;

out vec4 position;

void main()
{
	position = vec4(p.x, p.y, p.z, 1.0); 
	gl_Position = projection*position;
}