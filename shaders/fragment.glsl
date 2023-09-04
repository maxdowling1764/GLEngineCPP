#version 330 core
in vec4 position;
// in vec2 uv;
// in vec3 norm;

// uniform mat4 projectionMatrix;
uniform vec3 cam_up;
uniform vec3 cam_forward;
uniform vec3 cam_pos;
vec3 spherePos = vec3(0.0f, 0.0f, 2.0f);
vec3 spherePos2 = vec3(1.0f, 0.0f, 1.0f);
vec3 lightDir = normalize(vec3(1.0f, -1.0f, 1.0f));
vec3 lightPos = vec3(-1.0f, 1.0f, 0.0f);
vec3 lightColor = vec3(0.01f,0.1f,0.1f);
float lightIntensity = 100.0f;

vec4 i_hat = vec4(0.0f, 1.0f, 0.0f, 0.0f);
vec4 j_hat = vec4(0.0f, 0.0f, 1.0f, 0.0f);
vec4 k_hat = vec4(0.0f, 0.0f, 0.0f, 1.0f);
uniform vec2 julia_pos;
uniform float time;

vec4 quat_mult(vec4 a, vec4 b)
{
    vec4 prod;
    prod.x = a.x*b.x - a.y*b.y - a.z*b.z - a.w*b.w;
    prod.y = a.x*b.y + a.y*b.x + a.z*b.w - a.w*b.z;
    prod.z = a.x*b.z - a.y*b.w + a.z*b.x + a.w*b.y;
    prod.w = a.x*b.w + a.y*b.z - a.z*b.y + a.w*b.x;
    return prod;
}

vec4 quat_mult_lr(vec4 quat, vec4 a)
{
    return quat_mult(quat_mult(a, quat), a);
}

vec4 quat_conj(vec4 quat)
{
    return -0.5*(quat + quat_mult_lr(quat, i_hat) + quat_mult_lr(quat, j_hat) + quat_mult_lr(quat, k_hat));
    
}

float c_modulo(vec2 a)
{
    return distance(vec2(0.0f), a);
}

vec2 c_conj(vec2 a)
{
    return vec2(a.x, -a.y);
}

vec2 c_mult(vec2 a, vec2 b)
{
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + b.x*a.y);
}

vec2 c_sqr(vec2 a)
{
    return c_mult(a, a);
}

float c_sqr_mod(vec2 a)
{
    return c_mult(c_conj(a), a).x;
}

vec2 c_mult_lr(vec2 a, vec2 b)
{
    return c_mult(b, c_mult(a, b));
}
float c_arg(vec2 a)
{
    return atan(a.y/a.x);
}

vec4 sqr_mod( in vec4 a ) // square a quaterion
{
    return vec4( a.x*a.x - a.y*a.y - a.z*a.z - a.w*a.w,
                 2.0*a.x*a.y,
                 2.0*a.x*a.z,
                 2.0*a.x*a.w );
}

vec2 map(vec2 z0, vec2 c, out float i)
{
    vec2 z = z0;

	for(i=0; i < 20; i++) {
        z = c_sqr(z) + c;
		if((z.x*z.x + z.y*z.y) > 4.0)
        {
            break;
        } 
	}
    i = i / 20.0f;
    return z;
}

float integrateRayDensity(vec3 origin, vec3 ray, float step)
{
    int stepcount = 0;
    float accum = 0.0f;
    float temp = 0.0f;
    for(vec3 t = origin; t.z < 10.0f; t += ray*step)
    {
        
        stepcount++;
        vec2 z = map(1.5*t.xy*(t.z-2.0f), julia_pos*0.5f*(t.z*2.0f - 4.0f), temp);
        accum += temp;
    }
    return accum / stepcount;
}

float marchRayDensity(vec3 origin, vec3 ray, out vec3 dest, float step)
{
    int stepcount = 0;
    float accum = 0.0f;
    float occlusion = 0.0f;
    float temp = 0.0f;
    for (vec3 t = origin; t.z < 10.0f; t +=ray* step)
    {
        stepcount++;
        vec2 z = map(1.5*t.xy*(t.z-2.0f), julia_pos, temp);
        accum += temp;
    }
    return accum / stepcount;
}



void main()
{
    // ray sphere at camera
    vec3 ray = normalize(position.x * cross(cam_forward, cam_up) + position.y * cam_up + cam_forward);
    //vec3 ray2;
    vec3 dest;
    float val = marchRayDensity(cam_pos, ray, dest, 0.01f);
    //ray2 = normalize(dest-lightPos);
    //float occlusion = integrateRayDensity(dest, ray2, 1.0f);
    vec3 fragLight = position.xyz * val * 8.5;
    //vec3 argColor = vec3(cos(arg), sin(arg), 0.5*(cos(arg) + sin(arg)));

    gl_FragColor = vec4(fragLight, 1.0f);
} 