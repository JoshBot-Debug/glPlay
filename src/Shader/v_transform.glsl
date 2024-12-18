#version 330 core

layout(location=0)in vec3 in_Position;
layout(location=1)in vec3 in_Normal;
layout(location=2)in vec2 in_TexCoord;
layout(location=3)in vec3 in_Tangent;
layout(location=4)in vec3 in_BiTangent;
layout(location=5)in vec3 in_Color;

layout(location=6)in int in_ID;
layout(location=7)in vec3 in_iTranslate;
layout(location=8)in vec3 in_iRotation;
layout(location=9)in vec3 in_iScale;
layout(location=10)in vec4 in_iColor;

out vec4 f_Color;
out vec3 f_Normal;
out vec3 f_Position;
out vec2 f_TexCoord;

uniform mat4 u_ViewProjection;

mat4 scale(mat4 m,vec3 s){
  m[0][0]*=s.x;
  m[1][1]*=s.y;
  m[2][2]*=s.z;
  return m;
}

mat4 rotateX(mat4 m,float angle){
  float c=cos(angle);
  float s=sin(angle);
  mat4 rotation=mat4(1.);
  rotation[1][1]=c;
  rotation[1][2]=-s;
  rotation[2][1]=s;
  rotation[2][2]=c;
  return m*rotation;
}

mat4 rotateY(mat4 m,float angle){
  float c=cos(angle);
  float s=sin(angle);
  mat4 rotation=mat4(1.);
  rotation[0][0]=c;
  rotation[0][2]=s;
  rotation[2][0]=-s;
  rotation[2][2]=c;
  return m*rotation;
}

mat4 rotateZ(mat4 m,float angle){
  float c=cos(angle);
  float s=sin(angle);
  mat4 rotation=mat4(1.);
  rotation[0][0]=c;
  rotation[0][1]=-s;
  rotation[1][0]=s;
  rotation[1][1]=c;
  return m*rotation;
}

mat4 translate(mat4 m,vec3 t){
  m[3][0]+=t.x;
  m[3][1]+=t.y;
  m[3][2]+=t.z;
  return m;
}

void main(){
  mat4 model=mat4(1.);
  
  model=scale(model,in_iScale);
  model=rotateX(model,radians(in_iRotation.x));
  model=rotateY(model,radians(in_iRotation.y));
  model=rotateZ(model,radians(in_iRotation.z));
  model=translate(model,in_iTranslate);
  
  f_Color=vec4(in_Color,1.)*in_iColor;
  f_Normal=in_Normal;
  f_Position=vec3(model*vec4(in_Position,1.));
  f_TexCoord=in_TexCoord;
  
  gl_Position=u_ViewProjection*model*vec4(in_Position,1.);
}