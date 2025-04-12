#pragma once
class ContextManager;

class InspectorPanel
{
public:
	InspectorPanel(ContextManager& ContextManager);
	void DrawPanel();

private:
	ContextManager& m_ContextManager;
	void DisplayAssetInspector() const;
};

