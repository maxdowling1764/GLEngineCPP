#version 330 core

in vec2 uv;
uniform sampler2D colorbuffer;
uniform sampler2D depthbuffer;
uniform sampler2D domaindepth_back;
uniform sampler2D domaindepth_front;
uniform float t;

float r = 0.002f;

vec4 apply_kernel(float kernel[9], vec2 p, float r, sampler2D frame)
{
    vec4 res = vec4(0);
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            res += kernel[(i + 1)*3 + j + 1] * texture2D(frame, p + r * vec2(i, j));
        }
    }

    return res;
}

vec4 desaturate(vec4 c, float s)
{
    float v = dot(vec3(1), c.xyz)/3.0;
    vec3 center = vec3(v);
    return vec4(center + s*(c.xyz-center), c.w);
}

float lindepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * 100 * 0.1) / (100.0 + 0.1 - z * (100 - 0.1));
}

void main()
{
    /*
    float s = 1.0 / 16.0;
    float kernel[9] = float[](s, 2*s, s,
                              2*s,  4*s, 2*s,
                              s, 2*s, s);
    vec4 color = texture2D(framebuffer, uv);

    int n = 20;
    vec4 color2 = vec4(0);
    if (length(color.xyz) < 1.4)
    {
        color2 = color;
    }
    else
    { 
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                color2 += (1.0 / 9.0) * apply_kernel(kernel, uv + r * vec2(i, j), r, framebuffer);
            }
        }
        color2 = desaturate(color2, 1.4 - length(color.xyz));
    }
    */

    float d = texture2D(depthbuffer, uv).r;

    float domainDepth = lindepth(texture2D(domaindepth_back, uv).r) - lindepth(texture2D(domaindepth_front, uv).r);

    if (abs(domainDepth)  > 0.001 && lindepth(texture2D(domaindepth_front, uv).r) == lindepth(d))
    {
        gl_FragColor = vec4(domainDepth, 0.0, 0.0, 1.0);
    }
    else
    { 
        gl_FragColor = vec4(texture2D(depthbuffer, uv).xyz, 1.0);
    }
    
//    gl_FragColor = vec4(texture2D(domaindepth_back, uv).r, 0, 0, 1.0);
} 