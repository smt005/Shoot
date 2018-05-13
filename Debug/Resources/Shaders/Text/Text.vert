uniform mat4 uMatrix;

attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;

void main(void)
{
	v_texCoord = a_texCoord;
	gl_Position = uMatrix * a_position;
}
