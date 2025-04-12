#include "pch.h"
#include "Context/ContextManager.h"
#include <ImGuizmo.h>
#include <ImCurveEdit.h>
#include <imgui_impl_glfw.h>
#include "Algorithms/Algorithms.h"

EditorContext::EditorContext(ContextManager& ctxMgr) :
	m_ContextManager{ ctxMgr },
	m_SceneContext{ ctxMgr.GetSceneContext() },
	x{ 0.0 },
	y{ 0.0 },
	x1{ 0.0 },
	y1{ 0.0 },
	x2{ 0.0 },
	y2{ 0.0 }
{
}

EditorContext::~EditorContext() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool EditorContext::CreateContext() {
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	std::cout << "Initialized ImGui Version: " << ImGui::GetVersion() << std::endl;

	// ImGui configurations
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr; // disable .ini file creation
	io.IniSavingRate = -1;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard // enable keyboard controls
		| ImGuiConfigFlags_DockingEnable      // enable docking
		| ImGuiConfigFlags_ViewportsEnable    // enable multi-viewport
		| ImGuiConfigFlags_NoMouseCursorChange; // Disable cursor override 

	// Platform bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true))
		return false;

	if (!ImGui_ImplOpenGL3_Init(nullptr))
		return false;

	std::cout << Messages::EDITOR_CONTEXT_CREATION_SUCCESS << std::endl;

	return true;
}

void EditorContext::BeginEditorFrame() {
	// begins editor frame	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void EditorContext::RenderWidgets() {
	ImGuiWindowFlags window_flags = SetDockSpace();
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ShowMainMenu();
	ImGui::End();
}

