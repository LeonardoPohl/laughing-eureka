#shader vertex
#version 330 core

// Positions/Coordinates
layout(location = 0) in vec3 aPos;
// Colors
layout(location = 1) in vec3 aColor;
// Texture Coordinates
layout(location = 2) in vec2 aTex;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Controls the scale of the vertices
uniform float u_scale;

// Inputs the matrices needed for 3D viewing with perspective
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = u_proj * u_view * u_model * vec4(u_scale * aPos, 1.0);
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
}

#shader fragment
#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D u_Texture;


void main()
{
	FragColor = texture(u_Texture, texCoord);
}