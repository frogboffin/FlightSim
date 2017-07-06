#version 430

uniform sampler2D texColour;

vec3 l_dir = vec3(-0.0f,1.0f,0.0f);

vec4 diffuseColour = vec4(1,1,1,0);
vec4 ambientColour = vec4(0.1f,0.1f,0.0f,0);
vec4 specColour = vec4(1.0,1.0,1.0,1);

in vec4 outColour;
in vec2 outUV;

in vec4 outNormal;
in vec4 eye;

out vec4 outFrag;

void main()
{

	// normalize both input vectors
    vec3 n = vec3(outNormal);
    vec3 e = normalize(vec3(eye));

    float intensity = max(dot(n, e), 0.0);
	float angleToLight = max(dot(vec3(outNormal), l_dir), 0);

    // if the vertex is lit compute the specular color
    if (angleToLight > 0.0) 
	{
        //half vector
        vec3 h = normalize(e + l_dir);  
        //specular term into spec
        float intSpec = max(dot(h,n), -0.0);
        specColour = pow(intSpec,12f) * specColour;
    }


	outFrag = texture(texColour, outUV);

}



