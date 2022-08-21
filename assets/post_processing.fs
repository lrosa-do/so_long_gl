#version 300 es
precision highp float;
in vec2 TexCoords;
out vec4 color;

uniform sampler2D scene;
uniform vec2  offsets[9];
uniform float     edge_kernel[9];
uniform float  blur_kernel[9];

uniform bool chaos;
uniform bool blur;
uniform bool shake;

void main()
{
   
    color = vec4(0.0f);

    vec3 samples[9];
    if(chaos || shake || blur)
    {
        for(int i = 0; i < 9; i++)
        {
          samples[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
        }
    }


    if(chaos)
    {           
        for(int i = 0; i < 9; i++)
        {
            color += vec4(samples[i] * edge_kernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    else if(blur)
    {
          for(int i = 0; i < 9; i++)
            color += vec4(samples[i] * blur_kernel[i], 0.0f);
        color.a = 1.0f;
    }
    else if(shake)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(samples[i] * blur_kernel[i], 0.0f);
        color.a = 1.0f;
    }
    else
    {
        color =  texture(scene, TexCoords);
    }

    
    
}
