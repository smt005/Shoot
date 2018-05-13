#if defined BUILD_WIN_GLES || defined BUILD_WIN_GLFW
    precision mediump float;
#endif

uniform vec4 vColor;
varying vec2 v_texCoord;
uniform sampler2D s_baseMap;

void main(void)
{
	vec2 texCoord = v_texCoord;
	texCoord.t = 1.0 - texCoord.t;
	
	float value = texture2D(s_baseMap, texCoord).r;	// + 0.1; // + 0.1 - тест
	gl_FragColor = vec4(vColor.rgb, vColor.a * value);
}
