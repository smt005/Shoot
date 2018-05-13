attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec2 v_texCoord;
varying vec4 uv_color;

uniform mat4 u_matProjectionView;
uniform mat4 u_matViewModel;
uniform vec3 u_lightDirection;
uniform vec4 u_color;
uniform vec4 u_ambientColor;

void main(void)
{
    mat4 u_transform = u_matProjectionView * u_matViewModel;
    
    gl_Position = u_transform * a_position;
    
    vec4 n = u_matViewModel * vec4(a_normal, 0.0);
    
    vec3 v_normal = normalize(n.xyz);
    v_texCoord = a_texCoord;
  
    float lcoef = max(dot(v_normal, -u_lightDirection.xyz), 0.0);

    uv_color = u_ambientColor;
    uv_color.rgb += lcoef;
    uv_color *= u_color;
}

