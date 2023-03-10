#pragma once

#include "primitives.h"
#include "transformations.h"
#include <map>
#include <chrono>

// duck states
#define DUCK_ALIVE 0
#define DUCK_ESCAPING 1
#define DUCK_DYING 2

//duck types
#define DUCK 0
#define GOLDEN_DUCK 1
#define ICE_DUCK 2
#define LIFE_DUCK 3

// default duck color palette
#define COLOR_YELLOW NormalizedRGB(237, 233, 0)
#define COLOR_DARK_GREEN NormalizedRGB(8, 119, 0)
#define COLOR_BROWN NormalizedRGB(102, 68, 0)

// golden duck color palette
#define COLOR_DARK_CHERRY NormalizedRGB(66, 3, 44)
#define COLOR_ORANGE NormalizedRGB(211, 107, 0)
#define COLOR_BEIGE NormalizedRGB(230, 210, 170)

// ice duck color pallete
#define COLOR_GRAY NormalizedRGB(51, 51, 51)
#define COLOR_DARK_BLUE NormalizedRGB(26, 38, 83)
#define COLOR_BLUE NormalizedRGB(0, 107, 206)

// life duck color palettte
#define COLOR_PURPLE NormalizedRGB(154, 22, 99)
#define COLOR_CORAL_RED NormalizedRGB(224, 20, 76)
#define COLOR_CORAL NormalizedRGB(255, 88, 88)

class Duck {
protected:
	std::string id;
	int state;
	int type;
	std::pair<float, float> position;
	std::pair<float, float> spawnPosition;
	float rotation;
	float size;
	float flappingSpeed;
	float speed;
	float initialSpeed;
	float wingRotation;
	float wingAmplitude;
	bool flappingDirection;
	IsoscelesTriangle* beak;
	Circle* head;
	IsoscelesTriangle* body;
	std::pair<IsoscelesTriangle*, IsoscelesTriangle*> wings;
	std::vector<std::pair<float, float>> hitbox;
	int priority;
	int reward;
	bool slowed;
	std::chrono::system_clock::time_point slow_timestamp;
	std::chrono::system_clock::time_point spawn_timestamp;
	std::chrono::system_clock::time_point killed_timestamp;

	glm::vec3 getColor(std::string key, std::map<std::string, glm::vec3> colors, glm::vec3 defaultColor);
	void hitboxUpdate();

public:
	Duck() = default;
	Duck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space = LogicSpace(0, 0, 160, 90), float grassHeight = 30);

	IsoscelesTriangle* getBeak();
	Circle* getHead();
	IsoscelesTriangle* getBody();
	std::pair<IsoscelesTriangle*, IsoscelesTriangle*> getWings();
	std::pair<float, float> getSpawnPosition();
	std::pair<float, float> getPosition();
	float getRotation();
	float getFlappingSpeed();
	float getWingRotation();
	float getWingAmplitude();
	bool getFlappingDirection();
	int getPriority();
	int getState();
	int getType();
	float getInitalSpeed();
	float getSpeed();
	bool getSlowed();
	std::vector<std::pair<float, float>> getHitbox();
	std::chrono::system_clock::time_point getSlowTimestamp();
	std::chrono::system_clock::time_point getSpawnTimestamp();

	void setPosition(std::pair<float, float> position);
	void setRotation(float rad);
	void setWingRotation(float rad);
	void toggleFlappingDirection();
	void setState(int state);
	void setSlowed(bool slowed);
	void resetSlowTimestamp();
	void setSpeed(float speed);
	void setFlappingSpeed(float flappingSpeed);
	void setFlappingSpeed();
	void setHitbox(std::vector<std::pair<float, float>> hitbox);
	void setSpawnTimestamp();

	void applyModelMatrix(glm::mat3 modelMatrix);
	void resetModelMatrices();
	void move(float deltaTimeSeconds, LogicSpace boundaries, float grassHeight = 30);
	void wingsSetup(float deltaTimeSeconds);
	void moveDead();

	bool checkHit(std::pair<float, float> mouse);
	void ricochet(LogicSpace boundaries);
	int kill();
	void escape();
};

class GoldenDuck : public Duck {
public:
	GoldenDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space = LogicSpace(0, 0, 160, 90), float grassHeight = 30);
};

class IceDuck : public Duck {
public:
	IceDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space = LogicSpace(0, 0, 160, 90), float grassHeight = 30);
};

class LifeDuck : public Duck {
public:
	LifeDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space = LogicSpace(0, 0, 160, 90), float grassHeight = 30);
};