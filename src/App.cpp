#include "App.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include "Engine/PerspectiveCamera.h"
#include "Engine/Shader.h"

#include "Engine/Core/ArrayBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

const WindowOptions opts = {.title = "glPlay", .width = 800, .height = 600, .enableDepth = true, .enableVSync = true, .MSAA = 16, .imguiEnableDocking = true, .maximized = true};

/**
 * TODO need to add a ViewportManager class that holds all this information\
 * so that it can be edited/controlled from all menus and property panels.
 */
App::App() : Window(opts)
{
  controlPanel.setEngine(&engine);

  /**
   * Setup a camera
   * Specify the type, and other properties.
   */
  PerspectiveCamera *camera = engine.createCamera<PerspectiveCamera>();
  camera->setOffset(0.5f, 0.5f);
  camera->setPosition(2.0f, 2.0f, 10.0f);
  camera->setRotation(10.0f, 0.0f, 0.0f);

  /**
   * Setup a light source
   * Specify the type and other properties.
   */
  // Light *light = new Light();
  // light->setType(LightType::Ambient);

  /**
   * Load the model foo
   */
  // Model *sphere = engine.createModel("assets/model/sphere.fbx");
  Model *cube = engine.createModel("assets/model/cube-textured.fbx");

  /**
   * Setup the shader
   */
  Shader *shader = engine.getShader();
  unsigned int vertex = shader->compile("src/Shader/vertex.glsl", ShaderType::VERTEX_SHADER);
  unsigned int fragment = shader->compile("src/Shader/fragment.glsl", ShaderType::FRAGMENT_SHADER);
  shader->createProgram({vertex, fragment});

  /**
   * Load all textures
   */
  Texture2D *wall = engine.createTexture2D("assets/textures/wall.jpg");
  Texture2D *brick = engine.createTexture2D("assets/textures/brick.jpg");

  /**
   * Create all materials
   */
  // Material *material = new Material();
  // material->setDiffuseTexture(diffuseTexture);
  // material->setDiffuseColor(glm::vec3(1.0f, 0.5f, 0.3f));
  // material->setRoughness(0.8f);
  // material->setShininess(32.0f);

  // sphere->addTexture(brick);
  cube->addTexture(wall);

  // sphere->createInstance();
  cube->createInstance();
  // Instance &i1 = renderer.add<Instance>("sphere", "i1");
  // Instance &i2 = renderer.add<Instance>("cube", "i2");
  // i2.translate.x = 6.0f;
  // Begins the onDraw loop
  open();
}

void App::onUpdate()
{
  engine.getCamera<Camera>()->setViewportSize(Window::GetDimensions());

  engine.update();

  controlPanel.update();
}

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  engine.getShader()->bind(0);

  engine.begin();

  engine.draw();

  controlPanel.draw();
}