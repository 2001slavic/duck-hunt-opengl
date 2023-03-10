#pragma once

#include "utils/glm_utils.h"

struct ViewportSpace
{
    ViewportSpace() : x(0), y(0), width(1), height(1) {}
    ViewportSpace(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}
    int x;
    int y;
    int width;
    int height;
};

struct LogicSpace
{
    LogicSpace() : x(0), y(0), width(1), height(1) {}
    LogicSpace(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}
    float x;
    float y;
    float width;
    float height;
};

float generateRandom(float LO, float HI);

namespace transformations {

	glm::mat3 Translate(float tx, float ty);
	glm::mat3 Scale(float sx, float sy);
	glm::mat3 Rotate(float rx, float ry, float u);
	glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

    std::pair<float, float> RotatePoint(std::pair<float, float> pivot, float rotation, std::pair<float, float> target);
}