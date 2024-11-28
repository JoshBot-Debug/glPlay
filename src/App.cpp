#include "App.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Renderer/Model.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Renderer/FrameBuffer.h"

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

  /**
   * Setup a light source
   * Specify the type and other properties.
   */
  Light *light = new Light();
  light->setType(LightType::Ambient);

  /**
   * Load the model foo
   */
  Model *model = new Model("foo", "/path/to/model");

  /**
   * Load all shaders for every purpose
   */
  Shader *vShader = new Shader("/path/to/shader", ShaderType::VERTEX_SHADER);
  Shader *fShader = new Shader("/path/to/shader", ShaderType::FRAGMENT_SHADER);

  /**
   * Create a shader program
   * We'd create as many different programs as we want
   */
  ShaderProgram *shaderProgram = new ShaderProgram();
  Program *program = shaderProgram->createProgram("default-program");
  program->link(vShader);
  program->link(fShader);

  /**
   * Load all textures
   */
  Texture *diffuseTexture = new Texture("foo-diffused-texture", "/path/to/texture");

  /**
   * Create all materials
   */
  Material *material = new Material();
  material->setDiffuseTexture(diffuseTexture);
  material->setDiffuseColor(glm::vec3(1.0f, 0.5f, 0.3f));
  material->setRoughness(0.8f);
  material->setShininess(32.0f);

  model->setMaterial(material);

  /**
   * Create a frame buffer
   */
  FrameBuffer *scene = new FrameBuffer("scene");

  /**
   * Setup the renderer
   */
  renderer.setCamera(camera);
  renderer.addLight(light);
  renderer.addModel(model, RenderType::Instanced);
  renderer.addFrameBuffer(scene);
  renderer.addShaderProgram(shaderProgram);

  /**
   * Somewhere in the program, we will create instances.
   * This accept an argument instance that contains transforms, color, etc.
   * The argument will be a pointer.
   * 
   * You will update the Structs information and the renderer will update the unforms if changes were made before drawing
   */
  // renderer.createInstance("foo-1", instance);

  // Begins the onDraw loop
  beginLoop();
}

void App::onDraw()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // Bind the "default-program" shader
  renderer.bindShader("default-program");

  /**
   * Update uniforms for camera and lights
   */
  renderer.updateCamera();
  renderer.updateLights();

  /**
   * No need for this method, the renderer will update the uniforms for a model before drawing automatically.
   */
  // float transforms[3] = {1.0f, 1.0f, 1.0f};
  // renderer.updateInstance("foo", transforms);

  // Bind the framebuffer to render the scene to an offscreen texture
  // Optional
  renderer.bindFramebuffer("scene");

  /**
   * Draw the to the framebuffer "scene"
   */
  renderer.draw("foo");

  // Bind the default framebuffer to display the scene
  renderer.bindFramebuffer("default");

  // Apply post-processing effects (e.g., bloom, blur, etc.)
  renderer.applyPostProcessingEffects();
}