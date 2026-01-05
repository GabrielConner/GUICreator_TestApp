#version 460 core

layout(std140, binding=1) uniform TransformInfo {
		vec2 position;
		vec2 scale;
		vec2 sizeFix;
};

in vec2 fragUV;
in vec2 fragPos;
in vec2 boxPos;

out vec4 fragColor;

uniform	vec4 borderColor;
uniform	float borderThickness;


bool outsideOf(vec2 pos, vec2 bottomLeft, vec2 topRight) {
		return pos.x < bottomLeft.x || pos.y < bottomLeft.y || pos.x > topRight.x || pos.y > topRight.y;
}



void main() {
		vec2 thickness = sizeFix * borderThickness;

		if (outsideOf(fragPos, position - scale + thickness, position + scale - thickness)) {
				fragColor = borderColor;
		} else {
				fragColor = vec4(0,0,0,0);
		}
}
