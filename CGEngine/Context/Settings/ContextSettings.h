#pragma once

#include <glm/vec2.hpp>
#include <string>

struct WindowSettings
{
	static inline constexpr glm::ivec2 defaultWindowSize{ 1600,900 };
	static inline std::string windowTitle = "Computational Geometry Engine";
};

struct GraphicsSettings
{
	const std::string VERTEX_SHADER_FILE = "AppData/Shader/VertexShader.vert";
	const std::string FRAGMENT_SHADER_FILE = "AppData/Shader/FragmentShader.frag";
};