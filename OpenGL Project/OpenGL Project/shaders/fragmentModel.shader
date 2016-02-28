#version 410
in vec2 vTexCoord;
in vec4 vNormal;
in vec4 vPosition;
in vec4 vTangent;
in vec3 vBiTangent;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform vec3 LightColor;
uniform vec3 LightDir;

void main()
{ 
	mat3 TBN = mat3( normalize(vTangent), normalize(vBiTangent), normalize(vNormal)); 
	vec3 N = texture(normalMap, vTexCoord).xyz * 2 - 1;
	float d = max(0, dot( normalize(TBN * N), normalize(LightDir)));
	FragColor = texture(diffuse, vTexCoord);
	FragColor.rgb = FragColor.rgb * d;
}