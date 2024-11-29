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

const WindowOptions options = {.title = "glPlay", .width = 800, .height = 600, .enableVSync = true, .imguiEnableDocking = true};

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
  // Camera *camera = new Camera();
  // camera->setType(CameraType::Orthographic);

  /**
   * Setup a light source
   * Specify the type and other properties.
   */
  // Light *light = new Light();
  // light->setType(LightType::Ambient);

  /**
   * Load the model foo
   */
  Model *model = new Model("plane", "assets/model/plane.fbx");

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
   * Create a frame buffer
   */
  // FrameBuffer *scene = new FrameBuffer("scene");

  /**
   * Setup the renderer
   */
  // renderer.setCamera(camera);
  // renderer.addLight(light);
  renderer.addModel(model);
  // renderer.addFrameBuffer(scene);
  renderer.addShaderProgram(shaderProgram);

  /**
   * Somewhere in the program, we will create instances.
   * This accept an argument instance that contains transforms, color, etc.
   * The argument will be a pointer.
   *
   * You will update the Structs information and the renderer will update the unforms if changes were made before drawing
   */
  InstanceManager &instance = renderer.add<Instance>("p1");

  // Begins the onDraw loop
  open();
}

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind the "default" shader
  ShaderProgram *shader = renderer.getShaderProgram();
  shader->bind("default");

  /**
   * Update uniforms for camera and lights
   */
  renderer.updateCamera();
  renderer.updateLights();

  float wSize = 800 / 2.0f;
  float hSize = 600 / 2.0f;

  // Camera position
  glm::vec2 cPosition = glm::vec2(0.0f);
  float cRotation = 0.0f;

  glm::mat4 projection = glm::ortho(-wSize, +wSize, -hSize, +hSize, -1.0f, 1.0f);
  glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-cPosition.x, -cPosition.y, 0.0f));
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(cRotation), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 u_ViewProjection = projection * (translation * rotation);

  shader->uniformMatrix4fv("u_ViewProjection", u_ViewProjection);

  /**
   * No need for this method, the renderer will update the uniforms for a model before drawing automatically.
   */
  // float transforms[3] = {1.0f, 1.0f, 1.0f};
  // renderer.updateInstance("foo", transforms);

  // Bind the framebuffer to render the scene to an offscreen texture
  // Optional
  // renderer.bindFramebuffer("scene");

  /**
   * Draw the to the framebuffer "scene"
   */
  renderer.draw("plane");

  // Bind the default framebuffer to display the scene
  // renderer.bindFramebuffer("default");

  // Apply post-processing effects (e.g., bloom, blur, etc.)
  // renderer.applyPostProcessingEffects();
}