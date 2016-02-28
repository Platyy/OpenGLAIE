#version 410
in vec2 vTexCoord;

in float maxHeight;

uniform sampler2D perlinMap;
uniform sampler2D grassDiffuse; //red
uniform sampler2D rocksDiffuse; //blue

uniform float grassOffset;
uniform float rocksOffset;
uniform vec3 LightColor;
uniform vec3 LightDir;

out vec4 FragColor;

void main()
{
	float height = texture(perlinMap, vTexCoord).r;

	vec4 rocks = texture(rocksDiffuse, vTexCoord);
	vec4 grass = texture(grassDiffuse, vTexCoord);
	
	grass *= height + grassOffset;
	rocks *= height + rocksOffset;

	FragColor = mix(grass, rocks, height);
}