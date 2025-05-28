#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in float v_light;
layout (location = 3) in vec3 v_color_mult;  

out vec2 a_texCoord;
out float a_light;
out vec3 a_color_mult;  

uniform mat4 model;
uniform mat4 projview;

void main()
{
    a_light = v_light;
    a_texCoord = v_texCoord;
    a_color_mult = v_color_mult; 
    
    gl_Position = projview * model * vec4(v_position, 1.0);
}