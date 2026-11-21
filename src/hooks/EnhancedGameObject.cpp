#include <Geode/Geode.hpp>
#include <Geode/modify/EnhancedGameObject.hpp>

#include "../EditorPhysics.hpp"

using namespace geode::prelude;

class $modify(EPEnhancedGameObject, EnhancedGameObject) {
	void deactivateObject(bool deactivate) {
		if(LevelEditorLayer::get() && EditorPhysics::get()->isObjectSimulating(this)) return;
		EnhancedGameObject::deactivateObject(deactivate);
	}
};