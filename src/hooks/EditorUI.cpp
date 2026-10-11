#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPEditorUI, EditorUI) {
	bool init(LevelEditorLayer* editorLayer) {
		if(!EditorUI::init(editorLayer)) return false;

		auto physicsManager = EditorPhysics::get();
		if(physicsManager->getBtnEnabled()) {
			if(auto settingsMenu = (physicsManager->getBtnPos() == "Top Left Menu") ? this->getChildByID("undo-menu") : this->getChildByID("settings-menu")) {
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

		return true;
	}

	void onTogglePhysics(cocos2d::CCObject* sender) {
		EditorPhysics::get()->togglePhysics();
	}

	void moveObjectCall(EditCommand command) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::moveObjectCall(command);
	}

	void transformObjectCall(EditCommand command) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::transformObjectCall(command);
	}

	void activateRotationControl(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::activateRotationControl(sender);
	}

	void activateScaleControl(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::activateScaleControl(sender);
	}

	void activateTransformControl(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::activateTransformControl(sender);
	}

	void undoLastAction(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::undoLastAction(sender);
	}

	void redoLastAction(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::redoLastAction(sender);
	}

	void onDeleteSelected(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::onDeleteSelected(sender);
	}

	void onDeleteSelectedType(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::onDeleteSelectedType(sender);
	}

	void onCreateObject(int id) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::onCreateObject(id);
	}

	void onPaste(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::onPaste(sender);
	}

	void onDuplicate(cocos2d::CCObject* sender) {
		if(EditorPhysics::get()->isSimulating()) return;
		EditorUI::onDuplicate(sender);
	}

	void updateButtons() {
		EditorUI::updateButtons();
		if(EditorPhysics::get()->isSimulating()) EditorPhysics::get()->disableEditorButtons();
	}
};