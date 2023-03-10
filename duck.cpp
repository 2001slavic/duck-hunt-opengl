#include "duck.h"

// setting up properties for duck
Duck::Duck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space, float grassHeight) {
	this->id = id.c_str();
	this->state = DUCK_ALIVE;
	this->type = DUCK;
	this->position.first = generateRandom(logic_space.x, logic_space.width);
	this->position.second = grassHeight;
	this->spawnPosition = this->position;
	this->rotation = generateRandom(-M_PI_2 + 0.25, M_PI_2 - 0.25);
	if (cmpf(this->rotation, 0, 0.1f)) this->rotation += 0.25; // zero angle protection
	this->wingRotation = 0;
	this->size = baseSize - generateRandom(0, 3);
	this->speed = baseSpeed + generateRandom(0, 20);
	this->initialSpeed = this->speed;
	this->flappingSpeed = (this->speed * 0.2) * (2 * (1 / sqrt(this->size))); // wing flapping speed depends on duck speed and size
	this->wingAmplitude = M_PI_4;
	this->flappingDirection = true;
	this->priority = 0; // priority for case in which mouse hits two or more hitboxes simultaneously
	this->reward = baseReward;
	this->slowed = false;
	this->spawn_timestamp = std::chrono::system_clock::now();
	this->slow_timestamp = std::chrono::system_clock::now();
	this->killed_timestamp = std::chrono::system_clock::now();
	this->beak = new IsoscelesTriangle("beak" + this->id,
		position.first,
		position.second,
		this->size * 0.2,
		COLOR_YELLOW,
		rotation,
		1,
		0.5);
	this->head = new Circle("head" + this->id,
		position.first,
		beak->getY() - (beak->getSize() * beak->getHeight() * 0.8 + size * 0.23),
		this->size * 0.23,
		COLOR_DARK_GREEN,
		rotation);
	this->body = new IsoscelesTriangle("body" + this->id,
		position.first,
		head->getY(),
		this->size,
		COLOR_BROWN,
		rotation,
		1,
		0.35);
	this->wings.first = new IsoscelesTriangle("wingL" + this->id,
		body->getX() - body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BROWN,
		rotation,
		1,
		0.3);
	this->wings.second = new IsoscelesTriangle("wingR" + this->id,
		body->getX() + body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BROWN,
		rotation,
		1,
		0.3);

	this->hitbox = { {this->position.first - body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()},
		{this->position.first - body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()} };
};

// setting up properites for golden duck
GoldenDuck::GoldenDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space, float grassHeight) {
	this->id = id.c_str();
	this->state = DUCK_ALIVE;
	this->type = GOLDEN_DUCK;
	this->position.first = generateRandom(logic_space.x, logic_space.width);
	this->position.second = grassHeight;
	this->spawnPosition = this->position;
	this->rotation = generateRandom(-M_PI_2 + 0.25, M_PI_2 - 0.25);
	if (cmpf(this->rotation, 0, 0.1f)) this->rotation += 0.25;
	this->wingRotation = 0;
	this->size = (baseSize * 0.75) - generateRandom(0, 2);
	this->speed = (baseSpeed * 1.25) + generateRandom(0, 20);
	this->initialSpeed = this->speed;
	this->flappingSpeed = (this->speed * 0.2) * (2 * (1 / sqrt(this->size)));
	this->wingAmplitude = M_PI_4;
	this->flappingDirection = true;
	this->priority = 1;
	this->reward = baseReward * 2;
	this->slowed = false;
	this->spawn_timestamp = std::chrono::system_clock::now();
	this->slow_timestamp = std::chrono::system_clock::now();
	this->killed_timestamp = std::chrono::system_clock::now();
	this->beak = new IsoscelesTriangle("beak" + this->id,
		position.first,
		position.second,
		this->size * 0.2,
		COLOR_ORANGE,
		rotation,
		1,
		0.5);
	this->head = new Circle("head" + this->id,
		position.first,
		beak->getY() - (beak->getSize() * beak->getHeight() * 0.8 + size * 0.23),
		this->size * 0.23,
		COLOR_DARK_CHERRY,
		rotation);
	this->body = new IsoscelesTriangle("body" + this->id,
		position.first,
		head->getY(),
		this->size,
		COLOR_BEIGE,
		rotation,
		1,
		0.35);
	this->wings.first = new IsoscelesTriangle("wingL" + this->id,
		body->getX() - body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BEIGE,
		rotation,
		1,
		0.3);
	this->wings.second = new IsoscelesTriangle("wingR" + this->id,
		body->getX() + body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BEIGE,
		rotation,
		1,
		0.3);

	this->hitbox = { {this->position.first - body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()},
		{this->position.first - body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()} };
};

