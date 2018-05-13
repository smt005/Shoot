#if defined BUILD_WIN_GLES || defined BUILD_WIN_GLFW
    precision mediump float;
#endif

uniform vec4 u_color;

void main()
{
	gl_FragColor = u_color;
}
