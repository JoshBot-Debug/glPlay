#include "App.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include "Engine/Camera/PerspectiveCamera.h"
#include "Engine/Light/PointLight.h"
#include "Engine/Light/DirectionalLight.h"
#include "Engine/Shader.h"

#include "Engine/Core/ArrayBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

const WindowOptions opts = {.title = "glPlay", .width = 800, .height = 600, .enableDepth = true, .enableVSync = false, .MSAA = 16, .imguiEnableDocking = true, .maximized = true};

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
  camera->setPosition(0.0f, 0.0f, 20.0f);


  ResourceManager *resource = engine.getResourceManager();

  /**
   * Load the model foo
   */
  Model *sphere = resource->loadModel("assets/model/sphere.fbx");
  Model *cube = resource->loadModel("assets/model/cube-textured.fbx");

  /**
   * Setup the shader
   */
  Shader *shader = engine.getShader();
  unsigned int v_transform = shader->compile("src/Shader/v_transform.glsl", ShaderType::VERTEX_SHADER);
  unsigned int f_material = shader->compile("src/Shader/f_material.glsl", ShaderType::FRAGMENT_SHADER);
  const unsigned int modelShader = shader->createProgram({v_transform, f_material});

  const unsigned int sphere1ID = sphere->createInstance();
  const unsigned int sphere2ID = sphere->createInstance();

  const unsigned int cube1ID = cube->createInstance();
  const unsigned int cube2ID = cube->createInstance();

  sphere->getInstance(sphere1ID).translate.x = -5.0f;
  sphere->getInstance(sphere2ID).translate.x = -5.0f;
  sphere->getInstance(sphere2ID).translate.y = 5.0f;

  cube->getInstance(cube1ID).translate.x = 5.0f;
  cube->getInstance(cube2ID).translate.x = 5.0f;
  cube->getInstance(cube2ID).translate.y = 5.0f;

  // engine->createRenderer<BatchInstances>();
  // engine->createRenderer<MultiBatchInstances>();

  // Begins the onDraw loop
  open();
}

void App::onUpdate()
{
  const glm::vec2 &size = Window::GetDimensions();

  engine.getCamera<Camera>()->setViewportSize(size);

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