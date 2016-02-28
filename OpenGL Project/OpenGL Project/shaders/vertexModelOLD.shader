#version 330
layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
out vec4 vNormal;

uniform mat4 ProjectionView;
uniform mat4 World;

void main() 
{
 vNormal = Normal;
gl_Position = ProjectionView *Position; 
}