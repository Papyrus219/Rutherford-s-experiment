#version 330 core
out vec4 FragColor;

in vec2 tex_coord;

uniform vec4 color;
uniform bool is_glowing;

void main()
{
    float ambient = 0.0;
    if(is_glowing)
    {
        ambient = 1.0;
    }
    else
    {
        ambient = 1.0;
    }


    FragColor = vec4(color.xyz * ambient, color.w);
}
