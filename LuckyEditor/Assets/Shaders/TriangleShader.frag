#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

uniform vec3 u_Color;

void main()
{
    // color = vec4(u_Color, 1.0);
    color = v_Color;
}