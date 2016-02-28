#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;

out vec2 vTexCoord;

uniform sampler2D perlinMap;

uniform mat4 ProjectionView;


out float maxHeight;

void main() 
{
	maxHeight = 30;
	vec4 pos = Position;
	pos.y += texture(perlinMap, TexCoord).r * maxHeight;
	
	vTexCoord = TexCoord;
	gl_Position = ProjectionView * pos; 
}