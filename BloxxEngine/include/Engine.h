#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace BE {

inline VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

inline void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

/**
 * @brief QueueFamilyIndices struct
 *
 * This struct is used to store the queue family indices.
 */
struct QueueFamilyIndices {
  uint32_t graphicsFamily;
};

/**
 * @brief Engine class
 *
 * This class is used to create the main engine object, which is responsible
 * for initializing the window and Vulkan. It also contains the main loop and
 * cleanup functions.
 *
 * @param title The title of the window.
 * @param width The width of the window.
 * @param height The height of the window.
 *
 * TODO: Maybe only allow one instance of the engine to be created.
 * REF: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
 * TODO: Split the engine class into logical parts.
 */
class Engine {
 public:
  Engine(const std::string& title = "Vulkan Engine", const int width = 800,
         const int height = 600)
      : title(title), width(width), height(height) {}
  ~Engine() = default;

  void run();

 private:
  /**
   * @brief initWindow function
   *
   * This function initializes the window using GLFW.
   */
  void initWindow();

  /**
   * @brief initVulkan function
   *
   * This function initializes Vulkan.
   */
  void initVulkan();

  /**
   * @brief createVulkanInstance function
   *
   * This function creates the Vulkan instance.
   */
  void createVulkanInstance();

  /**
   * @brief checkValidationLayerSupport function
   *
   * This function checks if the validation layers are supported.
   *
   * @return bool
   */
  bool checkValidationLayerSupport();

  /**
   * @brief getRequiredExtensions function
   *
   * This function returns the required extensions.
   *
   * @return std::vector<const char*>
   */
  std::vector<const char*> getRequiredExtensions();

  /**
   * @brief populateDebugMessengerCreateInfo function
   *
   * This function populates the debug messenger create info.
   *
   * @param createInfo The debug messenger create info.
   */
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT& createInfo);

  /**
   * @brief setUpDebugMessenger function
   *
   * This function sets up the debug messenger.
   */
  void setUpDebugMessenger();

  /**
   * @brief pickPhysicalDevice function
   *
   * This function picks the physical device.
   *
   * TODO: Add score based selection.
   */
  void pickPhysicalDevice();

  /**
   * @brief isDeviceSuitable function
   *
   * This function checks if the device is suitable.
   *
   * @param device The physical device.
   * @return bool
   */
  bool isDeviceSuitable(VkPhysicalDevice device);

  /**
   * @brief findQueueFamilies function
   *
   * This function finds the queue families.
   *
   * @param device The physical device.
   * @return uint32_t
   */
  uint32_t findQueueFamilies(VkPhysicalDevice device);

  /**
   * @brief mainLoop function
   *
   * This function is the main loop of the engine.
   */
  void mainLoop();

  /**
   * @brief cleanup function
   *
   * This function cleans up the resources used by the engine.
   */
  void cleanup();

  std::string title;
  int width, height;
  GLFWwindow* window;
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
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
