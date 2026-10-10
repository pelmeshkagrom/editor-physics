#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPGameObject, GameObject) {
	void deactivateObject(bool deactivate) {
		if(LevelEditorLayer::get() != nullptr) {
			auto physicsManager = EditorPhysics::get();
			if(physicsManager->isSimulating() && physicsManager->isObjectSimulating(this)) return;
		}
		GameObject::deactivateObject(deactivate);
	}
};