// setting up properties for ice duck
IceDuck::IceDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space, float grassHeight) {
	this->id = id.c_str();
	this->state = DUCK_ALIVE;
	this->type = ICE_DUCK;
	this->position.first = generateRandom(logic_space.x, logic_space.width);
	this->position.second = grassHeight;
	this->spawnPosition = this->position;
	this->rotation = generateRandom(-M_PI_2 + 0.25, M_PI_2 - 0.25);
	if (cmpf(this->rotation, 0, 0.1f)) this->rotation += 0.25; // zero angle protection
	this->wingRotation = 0;
	this->size = (baseSize * 0.75) - generateRandom(0, 2);
	this->speed = (baseSpeed * 1.25) + generateRandom(0, 20);
	this->initialSpeed = this->speed;
	this->flappingSpeed = (this->speed * 0.2) * (2 * (1 / sqrt(this->size)));
	this->wingAmplitude = M_PI_4;
	this->flappingDirection = true;
	this->priority = 3;
	this->reward = baseReward;
	this->slowed = false;
	this->spawn_timestamp = std::chrono::system_clock::now();
	this->slow_timestamp = std::chrono::system_clock::now();
	this->killed_timestamp = std::chrono::system_clock::now();
	this->beak = new IsoscelesTriangle("beak" + this->id,
		position.first,
		position.second,
		this->size * 0.2,
		COLOR_GRAY,
		rotation,
		1,
		0.5);
	this->head = new Circle("head" + this->id,
		position.first,
		beak->getY() - (beak->getSize() * beak->getHeight() * 0.8 + size * 0.23),
		this->size * 0.23,
		COLOR_DARK_BLUE,
		rotation);
	this->body = new IsoscelesTriangle("body" + this->id,
		position.first,
		head->getY(),
		this->size,
		COLOR_BLUE,
		rotation,
		1,
		0.35);
	this->wings.first = new IsoscelesTriangle("wingL" + this->id,
		body->getX() - body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BLUE,
		rotation,
		1,
		0.3);
	this->wings.second = new IsoscelesTriangle("wingR" + this->id,
		body->getX() + body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_BLUE,
		rotation,
		1,
		0.3);

	this->hitbox = { {this->position.first - body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()},
		{this->position.first - body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()} };
}

