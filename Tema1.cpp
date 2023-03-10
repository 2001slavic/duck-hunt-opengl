#include "Tema1.h"

#include "primitives.h"
#include <iostream>

#include <fstream>

using namespace std;
using namespace m1;

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(COLOR_SKY[0], COLOR_SKY[1], COLOR_SKY[2], 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

// cleans the "ducks" vector from redundant objects
void Tema1::cleanup() {
    auto it = ducks.begin();
    while (it != ducks.end()) {
        float y1 = (*it)->getHitbox()[0].second;
        float y2 = (*it)->getHitbox()[2].second;
        if ((*it)->getState() == DUCK_DYING &&
            std::max(y1, y2) < grassHeight) {
            delete (*it);
            it = ducks.erase(it);
        }
        else if ((*it)->getState() == DUCK_ESCAPING &&
            std::min(y1, y2) > logic_space.height) {
            delete (*it);
            it = ducks.erase(it);
        }
        else {
            ++it;
        }
    }
    if (ducks.empty())
        newWave();
}

// starts new game wave
void Tema1::newWave() {
    wave++;
    last_shot = std::chrono::system_clock::now();
    id = 0;
    if (pauseRequested) { // pauses the game
        pauseRequested = false;
        paused = true;
        return;
    }

    ducks.clear();

    // spawns from 3 to 5 ducks
    int nDucks = 3 + (rand() % 3);

    ammo = 2 * nDucks;

    // deciding which ducks to spawn (default or special)
    for (int i = 0; i < nDucks; i++) {
        int duckType = rand() % 100;
        if (duckType >= 0 && duckType <= 9) // 10% of golden duck
            ducks.push_back(new GoldenDuck(to_string(id), 30 - difficulty * 2, difficulty * 6, difficulty));
        else if (duckType >= 10 && duckType <= 13) // 3% of ice duck
            ducks.push_back(new IceDuck(to_string(id), 30 - difficulty * 2, difficulty * 6, difficulty));
        else if (duckType >= 14 && duckType <= 19) // 5% of life duck
            ducks.push_back(new LifeDuck(to_string(id), 30 - difficulty * 2, difficulty * 6, difficulty));
        else
            ducks.push_back(new Duck(to_string(id), 30 - difficulty * 2, difficulty * 6, difficulty));

        id++;
    }

    for (auto duck : ducks) {
        AddMeshToList(duck->getBeak()->create());
        AddMeshToList(duck->getHead()->create());
        AddMeshToList(duck->getBody()->create());
        AddMeshToList(duck->getWings().first->create());
        AddMeshToList(duck->getWings().second->create());
    }

    // maximum difficulty -- 12
    if ((difficulty < 12) && (wave > difficulty * 2)) {
        difficulty++;
        wave = 0;
    }
        
    

}

// initiates the game (after game over)
void Tema1::game_init() {
    score = 0;
    hiScore = 0;
    difficulty = 1;
    slownessTimeout = 3;
    lives = 2;
    last_shot = std::chrono::system_clock::now();
    crosshair_highlighted = false;
    wave = 0;
    ammo = 1;
    gameOver = false;
    pauseRequested = false;
    paused = false;

    // reads hi-score from file
    ifstream readHiScore("hiscore.txt");
    readHiScore >> hiScore;
    readHiScore.close();

    newWave();
}


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{

    logic_space = LogicSpace(0, 0, 160, 90);
    grassHeight = logic_space.height / 3;
    time_to_shoot = 2;

    resolution = window->GetResolution();
    window->HidePointer();
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    
    // initiates heart, bullet and crosshair meshes
    AddMeshToList(Heart({ 0, 0 }, 50));
    AddMeshToList(Bullet({ 0, 0 }, 50));
    Rectangle* grass = new Rectangle("grass", { {logic_space.x, grassHeight}, {logic_space.width, grassHeight}, {logic_space.width, logic_space.y}, {logic_space.x, logic_space.y} }, COLOR_GRASS, 0);
    AddMeshToList(grass->create());
    Rectangle* crosshair = new Rectangle("crosshair", { {0.5, 0.5}, { -0.5, 0.5 }, { -0.5, -0.5 }, { 0.5, -0.5 } }, COLOR_BLACK, 0);
    AddMeshToList(crosshair->create());
    Rectangle* Hcrosshair = new Rectangle("Hcrosshair", { {0.5, 0.5}, { -0.5, 0.5 }, { -0.5, -0.5 }, { 0.5, -0.5 } }, COLOR_RED, 0);
    AddMeshToList(Hcrosshair->create());
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 20);

    game_init();
    
}


