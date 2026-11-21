#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

#include "EditorUI.hpp"
#include "../EditorPhysics.hpp"

using namespace geode::prelude;

bool EPEditorUI::init(LevelEditorLayer* editorLayer) {
	if(!EditorUI::init(editorLayer)) return false;

	this->updateBtn();

	this->addEventListener(
		KeybindSettingPressedEventV3(Mod::get(), "toggle-physics"),
		[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
			if (down && !repeat) {
				if(auto handlers = CCKeyboardDispatcher::get()->m_pHandlers) {
					if(handlers->count() > 0) {
						auto lastDelegate = static_cast<CCKeyboardHandler*>(handlers->lastObject())->getDelegate();
						if(typeinfo_cast<EditorUI*>(lastDelegate) || typeinfo_cast<EditorWindowPopup*>(lastDelegate)) {
							EditorPhysics::get()->togglePhysics();
						}
					}
				}
			}
		}
	);

	return true;
}

void EPEditorUI::updateBtn() {
	auto physicsManager = EditorPhysics::get();

	for(const char* menuID : {"playback-menu", "undo-menu", "settings-menu"}) {
		if(auto menu = this->getChildByID(menuID)) {
			if(auto btn = menu->getChildByID("toggle-physics-button"_spr)) {
				this->m_uiItems->removeObject(btn);
				physicsManager->setToggleBtn(nullptr);
				menu->removeChild(btn);
				menu->updateLayout();
				break;
			}
		}
	}

	CCNode* targetMenu = nullptr;
	if(physicsManager->m_settings.editorBtnPos == "Playback Menu") {
		targetMenu = this->getChildByID("playback-menu");
	} else if(physicsManager->m_settings.editorBtnPos == "Top Left Menu") {
		targetMenu = this->getChildByID("undo-menu");
	} else if(physicsManager->m_settings.editorBtnPos == "Top Right Menu") {
		targetMenu = this->getChildByID("settings-menu");
	}

	if(targetMenu) {
		auto togglePhysicsBtn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("start-physics.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Small),
			this,
			menu_selector(EPEditorUI::onTogglePhysics)
		);
		togglePhysicsBtn->setID("toggle-physics-button"_spr);
		targetMenu->addChild(togglePhysicsBtn);
		targetMenu->updateLayout();
		physicsManager->setToggleBtn(togglePhysicsBtn);
		this->m_uiItems->addObject(togglePhysicsBtn);
	}
}

void EPEditorUI::onTogglePhysics(CCObject* sender) {
	EditorPhysics::get()->togglePhysics();
}

void EPEditorUI::startFreeMoveTouch() {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::startFreeMoveTouch();
}

void EPEditorUI::moveObjectCall(EditCommand command) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::moveObjectCall(command);
}

void EPEditorUI::transformObjectCall(EditCommand command) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::transformObjectCall(command);
}

void EPEditorUI::activateRotationControl(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateRotationControl(sender);
}

void EPEditorUI::activateScaleControl(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateScaleControl(sender);
}

void EPEditorUI::activateTransformControl(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::activateTransformControl(sender);
}

void EPEditorUI::undoLastAction(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::undoLastAction(sender);
}

void EPEditorUI::redoLastAction(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::redoLastAction(sender);
}

void EPEditorUI::onDelete(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDelete(sender);
}

void EPEditorUI::onDeleteSelected(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDeleteSelected(sender);
}

void EPEditorUI::onDeleteSelectedType(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDeleteSelectedType(sender);
}

bool EPEditorUI::onCreate() {
	if(EditorPhysics::get()->isSimulating()) return true;
	return EditorUI::onCreate();
}

void EPEditorUI::onPaste(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onPaste(sender);
}

void EPEditorUI::onDuplicate(CCObject* sender) {
	if(EditorPhysics::get()->isSimulating()) return;
	EditorUI::onDuplicate(sender);
}

void EPEditorUI::onPlaytest(CCObject* sender) {
	auto physicsManager = EditorPhysics::get();
	if(physicsManager->isSimulating()) physicsManager->stopPhysics();
	EditorUI::onPlaytest(sender);
}

void EPEditorUI::updateButtons() {
	EditorUI::updateButtons();
	if(EditorPhysics::get()->isSimulating()) EditorPhysics::get()->disableEditorButtons();
}