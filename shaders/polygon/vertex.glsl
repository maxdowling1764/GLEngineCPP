#version 330 core
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

layout(location = 0) in vec3 v;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;

out vec4 position;
out vec3 norm;
out vec2 uv;

void main()
{
	vec4 vertex_position = vec4(v, 1.0);
	position = vertex_position;
	mat4 pvm = projection * view * model;
	gl_Position = pvm * vertex_position;
	norm = (model*vec4(vn, 0)).xyz;
	uv = vt;
}