#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 toLightVector;
// Gets the Texture Unit from the main function
uniform sampler2D tex0;

uniform vec3 lightColour;

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = max(nDot,0.0);
	vec3 diffuse = brightness * lightColour;

	FragColor = texture(tex0, texCoord);
}