#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aNorm;
// Texture Coordinates
layout (location = 2) in vec2 aTex;


// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

out vec3 surfaceNormal;
out vec3 toLightVector;

// Imports the camera matrix from the main function
uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightPosition;

void main()
{
	vec4 worldPosition = vec4(aPos,1.0);
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * worldPosition;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;

	surfaceNormal = aNorm;
	toLightVector = lightPosition - worldPosition.xyz;
}