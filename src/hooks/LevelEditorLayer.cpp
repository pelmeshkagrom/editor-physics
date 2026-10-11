#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPLevelEditorLayer, LevelEditorLayer) {
	struct Fields {
		EditorPhysics* physicsManager = nullptr;
		~Fields() {
			if(physicsManager) {
				delete physicsManager;
				physicsManager = nullptr;
			}
		}
	};

	bool init(GJGameLevel* level, bool noUI) {
		if(!LevelEditorLayer::init(level, noUI)) return false;

		m_fields->physicsManager = EditorPhysics::get();

		this->addEventListener(
			KeybindSettingPressedEventV3(Mod::get(), "toggle-physics"),
			[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
				if (down && !repeat) {
					if(m_fields->physicsManager) {
						m_fields->physicsManager->togglePhysics();
					}
				}
			}
		);

		return true;
	}

	void onPlaytest() {
		if(m_fields->physicsManager) {
			if(m_fields->physicsManager->isSimulating()) m_fields->physicsManager->stopPhysics();
		}
		LevelEditorLayer::onPlaytest();
	}

	void removeObject(GameObject* object, bool noUndo) {
		if(m_fields->physicsManager) {
			if(m_fields->physicsManager->isSimulating()) return;
		}
		LevelEditorLayer::removeObject(object, noUndo);
	}
};