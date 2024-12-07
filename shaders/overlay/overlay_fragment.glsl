#version 330 core

in vec2 uv;
uniform sampler2D colorbuffer;
uniform sampler2D depthbuffer;
uniform sampler2D domaindepth_back;
uniform sampler2D domaindepth_front;
uniform sampler2D domainpos_front;
uniform sampler2D domainpos_back;
uniform sampler3D volumeData;
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

    float domainDepth = min(lindepth(texture2D(domaindepth_back, uv).r), lindepth(d)) - lindepth(texture2D(domaindepth_front, uv).r);

    const int RAY_STEP_COUNT = 500;
    if (abs(domainDepth)  > 0.001 && lindepth(texture2D(domaindepth_front, uv).r) < lindepth(d))
    {
        vec3 domainRay = normalize(texture2D(domainpos_back, uv).xyz - texture2D(domainpos_front, uv).xyz);
        vec3 step = (domainDepth / RAY_STEP_COUNT) * domainRay;
        int i = 0;
        vec3 accum = vec3(0.0);
        for (vec3 p = texture2D(domainpos_front, uv).xyz; i < RAY_STEP_COUNT; p += step)
        {
            accum += texture(volumeData, vec3(0.5)+vec3(0.35, 0.35, 0.7)*p).xyz;
            i++;
        }

    
        float blendFactor = (10.0 / (RAY_STEP_COUNT * RAY_STEP_COUNT)) * dot(accum, accum);
        blendFactor = smoothstep(0.0, 1.0, blendFactor);
        gl_FragColor = vec4(mix(texture2D(colorbuffer, uv).xyz, (1.0 / RAY_STEP_COUNT) * accum, blendFactor), 1.0);
    
        
        /*
        if (dot(accum, accum) == 0)
        {
            gl_FragColor = vec4(texture2D(colorbuffer, uv).xyz, 1.0);
        }
        else
        {
            gl_FragColor = vec4((1.0 / RAY_STEP_COUNT) * accum, 1.0);

        }
        */
        
        
    }
    else
    { 
        gl_FragColor = vec4(texture2D(colorbuffer, uv).xyz, 1.0);
    }
} 