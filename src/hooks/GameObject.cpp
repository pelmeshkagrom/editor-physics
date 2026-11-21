#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPGameObject, GameObject) {
	void deactivateObject(bool deactivate) {
		if(LevelEditorLayer::get() && EditorPhysics::get()->isObjectSimulating(this)) return;
		GameObject::deactivateObject(deactivate);
	}
};