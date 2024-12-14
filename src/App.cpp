#include "App.h"
#include "Window/Input.h"
#include "Window/Time.h"

#include "Engine/Camera/PerspectiveCamera.h"
#include "Engine/Light/PointLight.h"
#include "Engine/Light/DirectionalLight.h"
#include "Engine/Shader.h"

#include "Engine/Core/ArrayBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Engine/Types.h"
#include "Engine/Core/VertexArray.h"
#include "Engine/Core/ArrayBuffer.h"

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
  // Model *sphere = resource.loadModel("assets/model/Keqing - Piercing Thunderbolt/Keqing - Piercing Thunderbolt.glb");
  // Model *sphere = resource.loadModel("assets/model/Keqing - Piercing Thunderbolt/Keqing.fbx");
  // Model *sphere = resource.loadModel("assets/model/Raiden Shogun/Raiden Shogun.fbx");

  // Model *sphere = resource.loadModel("assets/model/cube.fbx");
  // Model *sphere = resource.loadModel("assets/model/2B/2B.fbx");
  // Model *sphere = resource.loadModel("assets/model/2B/2B.glb");
  // Model *sphere = resource.loadModel("assets/model/sphere.fbx");

  // Model *sphere = resource.loadModel("assets/model/2-cube-joined.fbx");
  // Model *sphere = resource.loadModel("assets/model/2-cube-seperate.fbx");

  // Vertices: 4
  // -1 -1  0
  //  1 -1  0
  //  1  1  0
  // -1  1  0
  // Indices: 6 - 3 2 1 3 1 0
  // Model *sphere = resource.loadModel("assets/model/plane.fbx");

  // Vertices: 8
  // -1 -1  0
  //  1 -1  0
  //  1  1  0
  // -1  1  0
  // *********
  // -1 -4  0
  //  1 -4  0
  //  1 -2  0
  // -1 -2  0
  // Indices: 12 - 3 2 1 3 1 0 | 7 6 5 7 5 4
  // Model *sphere = resource.loadModel("assets/model/plane-joined.fbx");

  // Vertices: 8
  // Indices: 12
  Model *sphere = resource.loadModel("assets/model/plane-seperate.fbx");

  // Model *sphere = resource.loadModel("assets/model/cube-sphere-joined.fbx");
  // Model *sphere = resource.loadModel("assets/model/cube-sphere-seperate.fbx");

  // Model *sphere = resource.loadModel("assets/model/sphere.fbx");
  // Vertices: 1984
  // Indices: 2880

  /**
   * Setup the shader
   */
  Shader &shader = resource.getShader();
  unsigned int v_transform = shader.compile("src/Shader/v_transform.glsl", ShaderType::VERTEX_SHADER);
  unsigned int f_material = shader.compile("src/Shader/f_material.glsl", ShaderType::FRAGMENT_SHADER);
  const unsigned int modelShader = shader.createProgram({v_transform, f_material});

  unsigned int iSphereID = sphere->createInstance();
  Instance &iSphere = sphere->getInstance(iSphereID);

  MultiModelInstanceBuffer &buffer = buffers.emplace_back();

  instancedCommands.emplace_back();
  DrawElementsIndirectCommand &command = instancedCommands.at(sphere->getID());
  const unsigned int spherePartitionID = buffer.addBufferData(sphere->getVertices(), sphere->getIndices(), command.firstIndex, command.baseVertex);
  buffer.add(spherePartitionID, iSphere, command.baseInstance);
  command.count = sphere->getIndices().size();
  command.primCount = sphere->getInstances().size();

  std::cout << "Vertices: " << sphere->getVertices().size() << std::endl;
  std::cout << "Indices: " << sphere->getIndices().size() << std::endl;

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