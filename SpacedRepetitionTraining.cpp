#include "pch.h"
#include "SpacedRepetitionTraining.h"


BAKKESMOD_PLUGIN(SpacedRepetitionTraining, "Add spaced repetition to training packs", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void SpacedRepetitionTraining::onLoad()
{
	_globalCvarManager = cvarManager;
	//cvarManager->log("Plugin loaded!");

	cvarManager->registerCvar("spacedRepetitionEnabled", "1", "Enable spaced repetition plugin", true, false, 0, false, 0, true).bindTo(pluginEnabled);


	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameEvent_TrainingEditor_TA.LoadRound", [this](ActorWrapper cw, void* params, std::string eventName) {
		//cvarManager->log("customVarianceEnabled:" + *customVarianceEnabled);
		//LOG("LoadRound");
		if (!*pluginEnabled)
			return;
		loadTrainingPack();
		});

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function ProjectX.EngineShare_X.EventPostLoadMap", [this](ActorWrapper cw, void* params, std::string eventName) {
		//cvarManager->log("customVarianceEnabled:" + *customVarianceEnabled);
		//LOG("LoadRound");
		if (!*pluginEnabled)
			return;
		GfxDataTrainingWrapper  trainingData = gameWrapper->GetGfxTrainingData();
		if (!trainingData) {
			cvarManager->log("failed loading trainingData");
			return;
		}

		cvarManager->log(fmt::format("TotalRound:{0} CurrentRound:{1}", trainingData.GetTotalRounds(), trainingData.GetCurrentRound()));

		TrainingEditorWrapper trainingWrapper = (TrainingEditorWrapper)cw.memory_address;
		if (!trainingWrapper)
			return;
		
		//void SwitchToRoundNumber(int RoundNumber, unsigned long BackupCurrentRound);
		trainingData.StartAtPlaylistIndex(3);
		//trainingWrapper.SetShotAttempt(3);

		});
	

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GameEvent_TrainingEditor_TA.Destroyed", [this](ActorWrapper cw, void* params, std::string eventName) {
		if (*pluginEnabled && loaded) {
			onUnload();
		}

		});
}

void SpacedRepetitionTraining::onUnload()
{
		if (!loaded)
			return;
		loaded = false;
		//LOG("Unhooking");
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
		gameWrapper->UnhookEvent("Function TAGame.Ball_TA.RecordCarHit");
		gameWrapper->UnhookEvent("Function TAGame.Ball_TA.Explode");
		gameWrapper->UnhookEventPost("Function Engine.Controller.Restart");
		gameWrapper->UnregisterDrawables();
}

void SpacedRepetitionTraining::loadTrainingPack()
{
	//LOG(fmt::format("Loading loaded:{0}", loaded ? 1 : 0));

	if (loaded)
		return;
	loaded = true;
	resetEverything();


	/*
	if (!gameWrapper->IsInCustomTraining()) {
		cvarManager->log("Not in custom training");
		return;
	}
	*/

	GfxDataTrainingWrapper  trainingData = gameWrapper->GetGfxTrainingData();
	if (!trainingData ) {
		cvarManager->log("failed loading trainingData");
		return;
	}

	cvarManager->log(fmt::format("TotalRound:{0} CurrentRound:{1}", trainingData.GetTotalRounds(),trainingData.GetCurrentRound()));
	
	gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode",
		[this](std::string eventName) {
			//LOG("Explode DETECTED");
		//	cvarManager->log("Explode detected");
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
			//	cvarManager->log("Explode detected");
			if (!isGoal) {
				isExploded = true;
				isGoal = false;
			}
		});

	gameWrapper->HookEvent("Function TAGame.Ball_TA.OnHitGoal",
		[this](std::string eventName) {
			//	LOG("OnHitGoal DETECTED");
			//	cvarManager->log("Goal detected");
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
			//	cvarManager->log("Goal detected");
			isExploded = false;
			isGoal = true;
		});


	gameWrapper->HookEventPost("Function Engine.Controller.Restart",
		[this](std::string eventName) {
			if (!*pluginEnabled || !loaded || !gameWrapper->IsInCustomTraining())
				return;
			//LOG(fmt::format("Restart DETECTED isFirst:{0}", isFirst?1:0));
			if (isFirst) {
				isFirst = false;
				return;
			}
			handleRestart();
	});




	//gameWrapper->RegisterDrawable(bind(&TrainingSlowMo::renderSpeed, this, std::placeholders::_1));
}

void SpacedRepetitionTraining::resetEverything() {
	isFirst = true;
	isGoal = false;
	isExploded = false;
		 
}

void SpacedRepetitionTraining::renderStats(CanvasWrapper canvas) {
	bool training = gameWrapper->IsInCustomTraining();

	if (!*pluginEnabled || !loaded || !training) return;

	float screenWidth = canvas.GetSize().X;
	float screenHeight = canvas.GetSize().Y;

	float mid = -1.1;
	int range = 200;
	//int relLocation = (-1 * attempt.positionY) + range;
	int totalUnits = range * 2;



	float opacity = 1.0;
	Vector2 reqSize = Vector2{ (int)(screenWidth * 70 / 100.f), (int)(screenHeight * 4 / 100.f) };
	int unitWidth = reqSize.X / totalUnits;
	Vector2 boxSize = Vector2{ unitWidth * totalUnits, reqSize.Y };
	Vector2 startPos = Vector2{ (int)((screenWidth / 2) + (boxSize.X / 2)), (int)(screenHeight * 10 / 100.f) };


	int positionModifyer = -60;

	
}
void SpacedRepetitionTraining::setNextShot(int shotNum) {
	printDebug(fmt::format("Setting shot:{0}", shotNum));
	cvarManager->executeCommand(fmt::format("sv_training_set {0}", shotNum));
}

void SpacedRepetitionTraining::printDebug(string msg) {
	if (debugMode) {
		cvarManager->log(msg);
	}
}
void SpacedRepetitionTraining::initShotsArray(int shotsCount) {
	numOfShots = shotsCount;
	shots = new Shot[numOfShots];
	

	// Initialize each shot
	for (int i = 0; i < numOfShots; ++i) {
		// Example initialization values, you can modify this according to your requirements
		shots[i] = Shot(i + 1, 0, 0,false); // Just an example, you should replace these values with your own
	}
}

void SpacedRepetitionTraining::populateCurrBatch() {
	for (int i = 0; i < numOfShots; ++i) {
		if (shots[i].counter == 0) {
			currBatch.push_back(shots[i]);
		}
	}
}
Shot SpacedRepetitionTraining::getNextShot(bool toRemove) {
	Shot nextShot = currBatch.front();
	// Remove the shot from the current batch if toRemove is true
	if (toRemove) {
		currBatch.pop_front();
	}
	return nextShot;
}
}
void SpacedRepetitionTraining::randomizeCurrBatch() {
	currBatch.sort([](const Shot& a, const Shot& b) {
			return rand() < RAND_MAX / 2; // simple randomization
	});
}

void SpacedRepetitionTraining::shotSucceed(Shot* shot) {

}
void SpacedRepetitionTraining::shotFailed(Shot* shot) {

}
void SpacedRepetitionTraining::handleRestart() {

}