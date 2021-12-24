#version 330 core
struct Vertex
{
	vec3 position;
};

layout (location = 0) in vec3 v;

uniform mat4 projection;
// uniform mat4 view;
// uniform mat4 model;

out vec4 position;

void main()
{
	position = vec4(v.x, v.y, v.z, 1.0); 
	gl_Position = projection*position;
}