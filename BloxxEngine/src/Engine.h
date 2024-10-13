#pragma once

#include <string>

namespace BE {

class Engine {
public:
	Engine(const std::string& title): m_Title(title) {}
	~Engine() = default;

	void Run();

private:
	std::string m_Title = "BloxxEngine v0.0.1";
};

}
