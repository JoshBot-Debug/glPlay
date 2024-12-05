#include "App.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include <glm/gtc/matrix_transform.hpp>

const WindowOptions opts = {.title = "glPlay", .width = 800, .height = 600, .enableDepth = true, .enableVSync = true, .MSAA = 16, .imguiEnableDocking = true, .maximized = true};

/**
 * TODO need to add a ViewportManager class that holds all this information\
 * so that it can be edited/controlled from all menus and property panels.
 */
App::App() : Window(opts)
{
  /**
   * Setup a camera
   * Specify the type, and other properties.
   */
  camera.setType(CameraType::Perspective);
  camera.setOffset(0.5f, 0.5f);
  camera.setPosition(2.0f, 2.0f, 10.0f);
  camera.setRotation(10.0f, 0.0f, 0.0f);

  /**
   * Setup a light source
   * Specify the type and other properties.
   */
  // Light *light = new Light();
  // light->setType(LightType::Ambient);

  /**
   * Load the model foo
   */
  Model *sphere = new Model("sphere", "assets/model/sphere.fbx");
  Model *cube = new Model("cube", "assets/model/cube-textured.fbx");

  Shader *shader = renderer.getShader();

  unsigned int vertex = shader->compile("src/Shader/vertex.glsl", ShaderType::VERTEX_SHADER);
  unsigned int fragment = shader->compile("src/Shader/fragment.glsl", ShaderType::FRAGMENT_SHADER);

  /**
   * Create a shader program & link some shaders
   */
  shader->createProgram("default", {vertex, fragment});
  
  /**
   * Load all textures
   */
  Texture *wallTexture = new Texture("wall-texture", "assets/textures/wall.jpg");
  Texture *brickTexture = new Texture("brick-texture", "assets/textures/brick.jpg");

  /**
   * Create all materials
   */
  // Material *material = new Material();
  // material->setDiffuseTexture(diffuseTexture);
  // material->setDiffuseColor(glm::vec3(1.0f, 0.5f, 0.3f));
  // material->setRoughness(0.8f);
  // material->setShininess(32.0f);

  // model->setMaterial(material);
  sphere->addTexture(brickTexture);
  cube->addTexture(wallTexture);

  /**
   * Setup the renderer
   */
  renderer.setCamera(&camera);
  // renderer.addLight(light);
  renderer.addModel(sphere);
  renderer.addModel(cube);

  Instance &i1 = renderer.add<Instance>("sphere", "i1");
  Instance &i2 = renderer.add<Instance>("cube", "i2");
  i2.translate.x = 6.0f;

  debugMenu.setCamera(&camera);
  debugMenu.addInstance("i1", &i1);
  debugMenu.addInstance("i2", &i2);
  debugMenu.addShader(shader);

  // Begins the onDraw loop
  open();
}

void App::onUpdate()
{
  debugMenu.update();
}

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderer.getShader()->bind("default");

  renderer.draw();

  debugMenu.draw();
}