// setting up properties for duck awarding a life
LifeDuck::LifeDuck(std::string id, float baseSize, float baseSpeed, int baseReward, LogicSpace logic_space, float grassHeight) {
	this->id = id.c_str();
	this->state = DUCK_ALIVE;
	this->type = LIFE_DUCK;
	this->position.first = generateRandom(logic_space.x, logic_space.width);
	this->position.second = grassHeight;
	this->spawnPosition = this->position;
	this->rotation = generateRandom(-M_PI_2 + 0.25, M_PI_2 - 0.25);
	if (cmpf(this->rotation, 0, 0.1f)) this->rotation += 0.25;
	this->wingRotation = 0;
	this->size = (baseSize * 0.75) - generateRandom(0, 2);
	this->speed = (baseSpeed * 1.25) + generateRandom(0, 20);
	this->initialSpeed = this->speed;
	this->flappingSpeed = (this->speed * 0.2) * (2 * (1 / sqrt(this->size)));
	this->wingAmplitude = M_PI_4;
	this->flappingDirection = true;
	this->priority = 2;
	this->reward = baseReward;
	this->slowed = false;
	this->spawn_timestamp = std::chrono::system_clock::now();
	this->slow_timestamp = std::chrono::system_clock::now();
	this->killed_timestamp = std::chrono::system_clock::now();
	this->beak = new IsoscelesTriangle("beak" + this->id,
		position.first,
		position.second,
		this->size * 0.2,
		COLOR_PURPLE,
		rotation,
		1,
		0.5);
	this->head = new Circle("head" + this->id,
		position.first,
		beak->getY() - (beak->getSize() * beak->getHeight() * 0.8 + size * 0.23),
		this->size * 0.23,
		COLOR_CORAL_RED,
		rotation);
	this->body = new IsoscelesTriangle("body" + this->id,
		position.first,
		head->getY(),
		this->size,
		COLOR_CORAL,
		rotation,
		1,
		0.35);
	this->wings.first = new IsoscelesTriangle("wingL" + this->id,
		body->getX() - body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_CORAL,
		rotation,
		1,
		0.3);
	this->wings.second = new IsoscelesTriangle("wingR" + this->id,
		body->getX() + body->getHeight() * body->getSize() / 2,
		body->getY() - body->getHeight() * body->getSize() / 1.8,
		this->size * 0.5,
		COLOR_CORAL,
		rotation,
		1,
		0.3);

	this->hitbox = { {this->position.first - body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()},
		{this->position.first - body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()} };
}

// moves hitbox according to duck position and rotation
void Duck::hitboxUpdate() {
	this->hitbox = { {this->position.first - body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), this->position.second},
		{this->position.first + body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()},
		{this->position.first - body->getBaseLength() * body->getSize(), body->getY() - body->getHeight() * body->getSize()} };

	std::pair<float, float> x1y1 = this->hitbox[0];
	std::pair<float, float> x2y1 = this->hitbox[1];
	std::pair<float, float> x2y2 = this->hitbox[2];
	std::pair<float, float> x1y2 = this->hitbox[3];

	this->hitbox = { transformations::RotatePoint(this->position, this->rotation, x1y1),
	transformations::RotatePoint(this->position, this->rotation, x2y1),
	transformations::RotatePoint(this->position, this->rotation, x2y2),
	transformations::RotatePoint(this->position, this->rotation, x1y2) };
};

// getters

IsoscelesTriangle* Duck::getBeak() { return this->beak; };
Circle* Duck::getHead() { return this->head; };
IsoscelesTriangle* Duck::getBody() { return this->body; };
std::pair<IsoscelesTriangle*, IsoscelesTriangle*> Duck::getWings() { return this->wings; };
std::pair<float, float> Duck::getSpawnPosition() { return this->spawnPosition; };
std::pair<float, float> Duck::getPosition() { return this->position; };
float Duck::getRotation() { return this->rotation; };
float Duck::getFlappingSpeed() { return this->flappingSpeed; };
float Duck::getWingRotation() { return this->wingRotation; };
float Duck::getWingAmplitude() { return this->wingAmplitude; };
bool Duck::getFlappingDirection() { return this->flappingDirection; };
int Duck::getPriority() { return this->priority; };
int Duck::getState() { return this->state; };
int Duck::getType() { return this->type; };
float Duck::getSpeed() { return this->speed; };
float Duck::getInitalSpeed() { return this->initialSpeed; };
bool Duck::getSlowed() { return this->slowed; };
std::vector<std::pair<float, float>> Duck::getHitbox() { return this->hitbox; };
std::chrono::system_clock::time_point Duck::getSlowTimestamp() { return this->slow_timestamp; };
std::chrono::system_clock::time_point Duck::getSpawnTimestamp() { return this->spawn_timestamp; };

// setters

