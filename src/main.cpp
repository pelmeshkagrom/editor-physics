#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "hooks/EditorUI.hpp"
#include "hooks/EditorPauseLayer.hpp"
#include "EditorPhysics.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	listenForSettingChanges<float>("world-gravity", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) {
			physicsManager->m_settings.gravity = value;
			physicsManager->cleanWorld();
			physicsManager->initWorld();
		}
	});
	listenForSettingChanges<int>("physics-refresh-rate", [](int value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.timeStep = 1.0f / value;
	});

	listenForSettingChanges<float>("friction", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.friction = value;
	});
	listenForSettingChanges<float>("restitution", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.restitution = value;
	});
	listenForSettingChanges<float>("linear-damping", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.linearDamping = value;
	});
	listenForSettingChanges<float>("angular-damping", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.angularDamping = value;
	});

	listenForSettingChanges<bool>("enable-random-impulse", [](bool value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.enableRandImpulse = value;
	});
	listenForSettingChanges<float>("random-impulse-base", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.randImpulseBase = value;
	});
	listenForSettingChanges<float>("random-impulse-spread", [](float value) {
		if(auto physicsManager = EditorPhysics::get()) physicsManager->m_settings.randImpulseSpread = value;
	});

	listenForSettingChanges<std::string>("editor-button-position", [](std::string value) {
		if(auto physicsManager = EditorPhysics::get()) {
			physicsManager->m_settings.editorBtnPos = value;
			if(auto editorUI = EditorUI::get()) static_cast<EPEditorUI*>(editorUI)->updateBtn();
		}
	});
	listenForSettingChanges<bool>("show-settings-button", [](bool value) {
		if(auto physicsManager = EditorPhysics::get()) {
			physicsManager->m_settings.showSettingsBtn = value;
			if(auto scene = CCScene::get()) {
				if(auto editorPauseLayer = scene->getChildByType<EditorPauseLayer*>(0)) {
					static_cast<EPEditorPauseLayer*>(editorPauseLayer)->updateBtn();
				}
			}
		}
	});
}