void Tema1::FrameStart()
{
    resolution = window->GetResolution();
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    resolution = window->GetResolution();

    
    ViewportSpace viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);

    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = glm::mat3(1);
    visMatrix *= transformations::VisualizationTransf2D(logic_space, viewSpace);

    // decide which crosshair to render, normal or highlighted one
    glm::mat3 crosshairModelMatrix = glm::mat3(1);
    crosshairModelMatrix *= visMatrix;
    crosshairModelMatrix *= transformations::Translate(mouseCoords.first, mouseCoords.second);
    if (crosshair_highlighted) {
        RenderMesh2D(meshes["Hcrosshair"], shaders["VertexColor"], crosshairModelMatrix);
        if ((chrono::duration_cast<chrono::milliseconds>(std::chrono::system_clock::now() - last_shot).count()) >= 100)
            crosshair_highlighted = false;
    } else
        RenderMesh2D(meshes["crosshair"], shaders["VertexColor"], crosshairModelMatrix);

    RenderMesh2D(meshes["grass"], shaders["VertexColor"], visMatrix);

    // keep the game paused
    if (paused) {
        textRenderer->RenderText("PAUSED", 620, 360, 1, glm::vec3(1, 1, 1));
        textRenderer->RenderText("Press P to continue.", 540, 400, 1, glm::vec3(1, 1, 1));
        return;
    }

    // game over
    if (lives < 0) {
        gameOver = true;
        glClearColor(COLOR_GAME_OVER[0], COLOR_GAME_OVER[1], COLOR_GAME_OVER[2], 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        textRenderer->RenderText("GAME OVER!", 605, 320, 1, glm::vec3(1, 1, 1));
        textRenderer->RenderText("Your final score: " + to_string(score), 550, 360, 1, glm::vec3(1, 1, 1));
        textRenderer->RenderText("Click to retry.", 580, 400, 1, glm::vec3(1, 1, 1));
        return;
    }

    // pause request
    if (pauseRequested) {
        textRenderer->RenderText("Pause on next wave", 550, 60, 1, glm::vec3(1, 1, 1));
    }

    // main game driver code
    for (auto duck : ducks) {
        // all ducks escape on zero ammo
        if (ammo <= 0 && duck->getState() == DUCK_ALIVE) {
            duck->escape();
            lives--;
        }
            
        // slowness timeout
        if ((chrono::duration_cast<chrono::seconds>(std::chrono::system_clock::now() - duck->getSlowTimestamp()).count()) >= slownessTimeout && duck->getSlowed() && duck->getState() == DUCK_ALIVE) {
            duck->setSlowed(false);
            duck->setSpeed(duck->getInitalSpeed());
            duck->setFlappingSpeed();
        }
        // escape timeout
        if (chrono::duration_cast<chrono::seconds>(std::chrono::system_clock::now() - last_shot).count() >= time_to_shoot && duck->getState() == DUCK_ALIVE) {
            last_shot = std::chrono::system_clock::now();
            for (auto duck_i : ducks) {
                if (duck_i->getState() == DUCK_ALIVE) {
                    duck_i->escape();
                    lives--;
                    break;
                }
            }
        }
        duck->applyModelMatrix(visMatrix); // apply visualisation matrix on ducks
        duck->moveDead(); // update falling speed of dying ducks

        duck->move(deltaTimeSeconds, logic_space); // move ducks on scene
        duck->wingsSetup(deltaTimeSeconds); // animates wings

        // actual rendering the ducks on scene
        RenderMesh2D(meshes[duck->getHead()->getName()], shaders["VertexColor"], duck->getHead()->getModelMatrix());
        RenderMesh2D(meshes[duck->getBeak()->getName()], shaders["VertexColor"], duck->getBeak()->getModelMatrix());
        RenderMesh2D(meshes[duck->getBody()->getName()], shaders["VertexColor"], duck->getBody()->getModelMatrix());
        RenderMesh2D(meshes[duck->getWings().first->getName()], shaders["VertexColor"], duck->getWings().first->getModelMatrix());
        RenderMesh2D(meshes[duck->getWings().second->getName()], shaders["VertexColor"], duck->getWings().second->getModelMatrix());
        
        // reset model matrices to ensure proper functionality
        duck->resetModelMatrices();
    }
    // renders HUD
    RenderMesh2D(meshes["heart"], shaders["VertexColor"], transformations::Translate(30, resolution.y - 20));
    RenderMesh2D(meshes["bullet"], shaders["VertexColor"], transformations::Translate(30, resolution.y - 60));
    textRenderer->RenderText("x " + to_string(lives), 60, 20, 1, glm::vec3(1, 1, 1));
    textRenderer->RenderText("x " + to_string(ammo), 60, 60, 1, glm::vec3(1, 1, 1));

    textRenderer->RenderText("Score: " + to_string(score), 1120, 20, 1, glm::vec3(1, 1, 1));
    textRenderer->RenderText("Hi-Score: " + to_string(hiScore), 1085, 60, 1, glm::vec3(1, 1, 1));
    textRenderer->RenderText("Difficulty: " + to_string(difficulty), 580, 20, 1, glm::vec3(1, 1, 1));
    
    // checks for renduntant elements in memory (in "ducks" vector)
    cleanup();
    
}


