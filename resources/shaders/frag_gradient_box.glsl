#version 330 core

uniform float tempcolor;
in vec2 pos;

out vec4 fragColor;

void main()
{
    vec2 newPos = fract(pos * 10.0);
    fragColor = vec4(newPos, tempcolor, 1.0);
}
