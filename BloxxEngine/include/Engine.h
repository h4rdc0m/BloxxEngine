#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>

namespace BE {

class Engine {
public:
	Engine(const std::string& title): m_Title(title) {}
	~Engine() = default;
	
	void Run();

private:
	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void CleanUp();

	GLFWwindow* m_Window = nullptr;
	VkInstance m_Instance;

	int m_Width = 1280;
	int m_Height = 760;

	std::string m_Title = "BloxxEngine v0.0.1";
};
}
