#pragma once

#include <Geode/modify/EditorUI.hpp>

class $modify(EPEditorUI, EditorUI) {
	bool init(LevelEditorLayer* editorLayer);
	void updateBtn();
	void onTogglePhysics(CCObject* sender);
	void startFreeMoveTouch();
	void moveObjectCall(EditCommand command);
	void transformObjectCall(EditCommand command);
	void activateRotationControl(CCObject* sender);
	void activateScaleControl(CCObject* sender);
	void activateTransformControl(CCObject* sender);
	void undoLastAction(CCObject* sender);
	void redoLastAction(CCObject* sender);
	void onDelete(CCObject* sender);
	void onDeleteSelected(CCObject* sender);
	void onDeleteSelectedType(CCObject* sender);
	bool onCreate();
	void onPaste(CCObject* sender);
	void onDuplicate(CCObject* sender);
	void onPlaytest(CCObject* sender);
	void updateButtons();
};