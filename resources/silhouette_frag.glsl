#version 120

uniform mat4 view;

varying vec3 fragPosition;
varying vec3 fragNormal;

void main()
{
    vec4 viewPosition = view * vec4(fragPosition, 1.0);

    vec3 viewDir = normalize(-viewPosition.xyz); // From camera to fragment
    vec3 normal = normalize(fragNormal);

    vec3 color = vec3(0.0f);

    if(abs(dot(viewDir, normal)) < 0.2)
    {
        color = vec3(1.0f);
    }

    gl_FragColor = vec4(color, 1.0f);

}