#version 120

attribute vec3 vPositionModel; // in object space

uniform mat4 model;

uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    gl_Position = lightProjection * lightView * model * vec4(vPositionModel, 1.0);
}
