
varying vec2 v_texCoord;
uniform sampler2D s_baseMap;
uniform vec4 u_color;
uniform float u_alpha;

void main(void)
{
	vec2 texCoord = v_texCoord;
	vec4 color = texture2D(s_baseMap, v_texCoord).bgra * u_color;
	color.a = u_alpha;
	gl_FragColor = color;
}
