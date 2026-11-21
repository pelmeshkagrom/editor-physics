#pragma once
#include <Geode/Geode.hpp>
#include <box2d/box2d.h>
#include <string>
#include <vector>
#include <random>
#include <unordered_map>

using namespace geode::prelude;

enum class PhysicsObjectType {
	DynamicObject,
	StaticObject
};

class EditorPhysics : public cocos2d::CCObject {
private:
	EditorPhysics();

	std::mt19937 m_random;

	static inline EditorPhysics* s_instance = nullptr;
	b2World* m_world = nullptr;
	bool m_isSimulating = false;
	float m_timeAccumulator = 0.0f;

	std::vector<b2Body*> m_dynamicBodies;
	std::unordered_map<GameObject*, b2Body*> m_dynamicMap;

	CCMenuItemSpriteExtra* m_togglePhysicsBtn = nullptr;

	void loadSettings();
	void cleanMaps();
	bool isObjTypeSupported(GameObject* obj);
	b2Body* createBodyForObj(GameObject* obj, PhysicsObjectType type);
	void applyPhysics();
	void startSchedule();
	void stopSchedule();
	void update(float dt);
	void setBtnSprite(bool isRunning);
public:
	~EditorPhysics();

	struct Settings {
		float gravity = 10.0f;
		float timeStep = 1.0f / 60.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float linearDamping = 0.3f;
		float angularDamping = 0.5f;
		bool enableRandImpulse = false;
		float randImpulseBase = 1.0f;
		float randImpulseSpread = 1.0f;
		std::string editorBtnPos = "Playback Menu";
		bool showSettingsBtn = true;
	};
	Settings m_settings;

	static EditorPhysics* get();
	void cleanWorld();
	void initWorld();
	void setToggleBtn(CCMenuItemSpriteExtra* btn);
	void togglePhysics();
	void stopPhysics();
	bool isSimulating();
	bool isObjectSimulating(GameObject* obj);
	void disableEditorButtons();
};