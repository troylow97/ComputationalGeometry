#pragma once

class ContextManager;
class Scene;
class SceneContext;

class EditorContext
{

public:
	// constructor for EditorContext
	EditorContext(ContextManager& ctxMgr);
	// destructor for EditorContext
	~EditorContext();

	// creates an EditorContext, returns FALSE if context creation fails
	bool CreateContext();

	// starts a new editor frame
	void BeginEditorFrame();

	// Renders the ImGui Widgets
	void RenderWidgets();

	// ends and renders the editor frame
	void EndEditorFrame();

	// Member Reference Getters
	SceneContext& GetSceneContext() const { return m_SceneContext; }

	glm::vec2 GetSceneViewportSize() const { return glm::vec2{ m_SceneViewportSize.x ,m_SceneViewportSize.y }; }
	glm::vec2 GetSceneViewportPosition() const { return glm::vec2{ m_SceneViewportPos.x ,m_SceneViewportPos.y }; } // Top-left (Y-Inverted)

	glm::vec2 GetGameViewportSize() const { return glm::vec2{ m_GameViewportSize.x ,m_GameViewportSize.y }; }
	glm::vec2 GetGameViewportPosition() const { return glm::vec2{ m_GameViewportPos.x ,m_GameViewportPos.y }; } // Top-left (Y-Inverted)
	glm::vec2 GetGameViewportMinPosition() const { return glm::vec2{ m_GameViewportMinPos.x ,m_GameViewportMinPos.y }; } // Bot-left (Y-Inverted)
	glm::vec2 GetGameViewportMaxPosition() const { return glm::vec2{ m_GameViewportMaxPos.x ,m_GameViewportMaxPos.y }; } // Top-right (Y-Inverted)

	// Is the mouse hovered over Scene window?
	bool IsSceneViewportHovered() const { return m_EditorSceneHovered; }

	// Is the Scene window visible?
	bool IsSceneViewportVisible() const { return m_EditorSceneVisible; }

	// Is the mouse hovered over Game window?
	bool IsGameViewportHovered() const { return m_EditorGameHovered; }

	// Is the Game window visible?
	bool IsGameViewportVisible() const { return m_EditorGameVisible; }

	void HandleInput(const InputContext&, std::shared_ptr<Scene>);

private:
	// Context References
	ContextManager& m_ContextManager;
	SceneContext& m_SceneContext;

	// Is the mouse hovered over Scene/Game view window?
	bool m_EditorSceneHovered{}, m_EditorSceneVisible{}, m_EditorGameHovered{}, m_EditorGameVisible{};

	// Viewport Sizes
	ImVec2 m_SceneViewportSize, m_SceneViewportPos;
	ImVec2 m_GameViewportSize, m_GameViewportPos, m_GameViewportMinPos, m_GameViewportMaxPos;


	// Global Dock IDs
	ImGuiID m_HierarchyDockID{}, m_SceneDockID{}, m_InspectorDockID{}, m_ResourceDockID{}, m_ConsoleDockID{}, m_ExecutionBarID{};

	// Set the DockSpace window flags, styling and filling of viewport
	// Returns the window flag settings
	ImGuiWindowFlags SetDockSpace();

	// Draws Scene & Game Viewports
	void ShowMainMenu();
	void ShowPlaneSweepMenu();
	void ShowConvexHullMenu();

	void AddRandomPointsForConvexHull(ScenePtr);
	void AddRandomPointsForLineSegment(ScenePtr);

	void ShowCoordinatesBesideMouse();

	bool AddPointUsingMouseToggled;
	bool DeletePointUsingMouseToggled;
	bool AddLineSegmentUsingMouseToggled;
	bool DeleteLineSegmentUsingMouseToggled;
	std::vector<glm::vec2> AddLineSegmentUsingMouseVertexList;
	int m_RandomAmountForConvexHull;
	int m_RandomAmountForLineSegment;
	
	float x;
	float y;
	float x1;
	float y1;
	float x2;
	float y2;
};
