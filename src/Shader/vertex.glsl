#version 330 core

layout(location=0)in vec3 in_Position;
layout(location=1)in vec3 in_Normal;
layout(location=2)in vec2 in_TexCoord;

layout(location=3)in vec3 in_Translate;
layout(location=4)in vec3 in_Rotation;
layout(location=5)in vec3 in_Scale;
layout(location=6)in vec3 in_Color;

out vec3 f_Color;

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
  
  model=translate(model,in_Translate);
  model=rotateX(model,radians(in_Rotation.x));
  model=rotateY(model,radians(in_Rotation.y));
  model=rotateZ(model,radians(in_Rotation.z));
  model=scale(model,in_Scale);
  
  f_Color=in_Color;
  gl_Position=u_ViewProjection*model*vec4(in_Position,1.);
  // gl_Position=model*vec4(in_Position,1.);
}