void Duck::setPosition(std::pair<float, float> position) { this->position = position; };
void Duck::setRotation(float rad) { this->rotation = rad; };
void Duck::setWingRotation(float rad) { this->wingRotation = rad; };
void Duck::toggleFlappingDirection() { this->flappingDirection = !this->flappingDirection; };
void Duck::setHitbox(std::vector<std::pair<float, float>> hitbox) { this->hitbox = hitbox; };
void Duck::setState(int state) { this->state = state; };
void Duck::setSlowed(bool slowed) { this->slowed = slowed; };
void Duck::resetSlowTimestamp() { this->slow_timestamp = std::chrono::system_clock::now(); };
void Duck::setSpeed(float speed) { this->speed = speed; };
void Duck::setFlappingSpeed(float flappingSpeed) { this->flappingSpeed = flappingSpeed; };
void Duck::setFlappingSpeed() { this->flappingSpeed = (this->speed * 0.2) * (2 * (1 / sqrt(this->size))); };
void Duck::setSpawnTimestamp() { this->spawn_timestamp = std::chrono::system_clock::now(); };

// applies modelMatrix to all body parts (meshes) of a duck
void Duck::applyModelMatrix(glm::mat3 modelMatrix) {
	this->beak->applyModelMatrix(modelMatrix);
	this->head->applyModelMatrix(modelMatrix);
	this->body->applyModelMatrix(modelMatrix);
	this->wings.first->applyModelMatrix(modelMatrix);
	this->wings.second->applyModelMatrix(modelMatrix);
};

// sets all model matrices of duck to identity matrix
void Duck::resetModelMatrices() {
	this->beak->resetModelMatrix();
	this->head->resetModelMatrix();
	this->body->resetModelMatrix();
	this->wings.first->resetModelMatrix();
	this->wings.second->resetModelMatrix();
};

// actually "moves" ducks in the scene
void Duck::move(float deltaTimeSeconds, LogicSpace boundaries, float grassHeight) {
	glm::mat3 modelMatrix = glm::mat3(1);

	// random duck direction each tick
	if (this->state == DUCK_ALIVE && this->position.second >= grassHeight) {
		int changeDirection = rand() % 1000; //chance 1/1000
		if (changeDirection == 0) {
			this->rotation = generateRandom(0.25, 2 * M_PI - 0.25);
			if (cmpf(this->rotation, M_PI_2, 0.1f) || cmpf(this->rotation, M_PI, 0.1f) || cmpf(this->rotation, 3 * M_PI_2, 0.1f))
				this->rotation += 0.25;
		}
	}

	// compute next position based on facing direction
	this->position.first += cos(this->rotation + M_PI_2) * this->speed * deltaTimeSeconds;
	this->position.second += sin(this->rotation + M_PI_2) * this->speed * deltaTimeSeconds;
	ricochet(boundaries); // check if duck should ricochet from walls (screen borders)

	modelMatrix *= transformations::Rotate(this->position.first, this->position.second, this->rotation);
	modelMatrix *= transformations::Translate(this->position.first - this->spawnPosition.first, this->position.second - this->spawnPosition.second);

	// updates position and rotation of all body parts
	this->beak->setX(this->position.first);
	this->beak->setY(this->position.second);
	this->beak->setRotation(this->rotation);

	
	this->head->setX(this->position.first);
	this->head->setY(beak->getY() - (beak->getSize() * beak->getHeight() * 0.8 + size * 0.23));
	this->head->setRotation(this->rotation);

	
	this->body->setX(this->position.first);
	this->body->setY(head->getY());
	this->body->setRotation(this->rotation);

	// but not for wings, because it is buggy

	hitboxUpdate();

	// apply model matrix to make changes visible
	applyModelMatrix(modelMatrix);

	
};

