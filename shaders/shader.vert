#version 460 core

layout(location=0) in vec2 pos;
layout(location=1) in vec2 uv;


layout(std140, binding=1) uniform TransformInfo {
		vec2 position;
		vec2 scale;
		vec2 sizeFix;
};


out vec2 fragUV;
out vec2 fragPos;
out vec2 boxPos;

void main() {
		boxPos = pos;
		fragPos = (pos * scale) + position;
		gl_Position = vec4(fragPos, 0, 1);

		fragUV = uv;
}