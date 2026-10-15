#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

#include "EditorPhysics.hpp"
#include "hooks/EditorUI.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	listenForSettingChanges<float>("world-gravity", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) {
			physicsManager->m_settings.gravity = value;
			physicsManager->cleanWorld();
			physicsManager->initWorld();
		}
	});
	listenForSettingChanges<int>("physics-refresh-rate", [](int value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.timeStep = 1.0f / value;
	});

	listenForSettingChanges<float>("friction", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.friction = value;
	});
	listenForSettingChanges<float>("restitution", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.restitution = value;
	});
	listenForSettingChanges<float>("linear-damping", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.linearDamping = value;
	});
	listenForSettingChanges<float>("angular-damping", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.angularDamping = value;
	});

	listenForSettingChanges<bool>("enable-random-impulse", [](bool value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.enableRandImpulse = value;
	});
	listenForSettingChanges<float>("random-impulse-base", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.randImpulseBase = value;
	});
	listenForSettingChanges<float>("random-impulse-spread", [](float value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) physicsManager->m_settings.randImpulseSpread = value;
	});

	listenForSettingChanges<std::string>("editor-button-position", [](std::string value) {
		auto physicsManager = EditorPhysics::get();
		if(physicsManager != nullptr) {
			physicsManager->m_settings.editorBtnPos = value;
			if(auto editorUI = EditorUI::get()) static_cast<EPEditorUI*>(editorUI)->updateBtn();
		}
	});
}