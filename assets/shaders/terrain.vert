#version 410 core

layout(location=0) in vec4 a_Position;
layout(location=1) in vec2 a_Texcoord;
layout(location=2) in float a_Light;

layout(location=0) out vec2 v_Texcoord;
layout(location=1) out float v_Light;

uniform mat4 u_MVPMatrix;

void main() {
	gl_Position = u_MVPMatrix * a_Position;
	
	v_Texcoord = a_Texcoord;
	v_Light = a_Light;
}