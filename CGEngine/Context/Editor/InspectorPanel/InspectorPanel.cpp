#include "pch.h"
#include "Context/ContextManager.h"
#include "InspectorPanel.h"

InspectorPanel::InspectorPanel(ContextManager& contextManager) :
	m_ContextManager{contextManager}
{}

void InspectorPanel::DrawPanel()
{
	int numOfPoints = m_ContextManager.GetShapeContext().m_NumOfPoints;
	if (numOfPoints <= 0)
	{
		ImGui::InputInt("Set Number Of Points", &m_ContextManager.GetShapeContext().m_NumOfPoints);
	}
	else
	{
		std::string AddPoint = "Add Point " + 1;
		if (ImGui::BeginMenu(AddPoint.c_str()))
		{
			static bool enabled = true;
			ImGui::MenuItem("Enabled", "", &enabled);
			static float x = 0;
			ImGui::InputFloat("x Coordinate", &x, 0.01f, 1.0f, "%.3f");
			static float y = 0;
			ImGui::InputFloat("y Coordinate", &y, 0.01f, 1.0f, "%.3f");
			ImGui::EndMenu();
		}

	}

}

void InspectorPanel::DisplayAssetInspector() const
{

}
