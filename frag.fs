#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec4 srcRectangle;
uniform int renderFlat;


void main()
{    
	if(renderFlat == 0)
	{
		vec2 source = vec2((TexCoords.x + srcRectangle.x) / srcRectangle.z, (TexCoords.y + srcRectangle.y) / srcRectangle.w);
		color = vec4(spriteColor, 1.0) * texture(image, source);
	}
	else 
		color = vec4(spriteColor, 1.0);

}  
