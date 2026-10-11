#include "EditorPhysics.hpp"
#include <Geode/ui/BasedButtonSprite.hpp>
#include <cmath>
#include <algorithm>

void EditorPhysics::loadSettings() {
	m_settings.gravity = Mod::get()->getSettingValue<float>("world-gravity");
	m_timeStep = 1.0f / Mod::get()->getSettingValue<int64_t>("physics-refresh-rate");

	m_settings.friction = Mod::get()->getSettingValue<float>("friction");
	m_settings.restitution = Mod::get()->getSettingValue<float>("restitution");
	m_settings.linearDamping = Mod::get()->getSettingValue<float>("linear-damping");
	m_settings.angularDamping = Mod::get()->getSettingValue<float>("angular-damping");

	m_settings.enableEditorBtn = Mod::get()->getSettingValue<bool>("enable-editor-button");
	m_settings.editorBtnPos = Mod::get()->getSettingValue<std::string>("editor-button-position");
}

void EditorPhysics::initWorld() {
	b2Vec2 gravity(0.0f, -m_settings.gravity);
	m_world = new b2World(gravity);

	b2BodyDef groundBodyDef;
	auto groundBody = m_world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(99999.0f, 1.0f, b2Vec2(0.0f, groundHeight - 1.0f), 0.0f);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.density = 0.0f;
	groundFixtureDef.shape = &groundBox;
	groundFixtureDef.friction = staticObjectFriction;
	groundFixtureDef.restitution = staticObjectRestitution;
	groundBody->CreateFixture(&groundFixtureDef);
}

void EditorPhysics::cleanMaps() {
	m_dynamicBodies.clear();
	m_dynamicMap.clear();
}

void EditorPhysics::cleanWorld() {
	if(m_world) {
		delete m_world;
		m_world = nullptr;
	}
}

bool EditorPhysics::isObjTypeSupported(GameObject* obj) {
	switch (obj->getType()) {
		case GameObjectType::Solid:
			return true;
		case GameObjectType::Hazard:
			return true;
		case GameObjectType::Slope:
			return true;
		default:
			return false;
	}
}

