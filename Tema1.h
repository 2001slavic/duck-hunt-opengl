#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "duck.h"

#include <fstream>

#define COLOR_GRASS NormalizedRGB(107, 156, 88)
#define COLOR_SKY NormalizedRGB(135, 206, 235)
#define COLOR_GAME_OVER NormalizedRGB(126, 129, 129)
#define COLOR_GREEN NormalizedRGB(0, 255, 0)
#define COLOR_RED NormalizedRGB(255, 0, 0)
#define COLOR_BLACK NormalizedRGB(0, 0, 0)

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:

        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

        void cleanup();
        void newWave();
        void game_init();

    protected:
        std::vector<Duck*> ducks;
        glm::ivec2 resolution;
        glm::mat3 visMatrix;
        std::pair<float, float> mouseCoords;
        int hiScore;
        int score;
        int lives;
        unsigned int id;
        int difficulty;

        int slownessTimeout;

        LogicSpace logic_space;
        float grassHeight;

        std::chrono::system_clock::time_point last_shot;
        int time_to_shoot;
        int wave;
        int ammo;

        gfxc::TextRenderer* textRenderer;

        std::ofstream writeHiScore;

        bool gameOver;
        bool pauseRequested;
        bool paused;
        bool crosshair_highlighted;
        //float cameraZoom;
    };
}
