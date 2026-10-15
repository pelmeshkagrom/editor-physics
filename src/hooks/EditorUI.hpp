#pragma once

#include <Geode/modify/EditorUI.hpp>

class $modify(EPEditorUI, EditorUI) {
	bool init(LevelEditorLayer* editorLayer);
	void updateBtn();
	void onTogglePhysics(cocos2d::CCObject* sender);
	void moveObjectCall(EditCommand command);
	void transformObjectCall(EditCommand command);
	void activateRotationControl(cocos2d::CCObject* sender);
	void activateScaleControl(cocos2d::CCObject* sender);
	void activateTransformControl(cocos2d::CCObject* sender);
	void undoLastAction(cocos2d::CCObject* sender);
	void redoLastAction(cocos2d::CCObject* sender);
	void onDeleteSelected(cocos2d::CCObject* sender);
	void onDeleteSelectedType(cocos2d::CCObject* sender);
	void onCreateObject(int id);
	void onPaste(cocos2d::CCObject* sender);
	void onDuplicate(cocos2d::CCObject* sender);
	void updateButtons();
};