#include "App.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include "Engine/Camera/PerspectiveCamera.h"
#include "Engine/Light/PointLight.h"
#include "Engine/Light/DirectionalLight.h"
#include "Engine/Shader.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Types.h"

const WindowOptions opts = {.title = "glPlay", .width = 800, .height = 600, .enableDepth = true, .enableVSync = false, .MSAA = 16, .imguiEnableDocking = true, .maximized = true};

/**
 * TODO need to add a ViewportManager class that holds all this information\
 * so that it can be edited/controlled from all menus and property panels.
 */
App::App() : Window(opts)
{
  controlPanel.setCamera(&camera);
  controlPanel.setResourceManager(&resource);

  /**
   * Setup a camera
   * Specify the type, and other properties.
   */
  camera.setPosition(0.0f, 0.0f, 20.0f);

  /**
   * Setup the shader
   */
  Shader &shader = resource.getShader();
  unsigned int v_transform = shader.compile("src/Shader/v_p_transform.glsl", ShaderType::VERTEX_SHADER);
  unsigned int f_material = shader.compile("src/Shader/f_p_material.glsl", ShaderType::FRAGMENT_SHADER);
  const unsigned int modelShader = shader.createProgram({v_transform, f_material});

  /**
   * Load the model foo
   */
  Model *model = resource.loadModel("assets/model/sphere.obj");
  model->createInstance();

  draw.addModel(model);
  draw.update();

  // Begins the onDraw loop
  open();
}

void App::onUpdate()
{
  const glm::vec2 &size = Window::GetDimensions();

  camera.setViewportSize(size);
  camera.update();

  const std::vector<Model *> &models = resource.getModels();
  for (Model *model : resource.getModels())
    draw.update(model->getID(), 0, model->getInstances());

  controlPanel.update();
}

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Shader &shader = resource.getShader();
  shader.bind(0);
  shader.setUniformMatrix4fv("u_View", camera.getViewMatrix());
  shader.setUniformMatrix4fv("u_Projection", camera.getProjectionMatrix());

  double delta = Time::GetDeltaTime();

  glm::vec3 lightPosition = {5.0f, 5.0f, 5.0f};
  lightPosition.x = 1.0f + sin(glfwGetTime()) * 5.0f;
  lightPosition.y = sin(glfwGetTime() / 2.0f) * 5.0f;

  shader.setUniform3f("u_CameraPosition", camera.position);

  shader.setUniform3f("u_Material.ambient", 0.24725f, 0.1995f, 0.0745f);
  shader.setUniform3f("u_Material.diffuse", 0.75164f, 0.60648f, 0.22648f);
  shader.setUniform3f("u_Material.specular", 0.628281f, 0.555802f, 0.366065f);
  shader.setUniform1f("u_Material.shininess", 0.4f * 128.0f);

  shader.setUniform3f("u_Light.position", lightPosition);
  shader.setUniform3f("u_Light.specular", 1.0f, 1.0f, 1.0f);
  shader.setUniform3f("u_Light.ambient", 1.0f, 1.0f, 1.0f);
  shader.setUniform3f("u_Light.diffuse", 1.0f, 1.0f, 1.0f);

  Renderer::Draw(draw.getCommands());

  controlPanel.draw();
}