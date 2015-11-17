#version 400

// Attributes passed from the geometry shader
in vec4 frag_colour;
in vec2 tex_coord;

// Passed in via the material file
uniform sampler2D tex_samp;

void main (void)
{
	// Very simple fragment shader, but we can do anything we want here
	vec4 outval = texture(tex_samp, tex_coord);
	gl_FragColor = outval;
}