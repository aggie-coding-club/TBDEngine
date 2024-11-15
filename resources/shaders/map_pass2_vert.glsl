#version 120


attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

attribute vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelInverseTranspose;

uniform mat4 lightView;
uniform mat4 lightProjection;

varying vec4 lightSpace;
varying vec3 fragPosition;
varying vec3 fragNormal;

void main()
{
    fragPosition = vec3(model * vec4(vPositionModel, 1.0));
    fragNormal = normalize(vec3(modelInverseTranspose * vec4(vNormalModel, 1.0)));
    lightSpace = (lightProjection * lightView * model * vec4(vPositionModel, 1.0));

	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
}

