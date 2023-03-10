#include "primitives.h"

#include <vector>

// computes distance between two points in coordonate system
float distancePoints(std::pair<float, float> a, std::pair<float, float> b) {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
};

// computes area of a triangle
// \param triangle vertices in coordonate system
float triangleArea(std::pair<float, float> a, std::pair<float, float> b, std::pair<float, float> c) {
    return abs(a.first * (b.second - c.second) + b.first * (c.second - a.second) + c.first * (a.second - b.second)) / 2;
};

// compares floats
bool cmpf(float A, float B, float epsilon) {
    return (fabs(A - B) < epsilon);
};

// creates "bullet" mesh
Mesh* Bullet(std::pair<float, float> position, float size) {
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(position.first, position.second, 0), COLOR_BULLET_SHELL),
        VertexFormat(glm::vec3(position.first + size * 0.2, position.second - size * 0.25, 0), COLOR_BULLET_SHELL),
        VertexFormat(glm::vec3(position.first + size * 0.2, position.second - size, 0), COLOR_BULLET_SHELL),
        VertexFormat(glm::vec3(position.first - size * 0.2, position.second - size, 0), COLOR_BULLET_SHELL),
        VertexFormat(glm::vec3(position.first - size * 0.2, position.second - size * 0.25, 0), COLOR_BULLET_SHELL)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 0, 3, 0, 2};

    Mesh* bullet = new Mesh("bullet");

    bullet->InitFromData(vertices, indices);

    return bullet;
};

// creates "heart" mesh
Mesh* Heart(std::pair<float, float> position, float size) {
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(position.first, position.second, 0), COLOR_RED),
        VertexFormat(glm::vec3(position.first + size * 0.25, position.second + size * 0.25, 0), COLOR_RED),
        VertexFormat(glm::vec3(position.first + size * 0.5, position.second, 0), COLOR_RED),
        VertexFormat(glm::vec3(position.first, position.second - size * 0.75, 0), COLOR_RED),
        VertexFormat(glm::vec3(position.first - size * 0.5, position.second, 0), COLOR_RED),
        VertexFormat(glm::vec3(position.first - size * 0.25, position.second + size * 0.25, 0), COLOR_RED)
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0, 3, 4, 0, 4, 5, 0 };

    Mesh* heart = new Mesh("heart");

    heart->InitFromData(vertices, indices);

    return heart;
};

// isosceles triangle constructor
// \param name, position, size, color, rotation, height and base ratios
IsoscelesTriangle::IsoscelesTriangle(std::string name, float X, float Y, float size, glm::vec3 color, float rotation, float height, float baseLength) {
    this->Shape::name = name.c_str();
    this->Shape::X = X;
    this->Shape::Y = Y;
    this->Shape::size = size;
    this->Shape::color = color;
    this->Shape::modelMatrix = glm::mat3(1);
    this->Shape::rotation = rotation;
    this->height = height;
    this->baseLength = baseLength;
};

// circle constructor
Circle::Circle(std::string name, float X, float Y, float size, glm::vec3 color, float rotation) {
    this->Shape::name = name.c_str();
    this->Shape::X = X;
    this->Shape::Y = Y;
    this->Shape::size = size;
    this->Shape::color = color;
    this->Shape::rotation = rotation;
    this->Shape::modelMatrix = glm::mat3(1);
};

// rectangle constructor
// \param "points" should be a vector of four float pairs, which represent the coordonates of vertices of the rectangle
Rectangle::Rectangle(std::string name, std::vector<std::pair<float, float>> points, glm::vec3 color, float rotation) {
    this->Shape::name = name.c_str();
    this->Shape::X = 0;
    this->Shape::Y = 0;
    this->Shape::size = 1;
    this->Shape::color = color;
    this->Shape::rotation = rotation;
    this->Shape::modelMatrix = glm::mat3(1);
    this->points = points;
};

// creates the actual mesh of isosceles triangle
Mesh* IsoscelesTriangle::create() {
    glm::vec3 corner = glm::vec3(Shape::X, Shape::Y, 0);

    std::vector<VertexFormat> vertices =
        {
            VertexFormat(corner, Shape::color),
            VertexFormat(corner - glm::vec3(baseLength * Shape::size, height * Shape::size, 0), Shape::color),
            VertexFormat(corner - glm::vec3(-baseLength * Shape::size, height * Shape::size, 0), Shape::color),
        };

    Mesh* triangle = new Mesh(Shape::name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle->Mesh::InitFromData(vertices, indices);
    return triangle;
};

// creates actual mesh of circle
Mesh* Circle::create() {
    int polygons = 360;
    glm::vec3 centre = glm::vec3(Shape::X, Shape::Y, 0);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= polygons; i++) {
        float cos_expr = Shape::size * cos(i * 2 * M_PI / polygons);
        float sin_expr = Shape::size * sin(i * 2 * M_PI / polygons);
        vertices.push_back(VertexFormat(centre + glm::vec3(cos_expr, sin_expr, 0), Shape::color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(Shape::name);

    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);


    return circle;
};

// creates actual mesh of rectangle
Mesh* Rectangle::create() {

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(this->points[0].first, this->points[0].second, 0), Shape::color),
        VertexFormat(glm::vec3(this->points[1].first, this->points[1].second, 0), Shape::color),
        VertexFormat(glm::vec3(this->points[2].first, this->points[2].second, 0), Shape::color),
        VertexFormat(glm::vec3(this->points[3].first, this->points[3].second, 0), Shape::color),
    };

    Mesh* rectangle = new Mesh(Shape::name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 0, 2};

    rectangle->Mesh::InitFromData(vertices, indices);
    return rectangle;
}

// getters setters and other utils

float IsoscelesTriangle::getHeight() { return this->height; };
float IsoscelesTriangle::getBaseLength() { return this->baseLength; };
float IsoscelesTriangle::getSize() { return this->size; };
float IsoscelesTriangle::getX() { return this->X; };
float IsoscelesTriangle::getY() { return this->Y; };

std::string IsoscelesTriangle::getName() { return this->name; };
glm::mat3 IsoscelesTriangle::getModelMatrix() { return this->modelMatrix; };

void IsoscelesTriangle::setX(float X) { this->Shape::X = X; };
void IsoscelesTriangle::setY(float Y) { this->Shape::Y = Y; };
void IsoscelesTriangle::setRotation(float u) { this->Shape::rotation = u; };

void IsoscelesTriangle::applyModelMatrix(glm::mat3 modelMatrix) { this->Shape::modelMatrix *= modelMatrix; };
void IsoscelesTriangle::resetModelMatrix() { this->Shape::modelMatrix = glm::mat3(1); };

float Circle::getX() { return this->X; };
float Circle::getY() { return this->Y; };
std::string Circle::getName() { return this->name; };
glm::mat3 Circle::getModelMatrix() { return this->modelMatrix; };

void Circle::setX(float X) { this->Shape::X = X; };
void Circle::setY(float Y) { this->Shape::Y = Y; };
void Circle::setRotation(float u) { this->Shape::rotation = u; };

void Circle::applyModelMatrix(glm::mat3 modelMatrix) { this->Shape::modelMatrix *= modelMatrix; };
void Circle::resetModelMatrix() { this->Shape::modelMatrix = glm::mat3(1); };