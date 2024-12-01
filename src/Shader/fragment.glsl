#version 330 core

in vec4 f_Color;
in vec3 f_Normal;
in vec3 f_Position;

out vec4 FragColor;

uniform vec4 u_LightColor;
uniform vec3 u_LightPosition;

void main(){
  
  // float ambientStrength=1.;
  // vec4 ambient=ambientStrength*u_LightColor;
  // FragColor=ambient*f_Color;
  
  FragColor=vec4(1.,0.,0.,1.);
}