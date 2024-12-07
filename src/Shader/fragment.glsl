#version 330 core

in vec4 f_Color;
in vec3 f_Normal;
in vec3 f_Position;
in vec2 f_TexCoord;

out vec4 FragColor;

uniform vec4 u_LightColor;
uniform sampler2D u_Texture;

void main(){
  
  // float ambientStrength=1.;
  // vec4 ambient=ambientStrength*u_LightColor;
  // FragColor=ambient*f_Color;
  
  FragColor=texture(u_Texture, f_TexCoord)*f_Color;
  // FragColor=f_Color;
}