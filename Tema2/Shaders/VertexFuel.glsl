#version 330

layout(location = 0) in vec3 v_position;

uniform mat4 Model;
uniform vec3 v_color;

out vec3 frag_color;

void main()
{
	 frag_color = v_color;

	 gl_Position = Model * vec4(v_position, 1.0f);
}
