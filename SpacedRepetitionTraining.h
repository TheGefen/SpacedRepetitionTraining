#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);
using namespace std;

struct Shot {
	int count;    // Number of shots
	int level;    // Level of the shot
	int counter;  // Counter for the shot
	bool success; // Boolean indicating if the shot was successful or not


	Shot() : count(0), level(0), counter(0), success(false) {}
	// Constructor
	Shot(int c, int l, int ctr, bool ret) : count(c), level(l), counter(ctr), success(ret) {}
};

class SpacedRepetitionTraining: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	// Inherited via PluginSettingsWindow
	
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	
	shared_ptr<bool> pluginEnabled = make_shared<bool>(true);
	
	bool loaded = false;

	//Handle Attempts
	bool isGoal;
	bool isExploded;
	bool isFirst;


	bool debugMode = true;

	//Internal function
	void loadTrainingPack();
	void resetEverything();
	void renderStats(CanvasWrapper canvas);
	void handleRestart();
	void setNextShot(int i);
	void printDebug(string msg);

	//Handling shots
	Shot* shots; // Array of Shot structs
	list<Shot> currBatch;
	int numOfShots;

	void initShotsArray(int numOfShots);
	void populateCurrBatch();
	Shot getNextShot(bool toRemove);
	void randomizeCurrBatch();

	void shotSucceed(Shot* shot);
	void shotFailed(Shot* shot);

	// Inherited via PluginWindow
	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "SpacedRepetitionTraining";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;
	
	
};

