#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class ChickenInvaders : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {
            }
            int x, y, width, height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {
            }
            float x, y, width, height;
        };


        // =========================================================
        // Blocks
        // =========================================================
        enum class BlockType { NONE, SOLID, CANNON, ENGINE, BUMPER };
        struct Block { BlockType type = BlockType::NONE; };


        // =========================================================
        // Game Structs
        // =========================================================
        struct Ship {
            glm::vec2 position;
            std::vector<Block> blocks;
            std::vector<glm::vec2> offsets;
            float speed = 20.0f;
        };

        struct Projectile {
            glm::vec2 position;
            glm::vec2 direction;
            float speed = 10.0f;
            float size = 0.3f;
            bool isEnemy = false;
        };

        struct Enemy {
            glm::vec2 position;
            float speed = 5.0f;
            float size = 1.0f;
            std::string meshName;
        };

        struct AABB {
            glm::vec2 min;
            glm::vec2 max;
        };

        struct Upgrade {
            std::string name;
            int cost;
            float increment;
            int level;
            int maxLevel;
        };

        struct Particle {
            glm::vec2 position;
            glm::vec2 velocity;
            float lifetime;
            glm::vec3 color;
            float size;
        };
        
    public:
        ChickenInvaders();
        ~ChickenInvaders();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);
        void DrawStartButton(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);


        // =========================================================
        // Functions for grid
        // =========================================================
        glm::vec2 ConvertScreenToLogic(int mouseX, int mouseY);
        glm::ivec2 GetGridCell(int mouseX, int mouseY);
        int GetLeftPanelCell(int mouseX, int mouseY);
        bool IsValidPlacement(glm::ivec2 cell, BlockType type);
        bool IsGridValid();


        // =========================================================
        // Functions for game
        // =========================================================
        void RenderGame();
        void ShootProjectile();
        void EnemyShootProjectile(const Enemy& e);
        void SpawnEnemies();
        bool CheckCollision(const AABB& a, const AABB& b);
        AABB GetShipAABB();
        AABB GetEnemyAABB(const Enemy& e);
        void UpdateEnemyShooting(float deltaTime);
        void UpdateEnemyMovement(float deltaTimeSeconds);
        void SpawnParticles(glm::vec2 pos);
        void DrawEndScreen(glm::mat3 visMatrix);

    protected:
        // =========================================================
        // Visual vars
        // =========================================================
        float length;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;


        // =========================================================
        // Grid and blocks
        // =========================================================
        int gridWidth = 15;
        int gridHeight = 9;
        std::vector<std::vector<Block>> grid;
        std::vector<BlockType> leftPanelBlocks;
        int blocksLeft = 10;


        // =========================================================
        // Drag and drop vars
        // =========================================================
        bool isDragging = false;
        BlockType draggedBlock = BlockType::NONE;
        glm::vec2 mousePos;


        // =========================================================
        // Drag and drop vars
        // =========================================================
        bool inGarage;
        Ship ship;
        std::vector<Projectile> projectiles;
        std::vector<Projectile> enemyProjectiles;
        std::vector<Enemy> enemies;
        float shootInterval;
        float timeSinceLastShoot;
        bool gameOver;
        int score;
        gfxc::TextRenderer* textRenderer;
        int wave;
        std::vector<Particle> particles;

        // Base stats
        float shipSpeedBase = 10.0f;
        float projectileSpeedBase = 10.0f;

        float shipSpeedCurrent;
        float projectileSpeedCurrent;

        const int COST_SHIP_SPEED = 100;
        const int COST_PROJECTILE_SPEED = 100;

        int shipSpeedLevel;
        int projectileSpeedLevel;

    };
}   // namespace m1
