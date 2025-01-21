#version 120

varying vec2 pos;

void main()
{
	gl_FragColor = vec4(pos, 1., 1.);
}
