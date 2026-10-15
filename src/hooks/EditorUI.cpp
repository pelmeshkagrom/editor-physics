#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "EditorUI.hpp"
#include "../EditorPhysics.hpp"

using namespace geode::prelude;

bool EPEditorUI::init(LevelEditorLayer* editorLayer) {
	if(!EditorUI::init(editorLayer)) return false;

	this->updateBtn();

	return true;
}

void EPEditorUI::updateBtn() {
	auto physicsManager = EditorPhysics::get();
	for(const char* menuID : {"undo-menu", "settings-menu"}) {
		if(auto menu = typeinfo_cast<CCMenu*>(this->getChildByID(menuID))) {
			if(auto btn = menu->getChildByID("toggle-physics-button")) {
				this->m_uiItems->removeObject(btn);
				physicsManager->setToggleBtn(nullptr);
				menu->removeChild(btn, true);
			}
		}
	}
	if(auto settingsMenu = (physicsManager->m_settings.editorBtnPos == "Top Left Menu") ? this->getChildByID("undo-menu") : this->getChildByID("settings-menu")) {
		auto togglePhysicsBtn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("start-physics.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Small),
			this,
			menu_selector(EPEditorUI::onTogglePhysics)
		);
		togglePhysicsBtn->setID("toggle-physics-button");
		settingsMenu->addChild(togglePhysicsBtn);
		settingsMenu->updateLayout();
		physicsManager->setToggleBtn(togglePhysicsBtn);
		this->m_uiItems->addObject(togglePhysicsBtn);
	}
}

void EPEditorUI::onTogglePhysics(cocos2d::CCObject* sender) {
	EditorPhysics::get()->togglePhysics();
}

void EPEditorUI::moveObjectCall(EditCommand command) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::moveObjectCall(command);
}

void EPEditorUI::transformObjectCall(EditCommand command) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::transformObjectCall(command);
}

void EPEditorUI::activateRotationControl(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateRotationControl(sender);
}

void EPEditorUI::activateScaleControl(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateScaleControl(sender);
}

void EPEditorUI::activateTransformControl(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateTransformControl(sender);
}

void EPEditorUI::undoLastAction(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::undoLastAction(sender);
}

void EPEditorUI::redoLastAction(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::redoLastAction(sender);
}

void EPEditorUI::onDeleteSelected(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDeleteSelected(sender);
}

void EPEditorUI::onDeleteSelectedType(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDeleteSelectedType(sender);
}

void EPEditorUI::onCreateObject(int id) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onCreateObject(id);
}

void EPEditorUI::onPaste(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onPaste(sender);
}

void EPEditorUI::onDuplicate(cocos2d::CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDuplicate(sender);
}

void EPEditorUI::updateButtons() {
	EditorUI::updateButtons();
	if(EditorPhysics::get()->isSimulating()) EditorPhysics::get()->disableEditorButtons();
}