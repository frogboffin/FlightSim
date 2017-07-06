#version 430

uniform mat4 model, view, projection;

in vec4 position;
//in vec4 inNormal;
in vec2 inUV;

//out vec4 outNormal;
out vec2 outUV;

out vec4 eye;

void main()
{	
	gl_Position = projection*view*model*position;

	eye = (projection * position);
	//outNormal = vec4(normalize(inverse(mat3(model)) * vec3(inNormal)),1);

	outUV = inUV;
}
