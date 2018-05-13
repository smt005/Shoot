#if defined BUILD_WIN_GLES || defined BUILD_WIN_GLFW
    precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D s_baseMap;
uniform vec4 u_color;

void main(void)
{
	vec2 texCoord = v_texCoord;
	vec4 color = texture2D(s_baseMap, v_texCoord).bgra * u_color;
	gl_FragColor = color;
}
