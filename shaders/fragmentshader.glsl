#version 430 core

in vec3 vColor;
in vec2 vTexCoord;
in vec4 vPosition;

out vec4 fColor;
uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform float aTime;
void main()
{
	//fColor = texture(aTexture1, vTexCoord);
	fColor = vec4(vPosition.x / length(vPosition) * sin(aTime) + .5, vPosition.y / length(vPosition) * cos(aTime) + .5, vPosition.y / length(vPosition) * sin(aTime) + .5, 1.0);
	//fColor = mix(texture(aTexture0, vTexCoord), texture(aTexture1, vTexCoord), .5);

}