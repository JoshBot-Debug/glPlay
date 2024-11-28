#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void setFrameBufferSize(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void errorCallback(int error, const char *description)
{
  std::cerr << "GLFW Error " << error << ":" << description << std::endl;
}

void Window::beginLoop()
{
  this->onInitialize();

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    this->onUpdate();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    this->onDraw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  this->onCleanUp();
}

Window::Window(const WindowOptions &options)
{
  glfwSetErrorCallback(errorCallback);

  if (!glfwInit())
    std::cerr << "GLFW initialization failed!" << std::endl;

  const int init = glewInit();

  if (init != GLEW_OK)
    std::cerr << "GLEW initialization failed! Error code: " << init << std::endl;

  // If we want compatibility
  // GL 3.0 + GLSL 130
  // const char* glsl_version = "#version 130";
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // GL 4.6 + GLSL 460
  const char *glsl_version = "#version 460";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  window = glfwCreateWindow(options.width, options.height, options.title.c_str(), nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, setFrameBufferSize);

  if (options.enableVSync)
    glfwSwapInterval(1);

  if (options.enableDepth)
    glEnable(GL_DEPTH_TEST);

  IMGUI_CHECKVERSION();

  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();

  if (options.imguiEnableKeyboard)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  if (options.imguiEnableGamepad)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  if (options.imguiEnableDocking)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  if (options.darkMode)
    ImGui::StyleColorsDark();
  else
    ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

Window::~Window()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}
