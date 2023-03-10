#include "transformations.h"
// 2D translation
glm::mat3 transformations::Translate(float tx, float ty) {
	return glm::transpose(glm::mat3(1, 0, tx,
		0, 1, ty,
		0, 0, 1));
};

// 2D scale
glm::mat3 transformations::Scale(float sx, float sy) {
	return glm::transpose(glm::mat3(sx, 0, 0,
		0, sy, 0,
		0, 0, 1));
};

// 2D rotation
glm::mat3 transformations::Rotate(float rx, float ry, float u) {
	glm::mat3 res = glm::mat3(1);
	res *= Translate(rx, ry);
	res *= glm::transpose(glm::mat3(cos(u), -sin(u), 0,
		sin(u), cos(u), 0,
		0, 0, 1));
	res *= Translate(-rx, -ry);

	return res;
};

// window to viewport transformation
glm::mat3 transformations::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
};

// rotate point around another point
std::pair<float, float> transformations::RotatePoint(std::pair<float, float> pivot, float rotation, std::pair<float, float> target) {
	// translate point back to origin
	target.first -= pivot.first;
	target.second -= pivot.second;

	// rotate point
	float newX = target.first * cos(rotation) - target.second * sin(rotation);
	float newY = target.first * sin(rotation) + target.second * cos(rotation);

	// translate point back
	target.first = newX + pivot.first;
	target.second = newY + pivot.second;

	return target;

};

// generate random float
float generateRandom(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}