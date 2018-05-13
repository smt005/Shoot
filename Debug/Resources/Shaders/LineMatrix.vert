uniform mat4 u_matrix;

attribute vec4	a_position;

void main()
{
	gl_Position = u_matrix * a_position;
}
