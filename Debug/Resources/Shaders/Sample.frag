#if defined BUILD_WIN_GLES || defined BUILD_WIN_GLFW
    precision mediump float;
#endif

void main()
{
	gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
