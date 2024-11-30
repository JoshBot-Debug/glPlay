#include "App.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Model.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Renderer/FrameBuffer.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include <glm/gtc/matrix_transform.hpp>

const WindowOptions options = {.title = "glPlay", .width = 800, .height = 600, .enableDepth = true, .enableVSync = true, .imguiEnableDocking = true};

/**
 * TODO need to add a ViewportManager class that holds all this information\
 * so that it can be edited/controlled from all menus and property panels.
 */
App::App() : Window(options)
{
  /**
   * Setup a camera
   * Specify the type, and other properties.
   */
  Camera *camera = new Camera();
  camera->setType(CameraType::Orthographic);
  camera->setOffset(0.5f, 0.5f);

  /**
   * Setup a light source
   * Specify the type and other properties.
   */
  // Light *light = new Light();
  // light->setType(LightType::Ambient);

  /**
   * Load the model foo
   */
  Model *model = new Model("plane", "assets/model/cube.fbx");

  /**
   * Load all shaders for every purpose
   */
  Shader *vShader = new Shader("src/Shader/vertex.glsl", ShaderType::VERTEX_SHADER);
  Shader *fShader = new Shader("src/Shader/fragment.glsl", ShaderType::FRAGMENT_SHADER);

  /**
   * Create a shader program
   * We'd create as many different programs as we want
   */
  ShaderProgram *shaderProgram = new ShaderProgram();
  Program *program = shaderProgram->createProgram("default");
  program->link(vShader);
  program->link(fShader);

  /**
   * Load all textures
   */
  // Texture *diffuseTexture = new Texture("foo-diffused-texture", "/path/to/texture");

  /**
   * Create all materials
   */
  // Material *material = new Material();
  // material->setDiffuseTexture(diffuseTexture);
  // material->setDiffuseColor(glm::vec3(1.0f, 0.5f, 0.3f));
  // material->setRoughness(0.8f);
  // material->setShininess(32.0f);

  // model->setMaterial(material);

  /**
   * Setup the renderer
   */
  renderer.setCamera(camera);
  // renderer.addLight(light);
  renderer.addModel(model);
  renderer.addShaderProgram(shaderProgram);

  Instance &i1 = renderer.add<Instance>("i1");
  i1.translate.x = -100.0f;
  i1.scale.x = 50.0f;
  i1.scale.y = 50.0f;
  // i1.scale.z = 50.0f;

  i1.color.r = 1.0f;
  i1.color.g = 0.0f;
  i1.color.b = 0.0f;

  Instance &i2 = renderer.add<Instance>("i2");
  i2.translate.x = 100.0f;

  i2.scale.x = 50.0f;
  i2.scale.y = 50.0f;
  // i2.scale.z = 50.0f;

  i2.color.r = 0.0f;
  i2.color.g = 0.0f;
  i2.color.b = 1.0f;

  // Begins the onDraw loop
  open();
}

// void App::onUpdate()
// {
//   if(Input::KeyPress(KeyboardKey::UP))
//   {
    
//   }
// }

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ShaderProgram *shader = renderer.getShaderProgram();
  shader->bind("default");

  renderer.draw("plane");
}