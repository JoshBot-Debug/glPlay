#version 330 core

in vec4 f_Color;
in vec3 f_Normal;
in vec3 f_Position;

out vec4 FragColor;

uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;

void main(){
  
  // vec3 norm=normalize(f_Normal);
  // vec3 lightDir=normalize(u_LightPosition-f_Position);
  
  // float diff=max(dot(norm,lightDir),0.);
  // vec3 diffuse=diff*f_Color;
  
  float ambientStrength=1.;
  // vec4 ambient=ambientStrength*u_LightColor;
  vec4 ambient=ambientStrength*vec4(0.1,0.1,0.1,1.0);
  
  FragColor=ambient*f_Color;
  
  // FragColor=vec4(f_Color);
}