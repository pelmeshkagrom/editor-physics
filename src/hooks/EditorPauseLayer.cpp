#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "EditorPauseLayer.hpp"
#include "../EditorPhysics.hpp"

using namespace geode::prelude;

bool EPEditorPauseLayer::init(LevelEditorLayer* layer) {
	if(!EditorPauseLayer::init(layer)) return false;

	if(EditorPhysics::get()->isSimulating()) {
		this->scheduleOnce(schedule_selector(EPEditorPauseLayer::stopPhysics), 0.0f);
	}

	this->updateBtn();

	return true;
}

void EPEditorPauseLayer::updateBtn() {
	if(auto topMenu = this->getChildByID("top-menu")) {
		if(EditorPhysics::get()->m_settings.showSettingsBtn) {
			auto openModSettingsBtn = CCMenuItemSpriteExtra::create(
				CCSprite::create("settings-button.png"_spr),
				this,
				menu_selector(EPEditorPauseLayer::onOpenModSettings)
			);
			openModSettingsBtn->setID("editor-physics-settings-button"_spr);
			topMenu->addChild(openModSettingsBtn);
			topMenu->updateLayout();
		} else if(auto btn = topMenu->getChildByID("editor-physics-settings-button"_spr)) {
			topMenu->removeChild(btn);
			topMenu->updateLayout();
		}
	}
}

void EPEditorPauseLayer::onOpenModSettings(CCObject* sender) {
	openSettingsPopup(Mod::get());
}

void EPEditorPauseLayer::stopPhysics(float dt) {
	auto physicsManager = EditorPhysics::get();
	if(physicsManager->isSimulating()) physicsManager->stopPhysics();
}