b2Body* EditorPhysics::createBodyForObj(GameObject* obj, PhysicsObjectType type) {
	auto position = obj->getPosition() / PTM_RATIO;
	if(position.y <= groundHeight) return nullptr;

	auto objType = obj->getType();
	auto objID = obj->m_objectID;
	bool isRotatable = false;

	auto contentSize = obj->getScaledContentSize() / PTM_RATIO;
	auto width = contentSize.width - collisionOffset;
	auto height = contentSize.height - collisionOffset;

	b2PolygonShape polygonShape;
	b2CircleShape circleShape;
	b2Shape* chosenShape = nullptr;

	switch (objType) {
		case GameObjectType::Solid: {
			polygonShape.SetAsBox(std::abs(width) / 2.0f, std::abs(height) / 2.0f);
			chosenShape = &polygonShape;
			break;
		}
		case GameObjectType::Hazard: {
			isRotatable = true;
			if(SPIKE_IDS.contains(objID)) {
				b2Vec2 verticles[3];
				verticles[0].Set(-width / 2.0f, -height / 2.0f);
				verticles[1].Set(width / 2.0f, -height / 2.0f);
				verticles[2].Set(0.0f, height / 2.0f);
				polygonShape.Set(verticles, 3);
				chosenShape = &polygonShape;
				break;
			} else if (CIRCLE_IDS.contains(objID)) {
				auto radius = std::max(width, height) / 2.0f; // we don't have ellipse in box2d
				circleShape.m_radius = radius;
				chosenShape = &circleShape;
				break;
			} else if(RECT_IDS.contains(objID)) {
				polygonShape.SetAsBox(std::abs(width) / 2.0f, std::abs(height) / 2.0f);
				chosenShape = &polygonShape;
				break;
			}
			break;
		}
		case GameObjectType::Slope: {
			if(SLOPE_EXCLUDE_IDS.contains(objID)) break;
			b2Vec2 verticles[3];
			verticles[0].Set(-width / 2.0f, -height / 2.0f);
			verticles[1].Set(width / 2.0f, -height / 2.0f);
			verticles[2].Set(width / 2.0f, height / 2.0f);
			polygonShape.Set(verticles, 3);
			chosenShape = &polygonShape;
			break;
		}
		default: {
			return nullptr;
		}
	}

	if(!chosenShape) return nullptr;

	bool isDynamic = (type == PhysicsObjectType::DynamicObject);
	auto rotation = obj->getRotation();

	b2BodyDef bodyDef;
	bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = !isRotatable;
	bodyDef.angle = -rotation * (b2_pi / 180.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.density = isDynamic ? 1.0f : 0.0f;
	fixtureDef.shape = chosenShape;
	if(isDynamic) {
		fixtureDef.friction = m_settings.friction;
		fixtureDef.restitution = m_settings.restitution;
		bodyDef.linearDamping = m_settings.linearDamping;
		bodyDef.angularDamping = m_settings.angularDamping;
	} else {
		fixtureDef.friction = staticObjectFriction;
		fixtureDef.restitution = staticObjectRestitution;
	}

	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(obj);
	b2Body* body = m_world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	return body;
}

void EditorPhysics::applyPhysics() {
	if(!m_world || m_isSimulating || m_isPaused) return;

	auto editorLayer = LevelEditorLayer::get();
	if(!editorLayer) return;

	if(editorLayer->m_playbackMode == PlaybackMode::Playing || editorLayer->m_playbackMode == PlaybackMode::Paused) return;

	auto editorUI = EditorUI::get();
	if(!editorUI) return;

	CCArrayExt<GameObject*> allObjects = editorLayer->m_objects;
	if(allObjects.empty()) return;

	CCArrayExt<GameObject*> selectedObjects = editorUI->getSelectedObjects();
	if(selectedObjects.empty()) return;

	std::unordered_set<GameObject*> selectedSet;
	for(auto obj : selectedObjects) {
		if(obj) selectedSet.insert(obj);
	}

	bool simulationWillStart = false;

	for(auto obj : selectedObjects) {
		if(!obj || !isObjTypeSupported(obj)) continue;

		if(auto body = createBodyForObj(obj, PhysicsObjectType::DynamicObject)) {
			m_dynamicBodies.push_back(body);
			m_dynamicMap[obj] = body;
			obj->activateObject();
			simulationWillStart = true;
		}
	}

	for(auto obj : allObjects) {
		if(!obj || selectedSet.contains(obj) || !isObjTypeSupported(obj)) continue;

		createBodyForObj(obj, PhysicsObjectType::StaticObject);
	}

	if(simulationWillStart) {
		disableEditorButtons();
		editorUI->deactivateRotationControl();
		editorUI->deactivateScaleControl();
		editorUI->deactivateTransformControl();
		editorLayer->m_undoObjects->addObject(editorUI->createUndoObject(UndoCommand::Transform, true));
		setBtnSprite(true);
		startSchedule();
	}
}

void EditorPhysics::startSchedule() {
	if(m_isSimulating) return;
	m_isSimulating = true;
	CCScheduler::get()->scheduleSelector(
		schedule_selector(EditorPhysics::update),
		this,
		0,
		false
	);
}

void EditorPhysics::stopSchedule() {
	if(!m_isSimulating) return;
	CCScheduler::get()->unscheduleSelector(
		schedule_selector(EditorPhysics::update),
		this
	);
	m_isSimulating = false;
}

void EditorPhysics::update(float dt) {
	if(!m_world || m_dynamicBodies.empty()) {
		stopSchedule();
		return;
	}

	bool physicsCalculated = false;
	m_timeAccumulator += dt;
	while(m_timeAccumulator >= m_timeStep) {
		m_world->Step(m_timeStep, 8, 3);
		m_timeAccumulator -= m_timeStep;
		physicsCalculated = true;
	}

	if(!physicsCalculated) return;

	bool isBodyAwake = false;
	for(const auto body : m_dynamicBodies) {
		auto object = reinterpret_cast<GameObject*>(body->GetUserData().pointer);
		if(!object || !body) continue;

		if(body->IsAwake()) isBodyAwake = true;

		auto pos = body->GetPosition();
		auto angle = body->GetAngle();

		object->setPosition({pos.x * PTM_RATIO, pos.y * PTM_RATIO});
		object->setRotation(-angle * (180.0f / b2_pi));
	}

	if(!isBodyAwake) stopPhysics();
}

void EditorPhysics::setBtnSprite(bool isRunning) {
	if(!m_togglePhysicsBtn) return;

	if(isRunning) {
		m_togglePhysicsBtn->setSprite(CircleButtonSprite::createWithSprite("stop-physics.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Small));
	} else {
		m_togglePhysicsBtn->setSprite(CircleButtonSprite::createWithSprite("start-physics.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Small));
	}
}

EditorPhysics* EditorPhysics::get() {
	if(!s_instance && LevelEditorLayer::get() != nullptr) {
		s_instance = new EditorPhysics();
	}
	return s_instance;
}

bool EditorPhysics::getBtnEnabled() {
	return m_settings.enableEditorBtn;
}

std::string EditorPhysics::getBtnPos() {
	return m_settings.editorBtnPos;
}

void EditorPhysics::setToggleBtn(CCMenuItemSpriteExtra* btn) {
	m_togglePhysicsBtn = btn;
}

void EditorPhysics::togglePhysics() {
	if(!m_isSimulating) {
		applyPhysics();
	} else {
		stopPhysics();
	}
}

void EditorPhysics::stopPhysics() {
	stopSchedule();
	m_timeAccumulator = 0.0f;

	if(auto editorLayer = LevelEditorLayer::get()) {
		if(auto editorUI = EditorUI::get()) {
			CCArrayExt<GameObject*> allObjects = editorLayer->m_objects;
			if(!allObjects.empty()) {
				std::unordered_set<GameObject*> allSet;
				for(auto obj : allObjects) {
					if(obj) allSet.insert(obj);
				}

				for(const auto& [object, body] : m_dynamicMap) {
					if(!object) continue;
					if(allSet.contains(object)) editorUI->moveObject(object, {0, 0});
				}
			}

			editorUI->updateButtons();
		}
	}

	cleanMaps();
	cleanWorld();
	initWorld();
	setBtnSprite(false);
}

bool EditorPhysics::isSimulating() {
	return m_isSimulating;
}

bool EditorPhysics::isObjectSimulating(GameObject* obj) {
	return m_dynamicMap.contains(obj);
}

void EditorPhysics::disableEditorButtons() {
	if(auto editorUI = EditorUI::get()) {
		if(auto menu = editorUI->getChildByID("undo-menu")) {
			for(const char* btnID : {"undo-button", "redo-button", "delete-trash-button"}) {
				if(auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID(btnID))) {
					btn->setColor(ccColor3B(166, 166, 166));
					btn->setOpacity(175);
					btn->m_animationEnabled = false;
				}
			}
		}
		if(auto menu = editorUI->getChildByID("editor-buttons-menu")) {
			for(const char* btnID : {"paste-button", "copy-paste-button"}) {
				if(auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID(btnID))) {
					btn->setColor(ccColor3B(166, 166, 166));
					btn->setOpacity(175);
					btn->m_animationEnabled = false;
				}
			}
		}
	}
}

void EditorPhysics::setPaused(bool paused) {
	m_isPaused = paused;
}