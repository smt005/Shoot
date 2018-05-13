
varying vec2 v_texCoord;
uniform sampler2D s_baseMap;

void main()
{
	vec4 baseColor;
	baseColor = texture2D(s_baseMap, v_texCoord);
	baseColor.a = 0.05;
    gl_FragColor = baseColor;
}
