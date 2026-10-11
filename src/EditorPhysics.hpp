#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/cocos.hpp>
#include <box2d/box2d.h>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_set>

using namespace geode::prelude;

constexpr float PTM_RATIO = 30.0f;
constexpr float groundHeight = 89.78f / PTM_RATIO;
constexpr float collisionOffset = 0.5f / PTM_RATIO;
constexpr float staticObjectFriction = 0.5f;
constexpr float staticObjectRestitution = 0.1f;

enum class PhysicsObjectType {
	DynamicObject,
	StaticObject
};

class EditorPhysics : public cocos2d::CCObject {
private:
	float m_timeStep = 1.0f / 60.0f;
	float m_timeAccumulator = 0.0f;
	struct Settings {
		float gravity = 10.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float linearDamping = 0.3f;
		float angularDamping = 0.5f;
		bool enableEditorBtn = true;
		std::string editorBtnPos = "Top Left Menu";
	};
	Settings m_settings;
	void loadSettings();

	const std::unordered_set<std::uint16_t> SPIKE_IDS = {8, 39, 103, 392, 216, 217, 218, 458, 144, 205, 145, 459, 177, 178, 179};
	const std::unordered_set<std::uint16_t> CIRCLE_IDS = {1705, 1706, 1707, 1708, 1709, 1710, 1734, 1735, 1736, 186, 187, 188, 740, 741, 742, 678, 679, 680, 1619, 1620, 183, 184, 1701, 1702, 3611};
	const std::unordered_set<std::uint16_t> RECT_IDS = {3610, 1722, 1725, 1726};
	const std::unordered_set<std::uint16_t> SLOPE_EXCLUDE_IDS = {1717, 1718, 1723, 1724};

	static inline EditorPhysics* s_instance = nullptr;
	b2World* m_world = nullptr;
	bool m_isSimulating = false;
	bool m_isPaused = false;

	std::vector<b2Body*> m_dynamicBodies;
	std::unordered_map<GameObject*, b2Body*> m_dynamicMap;

	CCMenuItemSpriteExtra* m_togglePhysicsBtn = nullptr;

	void initWorld();
	void cleanMaps();
	void cleanWorld();
	bool isObjTypeSupported(GameObject* obj);
	b2Body* createBodyForObj(GameObject* obj, PhysicsObjectType type);
	void applyPhysics();
	void startSchedule();
	void stopSchedule();
	void update(float dt);
	void setBtnSprite(bool isRunning);
public:
	EditorPhysics() {
		loadSettings();
		initWorld();
	}
	~EditorPhysics() {
		stopSchedule();
		cleanMaps();
		cleanWorld();
		s_instance = nullptr;
	}
	static EditorPhysics* get();
	bool getBtnEnabled();
	std::string getBtnPos();
	void setToggleBtn(CCMenuItemSpriteExtra* btn);
	void togglePhysics();
	void stopPhysics();
	bool isSimulating();
	bool isObjectSimulating(GameObject* obj);
	void disableEditorButtons();
	void setPaused(bool paused);
};