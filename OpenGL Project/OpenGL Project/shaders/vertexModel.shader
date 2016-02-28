#version 410
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;
layout(location=4) in vec4 Weights;
layout(location=5) in vec4 Indices;

out vec4 vNormal;
out vec4 vPosition;
out vec2 vTexCoord;
out vec4 vTangent;
out vec3 vBiTangent;
const int MAX_BONES = 128;

uniform mat4 bones[MAX_BONES];
uniform mat4 ProjectionView;
uniform mat4 World;

void main()
{
vPosition = Position;
vNormal = Normal;
vTangent = Tangent;
vBiTangent = cross(Normal.xyz, Tangent.xyz);
vTexCoord = TexCoord;
ivec4 index = ivec4(Indices);
vec4 P  = bones[index.x] * Position * Weights.x;
     P += bones[index.y] * Position * Weights.y;
     P += bones[index.z] * Position * Weights.z;
     P += bones[index.w] * Position * Weights.w;
gl_Position = ProjectionView * World * P;
}