void EditorContext::EndEditorFrame() {
	// Ends editor frame
	GLFWwindow* currentContext = glfwGetCurrentContext();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(currentContext, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(currentContext);
}

ImGuiWindowFlags EditorContext::SetDockSpace() {
	// DockSpace window flags
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// Set DockSpace to fill viewport
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	// DockSpace Styling
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	return window_flags;
}

void EditorContext::ShowConvexHullMenu() {
	ScenePtr scenePtr = m_ContextManager.GetSceneContext().GetActiveScene();
	//Display Points
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Original Polygon:");
	ImGui::PopStyleColor();
	for (int i = 0; i < scenePtr->GetShapePolygon().getNumOfVertices(); ++i) {
		std::stringstream ss;
		ss << scenePtr->GetShapePolygon().getVertexList()[i].x << "," << scenePtr->GetShapePolygon().getVertexList()[i].y << std::endl;
		ImGui::Text(ss.str().c_str());
	}

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Convex Hull:");
	ImGui::PopStyleColor();
	for (int i = 0; i < scenePtr->GetConvexHull().getNumOfVertices(); ++i) {
		std::stringstream ss;
		ss << scenePtr->GetConvexHull().getVertexList()[i].x << "," << scenePtr->GetConvexHull().getVertexList()[i].y << std::endl;
		ImGui::Text(ss.str().c_str());
	}

	ImGui::Separator();

	//Float Input
	ImGui::PushID("Input Point");
	ImGui::PushID("Push Coord x"); ImGui::Text("x: "); ImGui::SameLine(); ImGui::InputFloat("", &x); ImGui::PopID();
	ImGui::PushID("Push Coord y"); ImGui::Text("y: "); ImGui::SameLine(); ImGui::InputFloat("", &y); ImGui::PopID();
	ImGui::PopID();																				   

	ImGui::Separator();

	//Add Point Button
	if (ImGui::Button("Add Point")) {
		scenePtr->GetShapePolygon().AddVertex({x, y});
	}

	ImGui::Separator();

	//Delete Point Button
	if (ImGui::Button("Delete Point")) {
		scenePtr->GetShapePolygon().RemoveVertex({ x, y });
	}

	ImGui::Separator();

	// Clear Point Button
	if (ImGui::Button("Clear All Points")) {
		scenePtr->GetShapePolygon().clearVertices();
		scenePtr->GetConvexHull().clearVertices();
	}

	ImGui::Separator();

	//Form Convex Hull
	if (ImGui::Button("Simulate Convex Hull")) {
		scenePtr->SetConvexHullSimulationCompleted(false);
	}

	ImGui::Separator();

	ImGui::DragInt("Simulation Wait Time (ms)", &scenePtr->GetSimulationWait(), 1, 0, 10000);

	if (scenePtr->GetSimulationWait() > scenePtr->GetMaxSimulationWait() || scenePtr->GetSimulationWait() < 0)
		scenePtr->GetSimulationWait() = scenePtr->GetMaxSimulationWait();

	ImGui::Separator();

	ImGui::Checkbox("Render Convex Hull", &scenePtr->GetRenderConvexHull());
	ImGui::Checkbox("Render only Convex Hull points", &scenePtr->GetRenderOnlyConvexHullPoints());
}

void EditorContext::ShowPlaneSweepMenu() {
	ScenePtr scenePtr = m_ContextManager.GetSceneContext().GetActiveScene();
	//Display Points
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Line Segments:");
	ImGui::PopStyleColor();
	for (int i = 0; i < scenePtr->m_LineSegments.size(); ++i) {
		std::stringstream ss;
		ss << "{" << scenePtr->m_LineSegments[i].GetBottom().x << "," << scenePtr->m_LineSegments[i].GetBottom().y << "}" << ","
			<< "{" << scenePtr->m_LineSegments[i].GetTop().x << "," << scenePtr->m_LineSegments[i].GetTop().y << "}" << std::endl;
		ImGui::Text(ss.str().c_str());
	}

	ImGui::Separator();

	//Float Input
	ImGui::PushID("Input Line");
	ImGui::PushID("Push Bot Coord x1"); ImGui::Text("x1: "); ImGui::SameLine(); ImGui::InputFloat("", &x1); ImGui::PopID();
	ImGui::PushID("Push Bot Coord y1"); ImGui::Text("y1: "); ImGui::SameLine(); ImGui::InputFloat("", &y1); ImGui::PopID();
	ImGui::PushID("Push Bot Coord x2"); ImGui::Text("x2: "); ImGui::SameLine(); ImGui::InputFloat("", &x2); ImGui::PopID();
	ImGui::PushID("Push Bot Coord y2"); ImGui::Text("y2: "); ImGui::SameLine(); ImGui::InputFloat("", &y2); ImGui::PopID();
	ImGui::PopID();

	ImGui::Separator();

	ImGui::DragInt("Simulation Wait Time (ms)", &scenePtr->GetSimulationWait(), 1, 0, 10000);

	if (scenePtr->GetSimulationWait() > scenePtr->GetMaxSimulationWait() || scenePtr->GetSimulationWait() < 0)
		scenePtr->GetSimulationWait() = scenePtr->GetMaxSimulationWait();

	ImGui::Separator();

	//Add Line Segment Button
	if (ImGui::Button("Add Line Segment"))
	{
		scenePtr->m_LineIntersections.clear();
		scenePtr->m_LineSweep_Q.clear();
		scenePtr->m_LineSweep_T.clear();

		scenePtr->m_LineSegments.push_back({ { x1,y1 }, { x2,y2 } });
	}

	ImGui::Separator();

	//Add Line Segment Button
	if (ImGui::Button("Delete Line Segment"))
	{
		scenePtr->m_LineIntersections.clear();
		scenePtr->m_LineSweep_Q.clear();
		scenePtr->m_LineSweep_T.clear();

		scenePtr->m_LineSegments.erase(std::remove_if(scenePtr->m_LineSegments.begin(), scenePtr->m_LineSegments.end(),
			[&](Shape::Line LineCompare) { return Shape::Line{ {x1,y1},{x2,y2} } == LineCompare; }), scenePtr->m_LineSegments.end());
		//scenePtr->m_LineIntersections = LineSweepAlgorithm(scenePtr->m_LineSegments);
	}

	ImGui::Separator();

	if (ImGui::Button("Clear all Line Segments"))
	{
		scenePtr->m_LineSegments.clear();
		scenePtr->m_LineSweep_Q.clear();
		scenePtr->m_LineSweep_T.clear();
		scenePtr->m_LineIntersections.clear();

		//scenePtr->m_LineIntersections = LineSweepAlgorithm(scenePtr->m_LineSegments);
	}

	ImGui::Separator();

	//Get Line Intersections
	if (ImGui::Button("Simulate Line Sweep Algo")) {
		// Set simulation bool to false to trigger Line Sweep simulation
		scenePtr->SetLineSweepSimulationCompleted(false);
	}

	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Line Segment Intersections:");
	ImGui::PopStyleColor();
	std::vector<glm::vec2> tempIntersectionVector = scenePtr->m_LineIntersections;
	for (int i = 0; i < tempIntersectionVector.size(); ++i) {
		std::stringstream ss;

		if (Mathf::SameFloat(tempIntersectionVector[i].x, FLT_MAX) || Mathf::SameFloat(tempIntersectionVector[i].y, FLT_MAX)) {
			ss << "Line Segment Collinear" << std::endl;
		}
		else {
			ss << "{" << scenePtr->m_LineIntersections[i].x << "," << scenePtr->m_LineIntersections[i].y << "}" << std::endl;
		}

		
		ImGui::Text(ss.str().c_str());
	}

	ImGui::Separator();	

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Event Q: (White Points)");
	ImGui::PopStyleColor();
	for (int i = 0; i < scenePtr->m_LineSweep_Q.size(); ++i) {
		std::stringstream ss;
		ss << "{" << scenePtr->m_LineSweep_Q[i].point.x << "," << scenePtr->m_LineSweep_Q[i].point.y << "}" << std::endl;
		ImGui::Text(ss.str().c_str());
	}

	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
	ImGui::Text("Event T: (Yellow Points)");
	ImGui::PopStyleColor();
	for (int i = 0; i < scenePtr->m_LineSweep_T.size(); ++i) {
		std::stringstream ss;
		ss << "{" << scenePtr->m_LineSweep_T[i].point.x << "," << scenePtr->m_LineSweep_T[i].point.y << "}" << std::endl;
		ImGui::Text(ss.str().c_str());
	}

	ImGui::Separator();
}

void EditorContext::ShowMainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("ConvexHull")) {
			
			ShowConvexHullMenu();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("PlaneSweep")) {
			ShowPlaneSweepMenu();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}