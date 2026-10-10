#include <Geode/Geode.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPEnhancedGameObject, EnhancedGameObject) {
	void deactivateObject(bool deactivate) {
		if(LevelEditorLayer::get() != nullptr) {
			auto physicsManager = EditorPhysics::get();
			if(physicsManager->isSimulating() && physicsManager->isObjectSimulating(this)) return;
		}
		EnhancedGameObject::deactivateObject(deactivate);
	}
};