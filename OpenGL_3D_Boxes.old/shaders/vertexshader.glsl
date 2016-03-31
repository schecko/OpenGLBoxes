
#version 430 core

//variable prefixes
//a == from application
//v == from vertexshader
//f == from fragmentshader
//g == from geometryshader
//  == created as a temp variable/not passed in by anything

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vColor;
out vec2 vTexCoord;

layout (location = 29) uniform mat4 aModel;
layout (location = 4) uniform mat4 aView;
layout (location = 5) uniform mat4 aProjection;

void main()
{
	gl_Position = aProjection * aView * aModel * vec4(aPosition, 1.0);
	vColor = aColor;
	vTexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

}