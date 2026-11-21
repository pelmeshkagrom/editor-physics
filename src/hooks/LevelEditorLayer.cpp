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

		return true;
	}

	void undoLastAction() {
		if(m_fields->physicsManager) {
			if(m_fields->physicsManager->isSimulating()) return;
		}
		LevelEditorLayer::undoLastAction();
	}

	void redoLastAction() {
		if(m_fields->physicsManager) {
			if(m_fields->physicsManager->isSimulating()) return;
		}
		LevelEditorLayer::redoLastAction();
	}
};