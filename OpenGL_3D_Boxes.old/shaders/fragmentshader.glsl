#version 430 core

in vec3 vColor;
in vec2 vTexCoord;

out vec4 fColor;
uniform sampler2D aTexture[2];
uniform sampler2D aTexture1;
void main()
{
	fColor = texture(aTexture1, vTexCoord);
	//fColor = vec4(0.5, 0.5, 0.1, 1.0);
	//fColor = mix(texture(aTexture[1], vTexCoord), texture(aTexture[0], vTexCoord), .5);
}