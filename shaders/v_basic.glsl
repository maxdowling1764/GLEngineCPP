#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
out vec3 pos;
out vec3 norm;
out vec2 tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{ 
    gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
    norm = (model*vec4(aNorm, 0.0)).xyz;
    pos = (model*vec4(aPos, 1.0)).xyz;
    tex = aTex;
}