void Tema1::FrameEnd()
{
    
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{

}


void Tema1::OnKeyPress(int key, int mods)
{
    // request pause on P key press
    if (key == GLFW_KEY_P)
        if (!paused)
            pauseRequested = !pauseRequested;
        else
            paused = false;
}


void Tema1::OnKeyRelease(int key, int mods)
{

}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // update mouse coordonates to move crosshair properly
    ViewportSpace viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    mouseCoords.first = ((((float)mouseX - viewSpace.x) / (viewSpace.width - viewSpace.x)) * (logic_space.width - logic_space.x)) + logic_space.x;
    mouseCoords.second = logic_space.height - ((((float)(mouseY - viewSpace.y) / (viewSpace.height - viewSpace.y)) * (logic_space.height - logic_space.y)) + logic_space.y);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    if (gameOver) { // retry after game over
        game_init();
        return;
    }
        
    resolution = window->GetResolution();
    last_shot = std::chrono::system_clock::now(); // update time of last shot
    ammo--;

    // update mouse coordonates
    ViewportSpace viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    mouseCoords.first = ((((float)mouseX - viewSpace.x) / (viewSpace.width - viewSpace.x)) * (logic_space.width - logic_space.x)) + logic_space.x;
    mouseCoords.second = logic_space.height - ((((float)(mouseY - viewSpace.y) / (viewSpace.height - viewSpace.y)) * (logic_space.height - logic_space.y)) + logic_space.y);

    // check if a hitbox is being hit, also kill the duck with highest priority
    bool registered_hit = false;
    int max_priority = 0;
    int max_priority_index = 0;
    for (int i = 0; i < ducks.size(); i++) {
        if (ducks[i]->checkHit(mouseCoords)) {
            registered_hit = true;
            if (max_priority <= ducks[i]->getPriority()) {
                max_priority = ducks[i]->getPriority();
                max_priority_index = i;
            }
        }
    }
    if (registered_hit) {
        score += ducks[max_priority_index]->kill();
        crosshair_highlighted = true; // highlight crosshair on hit
        if (score > hiScore) { // update hi-score
            writeHiScore.open("hiscore.txt");
            hiScore = score;
            writeHiScore << hiScore;
            writeHiScore.close();
        }
        if (ducks[max_priority_index]->getType() == ICE_DUCK) { // apply ice duck effect
            for (auto duck : ducks) {
                if (duck->getState() == DUCK_ALIVE) {
                    if (!duck->getSlowed()) {
                        duck->setSpeed(duck->getSpeed() / 2);
                        duck->setFlappingSpeed();
                    }
                    duck->setSlowed(true);
                    duck->resetSlowTimestamp();
                }
            }
        }
        // give one life if suitable duck is hunted
        if (ducks[max_priority_index]->getType() == LIFE_DUCK)
            lives++;
    }
        

}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
