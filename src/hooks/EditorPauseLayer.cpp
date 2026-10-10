#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

#include "../EditorPhysics.hpp"

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

		return true;
	}

	void stopPhysics(float dt) {
		if(m_fields->physicsManager && m_fields->physicsManager->isSimulating()) {
			m_fields->physicsManager->stopPhysics();
		}
	}
};