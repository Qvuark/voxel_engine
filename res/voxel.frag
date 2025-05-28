#version 330 core

in vec2 a_texCoord;
in float a_light;
in vec3 a_color_mult;  

out vec4 f_color;

uniform sampler2D u_texture0;

void main()
{
    vec4 texColor = texture(u_texture0, a_texCoord);

    f_color = texColor * vec4(a_color_mult * a_light, 1.0);
}