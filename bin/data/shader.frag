#version 150
in vec4 verts;
out vec4 outputColor;
in float qnoise;
uniform float time;
uniform bool redHell;
uniform float r_color;
uniform float g_color;
uniform float b_color;


void main()
{
  float  r,g,b;
  
  
  vec3 normal = normalize(verts.xyz);
  
 
  r = cos(qnoise * (r_color));
  g = cos(qnoise * g_color);
  b = cos(qnoise * (b_color));
    
   outputColor = vec4 (r,g,b,1. );
  
}