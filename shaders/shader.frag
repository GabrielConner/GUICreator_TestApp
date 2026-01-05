#version 460 core

uniform vec4 backgroundColor;

out vec4 fragColor;

layout(std140, binding=0) uniform GUIElementInformation {
		vec4 primaryColor;
		vec4 secondaryColor;
		vec2 gradientStart;

  float gradientStep;
		float gradientDistance;

		bool gradientX;
		bool gradientY;
		bool manhattan;
};

layout(std140, binding=1) uniform TransformInfo {
		vec2 position;
		vec2 scale;
		vec2 sizeFix;
};

in vec2 fragUV;
in vec2 fragPos;
in vec2 boxPos;


uniform sampler2D texTarget;
uniform bool useTexture;


bool outsideOf(vec2 pos, vec2 bottomLeft, vec2 topRight) {
		return pos.x < bottomLeft.x || pos.y < bottomLeft.y || pos.x > topRight.x || pos.y > topRight.y;
}



void main() {
		if (useTexture == true) {
				fragColor = texture(texTarget, fragUV);
		} else {
				float mixAmount = 0.f;
				vec2 distTo = boxPos;

				if (gradientX) {
						distTo.x = gradientStart.x;
				}
				if (gradientY) {
						distTo.y = gradientStart.y;
				}
				if (manhattan)
						mixAmount = abs(distTo.x - boxPos.x) + abs(distTo.y - boxPos.y);
				else
						mixAmount = distance(boxPos, distTo);

				if (gradientStep > 0) {
						mixAmount = gradientStep * floor(mixAmount / gradientStep);
				}

				mixAmount /= gradientDistance;

				fragColor = mix(primaryColor, secondaryColor, clamp(mixAmount, 0.0f, 1.0f));
		}
}