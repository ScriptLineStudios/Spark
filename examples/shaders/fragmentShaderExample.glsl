#version 330 core

out vec4 FragColor;
in vec3 outColor;

in vec2 texCoord;

uniform sampler2D tex0;

void main(){
	FragColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
}
