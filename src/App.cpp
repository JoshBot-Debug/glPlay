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
  controlPanel.setCamera(&camera);
  controlPanel.setResourceManager(&resource);

  /**
   * Setup a camera
   * Specify the type, and other properties.
   */
  camera.setPosition(0.0f, 0.0f, 20.0f);

  /**
   * Load the model foo
   */
  Model *sphere = resource.loadModel("assets/model/sphere.fbx");
  Model *cube = resource.loadModel("assets/model/cube-textured.fbx");

  /**
   * Setup the shader
   */
  Shader &shader = resource.getShader();
  unsigned int v_transform = shader.compile("src/Shader/v_transform.glsl", ShaderType::VERTEX_SHADER);
  unsigned int f_material = shader.compile("src/Shader/f_material.glsl", ShaderType::FRAGMENT_SHADER);
  const unsigned int modelShader = shader.createProgram({v_transform, f_material});

  const unsigned int sphere1ID = sphere->createInstance();
  const unsigned int sphere2ID = sphere->createInstance();

  const unsigned int cube1ID = cube->createInstance();
  const unsigned int cube2ID = cube->createInstance();

  Instance &iSphere1 = sphere->getInstance(sphere1ID);
  Instance &iSphere2 = sphere->getInstance(sphere2ID);

  Instance &iCube1 = cube->getInstance(cube1ID);
  Instance &iCube2 = cube->getInstance(cube2ID);

  iSphere1.translate.x = -5.0f;
  iSphere2.translate.x = -5.0f;
  iSphere2.translate.y = 5.0f;

  iCube1.translate.x = 5.0f;
  iCube2.translate.x = 5.0f;
  iCube2.translate.y = 5.0f;

  MultiModelInstanceBuffer &buffer = buffers.emplace_back();

  instancedCommands.emplace_back();
  DrawElementsIndirectCommand &sphereCommand = instancedCommands.at(sphere->getID());
  const unsigned int spherePartitionID = buffer.addBufferData(sphere->getVertices(), sphere->getIndices(), sphereCommand.firstIndex, sphereCommand.baseVertex);
  buffer.add(spherePartitionID, iSphere1, sphereCommand.baseInstance);
  buffer.add(spherePartitionID, iSphere2, sphereCommand.baseInstance);
  sphereCommand.count = sphere->getIndices().size();
  sphereCommand.primCount = sphere->getInstances().size();


  instancedCommands.emplace_back();
  DrawElementsIndirectCommand &cubeCommand = instancedCommands.at(cube->getID());
  const unsigned int cubePartitionID = buffer.addBufferData(cube->getVertices(), cube->getIndices(), cubeCommand.firstIndex, cubeCommand.baseVertex);
  buffer.add(cubePartitionID, iCube1, cubeCommand.baseInstance);
  buffer.add(cubePartitionID, iCube2, cubeCommand.baseInstance);
  cubeCommand.count = cube->getIndices().size();
  cubeCommand.primCount = cube->getInstances().size();

  unsigned int indirectBuffer;
  glGenBuffers(1, &indirectBuffer);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectBuffer);
  glBufferData(GL_DRAW_INDIRECT_BUFFER, instancedCommands.size() * sizeof(DrawElementsIndirectCommand), instancedCommands.data(), GL_STATIC_DRAW);

  // Begins the onDraw loop
  open();
}

void App::onUpdate()
{
  const glm::vec2 &size = Window::GetDimensions();

  camera.setViewportSize(size);
  camera.update();

  controlPanel.update();

  MultiModelInstanceBuffer &buffer = buffers.at(0);
  const std::vector<Model *> &models = resource.getModels();

  for (unsigned int i = 0; i < models.size(); i++)
  {
    const std::vector<Instance> &instances = models.at(i)->getInstances();
    for (unsigned int j = 0; j < instances.size(); j++)
      buffer.update(i, j, instances.at(j));
  }
}

void App::onDraw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Shader &shader = resource.getShader();
  shader.bind(0);
  shader.setUniformMatrix4fv("u_ViewProjection", camera.getViewProjectionMatrix());

  Renderer::Draw(instancedCommands);

  controlPanel.draw();
}