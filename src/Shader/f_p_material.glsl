#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 f_Color;
in vec3 f_Normal;
in vec3 f_Position;
in vec2 f_TexCoord;

out vec4 FragColor;

uniform vec3 u_CameraPosition;
uniform Material u_Material;
uniform Light u_Light;  

void main(){
  vec3 norm = normalize(f_Normal);
  vec3 lightDirection = normalize(u_Light.position - f_Position);  
  float diff = max(dot(norm, lightDirection), 0.0);

  vec3 viewDirection = normalize(u_CameraPosition - f_Position);
  vec3 reflectDirection = reflect(-lightDirection, norm);  
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);

  vec3 ambient = u_Light.ambient * u_Material.ambient;
  vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);
  vec3 specular = u_Light.specular * (spec * u_Material.specular);  

  FragColor = vec4((ambient + diffuse + specular) * f_Color, 1.0);
}