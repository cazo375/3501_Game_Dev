#version 400

// Attributes passed from the vertex shader
in vec3 position_interp;
in vec3 normal_interp;
in vec4 colour_interp;
in vec2 uv_interp;

// Attributes passed with the material file
uniform vec4 ambient_colour;
uniform vec4 diffuse_colour;
uniform vec4 specular_colour;
uniform float ambient_amount;
uniform float phong_exponent;
uniform sampler2D diffuse_map;


void main() 
{
	// Retrieve texture value
	vec4 pixel = texture(diffuse_map, uv_interp);

	// Apply Texture To The Image
	gl_FragColor = pixel;
}