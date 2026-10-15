#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPEditorPauseLayer, EditorPauseLayer) {
	struct Fields {
		EditorPhysics* physicsManager = nullptr;
		~Fields() {
			if(physicsManager) {
				physicsManager->setPaused(false);
			}
		}
	};

	bool init(LevelEditorLayer* layer) {
		if(!EditorPauseLayer::init(layer)) return false;

		m_fields->physicsManager = EditorPhysics::get();
		if(m_fields->physicsManager) {
			m_fields->physicsManager->setPaused(true);
			if(m_fields->physicsManager->isSimulating())
				this->scheduleOnce(schedule_selector(EPEditorPauseLayer::stopPhysics), 0.0f);
		}

		if(auto bottomMenu = this->getChildByID("guidelines-menu")) {
			auto openModSettingsBtn = CCMenuItemSpriteExtra::create(
				CCSprite::create("settings-button.png"_spr),
				this,
				menu_selector(EPEditorPauseLayer::onOpenModSettings)
			);
			openModSettingsBtn->setID("editor-physics-settings-button");
			bottomMenu->addChild(openModSettingsBtn);
			bottomMenu->updateLayout();
		}

		return true;
	}

	void onOpenModSettings(cocos2d::CCObject* sender) {
		geode::openSettingsPopup(Mod::get(), true);
	}

	void stopPhysics(float dt) {
		if(m_fields->physicsManager && m_fields->physicsManager->isSimulating()) {
			m_fields->physicsManager->stopPhysics();
		}
	}
};