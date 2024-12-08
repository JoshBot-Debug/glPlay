#version 330 core

in vec4 f_Color;
in vec3 f_Normal;
in vec3 f_Position;
in vec2 f_TexCoord;

out vec4 FragColor;

uniform sampler2D u_Texture;

void main(){
  // FragColor=texture(u_Texture, f_TexCoord)*f_Color;
  FragColor=f_Color;
}