// animates wings
void Duck::wingsSetup(float deltaTimeSeconds) {
	// making wings perpendicular to body
	this->wings.first->applyModelMatrix(transformations::Rotate(this->wings.first->getX(),
		this->wings.first->getY(),
		M_PI_2));
	this->wings.second->applyModelMatrix(transformations::Rotate(this->wings.second->getX(),
		this->wings.second->getY(),
		-M_PI_2));

	// checking limits of wings' movement
	if ((this->wingRotation + this->flappingSpeed * deltaTimeSeconds > this->wingAmplitude && this->flappingDirection) ||
		(this->wingRotation - this->flappingSpeed * deltaTimeSeconds < -this->wingAmplitude && !this->flappingDirection))
		this->toggleFlappingDirection();

	// moving wings
	if (this->flappingDirection)
		this->wingRotation = this->wingRotation + this->flappingSpeed * deltaTimeSeconds;
	else
		this->wingRotation = this->wingRotation - this->flappingSpeed * deltaTimeSeconds;

	// aplying rotation transformation for wings animation
	this->wings.first->applyModelMatrix(transformations::Rotate(this->wings.first->getX(),
		this->wings.first->getY() - (this->wings.first->getHeight() * this->wings.first->getSize()),
		this->wingRotation));
	this->wings.second->applyModelMatrix(transformations::Rotate(this->wings.second->getX(),
		this->wings.second->getY() - (this->wings.second->getHeight() * this->wings.second->getSize()),
		-this->wingRotation));
};

// changes falling speed for dying ducks based on free fall physics
void Duck::moveDead() {
	if (this->state != DUCK_DYING) return;

	// free fall equation
	float t = std::chrono::duration_cast<std::chrono::milliseconds>(this->killed_timestamp - std::chrono::system_clock::now()).count() / 1000.0f;
	float g = 9.18; // gravitational acceleration

	this->speed = this->initialSpeed - g * t;
};

// checks if mouse click coordonates are inside a hitbox
bool Duck::checkHit(std::pair<float, float> mouse) {
	if (this->state != DUCK_ALIVE) return false;
	std::pair<float, float> A = this->hitbox[0];
	std::pair<float, float> B = this->hitbox[1];
	std::pair<float, float> C = this->hitbox[2];
	std::pair<float, float> D = this->hitbox[3];

	float hitboxLength = distancePoints(A, B);
	float hitboxHeight = distancePoints(A, D);

	float hitboxArea = hitboxLength * hitboxHeight; // compute hitbox area
	// make four triangles from mouse point and hitbox vertices
	float trianglesArea = triangleArea(A, mouse, D) + triangleArea(D, mouse, C) + triangleArea(C, mouse, B) + triangleArea(mouse, B, A);

	// if sum of areas of triangles is bigger than hitbox area -- mouse point is outside of hitbox
	return cmpf(hitboxArea, trianglesArea);

};

// checks if duck should ricochet, and computes angle of rotation accordingly
void Duck::ricochet(LogicSpace boundaries) {
	if (this->state != DUCK_ALIVE) return;
	std::pair<float, float> x1y1 = hitbox[0];
	std::pair<float, float> x2y2 = hitbox[2];
	if (this->position.first <= boundaries.x) {
		this->rotation = -this->rotation;
		this->position.first = std::max(x1y1.first, x2y2.first);
	}
	if (this->position.first >= boundaries.width) {
		this->rotation = -this->rotation;
		this->position.first = std::min(x1y1.first, x2y2.first);
	}
	if (this->position.second <= boundaries.y) {
		this->rotation = M_PI - this->rotation;
		this->position.second = std::max(x1y1.second, x2y2.second);
	}
	if (this->position.second >= boundaries.height) {
		this->rotation = M_PI - this->rotation;
		this->position.second = std::min(x1y1.second, x2y2.second);
	}
};

// transitions duck to "dying" state
// \returns score reward for hunting the duck
int Duck::kill() {
	if (this->state != DUCK_ALIVE) return 0;

	std::pair<float, float> x1y1 = hitbox[0];
	std::pair<float, float> x2y2 = hitbox[2];

	this->state = DUCK_DYING;
	this->killed_timestamp = std::chrono::system_clock::now();
	this->position.second = std::min(x1y1.second, x2y2.second);
	// if duck was facing to positive Y (up), initial falling speed will be smaller
	this->initialSpeed *= 0.25 * (1 - cos(this->rotation));
	this->speed = 0;
	this->rotation = M_PI;
	this->flappingSpeed = 0;
	this->wingRotation = M_PI / 6;
	return this->reward;
};

// transitions duck to "escaping" state
void Duck::escape() {
	this->state = DUCK_ESCAPING;
	this->speed = this->initialSpeed * 2;
	this->setFlappingSpeed();
	this->rotation = 0;
};
