#version 330 core

in vec4 position;
uniform sampler3D tex;
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

float smoothmax(float a, float b, float k)
{
    return log(exp(k * a) + exp(k * b)) / k;
}

vec2 grad_smoothmax(float a, float b)
{
    float d = exp(a) + exp(b);
    return vec2(exp(a) / d, exp(a) / d);
}

float sdf_sphere(vec3 c, float r, vec3 p)
{
    return length(p - c) - r;
}

float sdf(vec3 p)
{
    vec3 c1 = vec3(1.0f,2f,0.0f);
    vec3 c2 = vec3(2f, 0.0f, 0f);
    float r1 = 1f;
    float r2 = 1f;
    // f = -log(exp(-k*(sqrt((p-c1)*(p-c1))-r1)) + exp(-k*(sqrt((p-c2)*(p-c2))-r2)))/k;
    // f = -log(exp(-k*(sqrt((p-c1)*(p-c1))-r1)) + exp(-k*(sqrt((p-c2)*(p-c2))-r2)))/k
    // u = exp(-k*(sqrt((p-c1)*(p-c1))-r1)) + exp(-k*(sqrt((p-c2)*(p-c2))-r2)) = exp(-k*v1) + exp(-k*v2)
    // v_i = sqrt((p-c_i)*(p-c_i))-r_i
    // @x_j v_i = -
    // 
    // f = -log(u)/k
    // grad(f) = (-1/k)*(1/u)grad(u) = (-1/ku)grad(u)
    // grad(u)_i = @x_i(u)
    // u = exp(-k*(sqrt(sum(p_i - c_i)^2)
    return -smoothmax(-sdf_sphere(c1, r1, p), -sdf_sphere(c2, r2, p), 2f);
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
    i = i / 5.0f;
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

// Integrate texture over ray
vec4 mean_tex_ray(vec3 origin, vec3 ray, float step, float viewdist, sampler3D tex)
{
    vec4 res = vec4(0.0f);
    int n_steps = 1;
    for (vec3 s = origin; length(s-origin) < viewdist; s += ray * step)
    {
        res += texture(tex, s / viewdist);
        n_steps++;
    }
    return res / n_steps;
}

float marchRayDensity(vec3 origin, vec3 ray, out vec3 dest, float step)
{
    int stepcount = 0;
    float accum = 0.0f;
    float occlusion = 0.0f;
    float temp = 0.0f;
    for (vec3 t = origin; length(t-origin) < 50.0f; t +=ray* step)
    {
        stepcount++;
        vec2 z = map(1.5*t.xy*(t.z-2.0f), julia_pos, temp);
        accum += temp;
    }
    return accum / stepcount;
}

float sdfmarch(vec3 origin, vec3 ray, out vec3 dest, int maxiter, float epsilon)
{
    vec3 t = origin;
    float d;
    int i = 0;
    while ((d = sdf(t)) > epsilon && i < maxiter)
    {
        t += d * ray;
        i++;
    }
    dest = t;
    return i / maxiter;
}

// TODO: Viewdist and step are dual to each other, so it's not necessary to renormalize twice. maxiter -> 1, decrease step to compensate
vec4 mean_tex_ray_scattered(vec3 origin, vec3 ray, vec3 lightpos, float step, float viewdist, sampler3D tex)
{
    vec4 res = vec4(0.0f);
    int n_steps = 1;
    for (vec3 t = origin; length(t-origin) < viewdist; t += ray * step)
    {
        vec3 light_vec = lightpos - t;
        vec4 light_color = mean_tex_ray(t, normalize(light_vec), step*10.0f, viewdist/4.0f, tex); // Contribution to luminance from light source
        float luminance = 1.0f / (dot(light_vec, light_vec)/10.0f + 0.001f);
        vec4 scatter_color = texture(tex, vec3(1.0f, 1.0f, 2.0f)*(t / viewdist));
        //res += luminance * scatter_color;
        res += luminance * light_color * scatter_color + vec4(vec3(0.1f), 1.0f) * scatter_color;
        n_steps++;
    }
    return res / n_steps;
}

void main()
{
    float aspect = 1920.0f / 1080.0f;
    // ray sphere at camera
    vec3 ray = (position.x * aspect * cross(cam_forward, cam_up) + position.y * cam_up + cam_forward);
    //vec3 ray2;
    vec3 dest;
    vec4 val = mean_tex_ray_scattered(cam_pos, ray, vec3(5.0f*sin(100.0f * time)+10.0f, 5.0*cos(100.0f * time)+10.0f, 5.0f), 0.125f, 20, tex);
        //marchRayDensity(cam_pos, ray, dest, 0.025f);
        //sdfmarch(cam_pos, ray, dest, 100, 0.01f);
    //ray2 = normalize(dest-lightPos);
    //float occlusion = integrateRayDensity(dest, ray2, 1.0f);
    //vec3 fragLight = dest.xyz * (1.0f-val)*0.5f;
    //vec3 argColor = vec3(cos(arg), sin(arg), 0.5*(cos(arg) + sin(arg)));

    gl_FragColor = val;//vec4(fragLight, 1.0f);
} 