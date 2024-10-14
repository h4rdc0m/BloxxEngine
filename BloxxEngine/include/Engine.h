#pragma once

#include <iostream>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace BE {

class Engine {
 public:
  Engine(const std::string& title = "Vulkan Engine", const int width = 800,
         const int height = 600)
      : title(title), width(width), height(height) {}
  ~Engine() = default;

  void run();

 private:
  void initWindow();
  void initVulkan();
  void createVulkanInstance();
  bool checkValidationLayerSupport();
  std::vector<const char*> getRequiredExtensions();
  void setUpDebugMessenger();
  void mainLoop();
  void cleanup();

  std::string title;
  int width, height;
  GLFWwindow* window;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;

  const std::vector<const char*> validationLayers = {
      "VK_LAYER_KHRONOS_validation"};

  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
  }

#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif
};

}  // namespace BE
