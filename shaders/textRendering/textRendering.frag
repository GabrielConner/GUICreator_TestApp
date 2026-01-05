#version 460 core

out vec4 fragColor;

in vec2 fragUV;

uniform usampler2D bitmapTexture;

uniform vec4 textColor;


void main () {
		fragColor = texture(bitmapTexture, fragUV).x / 255.f * textColor;
}