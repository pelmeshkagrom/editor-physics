#pragma once

#include <Geode/modify/EditorPauseLayer.hpp>

class $modify(EPEditorPauseLayer, EditorPauseLayer) {
	bool init(LevelEditorLayer* layer);
	void updateBtn();
	void onOpenModSettings(CCObject* sender);
	void stopPhysics(float dt);
};