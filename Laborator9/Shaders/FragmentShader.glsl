#version 330
 
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform bool mix_texture;
 
in vec2 texcoord;
uniform  vec4 color;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO : calculate the out_color using the texture2D() function
	vec4 color1 = texture2D(texture_1, texcoord);
	vec4 color2 = texture2D(texture_2, texcoord);

	if (mix_texture == true)
	{
		out_color = mix(color1, color2, 0.5f);
	}
	else
	{
		out_color = color1;
	}

	if (out_color.a < 0.5f) 
	{
		discard;
	}
}