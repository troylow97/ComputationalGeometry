#pragma once
#define BT_THREADSAFE 1
/* [Precompiled Header Notes]
 * 1. To speed up build times, add headers that
 *    are infrequently changed into this file
 *
 * 2. #include "pch.h" should only be added to source files (.cpp)
 *    and not header files (.h)
 */


 // OpenGL
#include <GL/glew.h>

// ImGui
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>

// ImGui Addons
#include <ImGuiExtraStyles.h>

// GLM
#pragma warning(push)
#pragma warning(disable:4201) // Disable nameless struct warning from glm::quat
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.inl>
#pragma warning(pop)

// MSVC++ Run-Time Type Information
#include <typeinfo>
#include <typeindex>

// Standard Libraries
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <utility>
#include <functional>

// STL Strings
#include <string>
#include <sstream>

// STL Container
#include <vector>
#include <array>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <thread>
#include <unordered_map>
#include <deque>

// Debugging
//#include "Frameworks/Logger/Log.h"


// Graphics

// Utilities
#include "Mathf/Mathf.h"

