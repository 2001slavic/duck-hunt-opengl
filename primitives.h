#pragma once

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define COLOR_BULLET_SHELL NormalizedRGB(175, 155, 96)
#define COLOR_RED NormalizedRGB(255, 0, 0)


float distancePoints(std::pair<float, float> a, std::pair<float, float> b);
float triangleArea(std::pair<float, float> a, std::pair<float, float> b, std::pair<float, float> c);
bool cmpf(float A, float B, float epsilon = 0.005f);

Mesh* Bullet(std::pair<float, float> position, float size);
Mesh* Heart(std::pair<float, float> position, float size);

class Shape {
protected:
	std::string name;
	float X;
	float Y;
	float rotation;
	float size;
	glm::vec3 color;
	glm::mat3 modelMatrix;

};

class IsoscelesTriangle : virtual Shape {
private:
	float height;
	float baseLength;

public:
	IsoscelesTriangle(std::string name, float X, float Y, float size, glm::vec3 color, float rotation, float height, float baseLength);
	Mesh* create();
	float getSize();
	float getHeight();
	float getBaseLength();
	float getX();
	float getY();
	std::string getName();
	glm::mat3 getModelMatrix();

	void setX(float X);
	void setY(float Y);
	void setRotation(float u);

	void applyModelMatrix(glm::mat3 modelMatrix);
	void resetModelMatrix();
};

class Circle : virtual Shape {
public:
	Circle(std::string name, float X, float Y, float size, glm::vec3 color, float rotation);

	Mesh* create();
	float getX();
	float getY();
	std::string getName();
	glm::mat3 getModelMatrix();

	void setX(float X);
	void setY(float Y);
	void setRotation(float u);

	void applyModelMatrix(glm::mat3 modelMatrix);
	void resetModelMatrix();
};

class Rectangle : virtual Shape {
private:
	std::vector<std::pair<float, float>> points;
public:
	Rectangle(std::string name, std::vector<std::pair<float, float>> points, glm::vec3 color, float rotation);
	Mesh* create();
};

