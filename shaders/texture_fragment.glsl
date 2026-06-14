#version 330 core
out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D our_texture;

void main()
{
    float ambient = 1.0;
    vec4 result = texture(our_texture, tex_coord);

    FragColor = vec4(result.xyz * ambient, 1.0);
}
