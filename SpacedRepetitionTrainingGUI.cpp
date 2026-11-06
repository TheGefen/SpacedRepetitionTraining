#include "pch.h"
#include "SpacedRepetitionTraining.h"

// Plugin Settings Window code here
std::string SpacedRepetitionTraining::GetPluginName() {
	return "SpacedRepetitionTraining";
}

void SpacedRepetitionTraining::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> SpacedRepetitionTraining
void SpacedRepetitionTraining::RenderSettings() {
	ImGui::TextUnformatted("SpacedRepetitionTraining plugin settings");


	CVarWrapper enableCvar = cvarManager->getCvar("spacedRepetitionEnabled");
	if (!enableCvar) return;
	bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable plugin", &enabled))
		enableCvar.setValue(enabled);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Enable/Disable SpacedRepetition plugin");
	ImGui::Separator();
}



// Do ImGui rendering here
void SpacedRepetitionTraining::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string SpacedRepetitionTraining::GetMenuName()
{
	return "SpacedRepetitionTraining";
}

// Title to give the menu
std::string SpacedRepetitionTraining::GetMenuTitle()
{
	return menuTitle_;
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool SpacedRepetitionTraining::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool SpacedRepetitionTraining::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void SpacedRepetitionTraining::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void SpacedRepetitionTraining::OnClose()
{
	isWindowOpen_ = false;
}

