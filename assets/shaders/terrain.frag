#version 410 core

layout(location=0) in vec2 v_Texcoord;
layout(location=1) in float v_Light;

uniform sampler2D u_ColorSampler;

void main() {
	gl_FragColor = texture2D(u_ColorSampler, v_Texcoord);
	gl_FragColor.rgb *= v_Light;
}