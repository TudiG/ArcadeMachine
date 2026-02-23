#include "chicken_invaders.h"

#include <vector>
#include <iostream>
#include <queue>

#include "include/transform2D.h"
#include "include/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

ChickenInvaders::ChickenInvaders()
{
}


ChickenInvaders::~ChickenInvaders()
{
    delete textRenderer;
}


void ChickenInvaders::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace = LogicSpace(0, 0, 32, 18);

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    // Editor blocks
    Mesh* sidebarSquare = object2D::CreateSquare("sidebarSquare", corner, length, glm::vec3(1, 0, 0));
    AddMeshToList(sidebarSquare);

    Mesh* gridSquare = object2D::CreateSquare("gridSquare", corner, length, glm::vec3(0, 0, 1), true);
    AddMeshToList(gridSquare);

    Mesh* gridDelimiterSquare = object2D::CreateSquare("gridDelimiterSquare", corner, length, glm::vec3(0, 0, 1));
    AddMeshToList(gridDelimiterSquare);

    Mesh* componentCountSquare = object2D::CreateSquare("componentCountSquare", corner, length, glm::vec3(0, 1, 0), true);
    AddMeshToList(componentCountSquare);

    Mesh* validStartButton = object2D::CreateStartButton("validStartButton", corner, length, glm::vec3(0, 1, 0), true);
    AddMeshToList(validStartButton);

    Mesh* invalidStartButton = object2D::CreateStartButton("invalidStartButton", corner, length, glm::vec3(1, 0, 0), true);
    AddMeshToList(invalidStartButton);
    // end

    // Components
    Mesh* solidBlock = object2D::CreateSolidBlock("solidBlock", corner, length, glm::vec3(1, 1, 1), true);
    AddMeshToList(solidBlock);

    Mesh* cannonBlock = object2D::CreateCannon("cannonBlock", corner, length, glm::vec3(1, 0, 1), true);
    AddMeshToList(cannonBlock);

    Mesh* engineBlock = object2D::CreateEngine("engineBlock", corner, length, glm::vec3(1, 0, 1), true);
    AddMeshToList(engineBlock);

    Mesh* bumperBlock = object2D::CreateBumper("bumperBlock", corner, length, glm::vec3(1, 1, 1), true);
    AddMeshToList(bumperBlock);
    // end

    // Projectile
    Mesh* projectile = object2D::CreateProjectile("projectile", corner, length, glm::vec3(0, 1, 0), true);
    AddMeshToList(projectile);
    // end

    Mesh* enemy = object2D::CreateChicken("enemy", corner, glm::vec3(0, 1, 0));
    AddMeshToList(enemy);

    // Grid and componenet panel
    grid = vector<vector<Block>>(gridHeight, vector<Block>(gridWidth));
    leftPanelBlocks = { BlockType::SOLID, BlockType::ENGINE, BlockType::CANNON, BlockType::BUMPER };
    // end

    // Game
    inGarage = true;

    shootInterval = 2.0f;
    timeSinceLastShoot = 0.0f;

    gameOver = false;
    score = 0;
    wave = 0;

    auto resolution = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 48);

    shipSpeedCurrent = shipSpeedBase;
    projectileSpeedCurrent = projectileSpeedBase;

    shipSpeedLevel = 0;
    projectileSpeedLevel = 0;
    //end
}


// ====================================================================
// 2D visualization matrix (same scale factor on x and y axes)
// ====================================================================
glm::mat3 ChickenInvaders::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float tx = viewSpace.x - sx * logicSpace.x;
    float ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// ====================================================================
// Uniform 2D visualization matrix (same scale factor on x and y axes)
// ====================================================================
glm::mat3 ChickenInvaders::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float smin = min(sx, sy);
    float tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    float ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


// ====================================================================
// Viewport
// ====================================================================
void ChickenInvaders::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    if (clear) glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width),
        (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


// ====================================================================
// Frame Start
// ====================================================================
void ChickenInvaders::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// ====================================================================
// Update
// ====================================================================
void ChickenInvaders::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window (lab)
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute the 2D visualization matrix (lab)
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

    if (gameOver) {
        DrawEndScreen(visMatrix);
        return;
    }
    else if (inGarage) {
        DrawScene(visMatrix);
        DrawStartButton(visMatrix);
    } else {
        RenderGame();
        UpdateEnemyShooting(deltaTimeSeconds);
        UpdateEnemyMovement(deltaTimeSeconds);

        // ***** SCOREBOARD AND INFO *****
        std::string scoreText = "Score: " + std::to_string(score);
        std::string enemiesText = "Enemies: " + std::to_string(enemies.size());
        textRenderer->RenderText(scoreText, 10, 10, 0.5f, glm::vec3(1, 1, 1));
        textRenderer->RenderText(enemiesText, 10, 50, 0.5f, glm::vec3(1, 1, 1));

        // ***** UPGRADES INFO *****
        std::string shipSpeedText = "Ship Speed Level: " + std::to_string((int) shipSpeedLevel);
        std::string projectileSpeedText = "Projectile Speed Level: " + std::to_string((int) projectileSpeedLevel);
        textRenderer->RenderText(shipSpeedText, 900, 10, 0.5f, glm::vec3(1, 1, 1));
        textRenderer->RenderText(projectileSpeedText, 900, 50, 0.5f, glm::vec3(1, 1, 1));

        // ***** PARTICLES *****
        for (size_t i = 0; i < particles.size(); ) {
            particles[i].position += particles[i].velocity * deltaTimeSeconds;
            particles[i].lifetime -= deltaTimeSeconds;

            if (particles[i].lifetime <= 0) {
                particles.erase(particles.begin() + i);
            }
            else {
                ++i;
            }
        }

        // ***** PROJECTILE UPDATE - SHIP + FAILSTATE 1 - DEAD *****
        for (size_t i = 0; i < projectiles.size(); ) {
            projectiles[i].position += projectiles[i].direction * projectiles[i].speed * deltaTimeSeconds;

            // Creare bounding box proiectil
            AABB projBox = { projectiles[i].position, projectiles[i].position + glm::vec2(projectiles[i].size) };
            bool hit = false;

            // Verificare coliziune cu inamici
            for (size_t j = 0; j < enemies.size(); j++) {
                AABB enemyBox = GetEnemyAABB(enemies[j]);
                if (CheckCollision(projBox, enemyBox)) {
                    SpawnParticles(enemies[j].position);

                    enemies.erase(enemies.begin() + j);
                    hit = true;
                    score += 10;
                    break;
                }
            }

            if (hit || projectiles[i].position.y > logicSpace.height) {
                projectiles.erase(projectiles.begin() + i);
            }
            else {
                ++i;
            }
        }

        // ***** PROJECTILE UPDATE - CHICKEN *****
        AABB shipBox = GetShipAABB();
        for (size_t i = 0; i < enemyProjectiles.size(); ) {
            enemyProjectiles[i].position += enemyProjectiles[i].direction * enemyProjectiles[i].speed * deltaTimeSeconds;

            AABB projBox = { enemyProjectiles[i].position, enemyProjectiles[i].position + glm::vec2(enemyProjectiles[i].size) };
            if (CheckCollision(projBox, shipBox)) {
                enemyProjectiles.erase(enemyProjectiles.begin() + i);
                gameOver = true;
            }
            else if (enemyProjectiles[i].position.y < 0 ||
                enemyProjectiles[i].position.y > logicSpace.height ||
                enemyProjectiles[i].position.x < 0 ||
                enemyProjectiles[i].position.x > logicSpace.width) {
                enemyProjectiles.erase(enemyProjectiles.begin() + i);
            }
            else {
                ++i;
            }
        }

        // ***** FAIL STATE 2 - HERE COMES THE ROOSTER ***** 
        for (auto& e : enemies) {
            if (e.position.y - e.size <= 0) {
                gameOver = true;
                break;
            }
        }

        // ***** NEXT WAVE ***** 
        if (enemies.empty()) {
            wave++;
            projectiles.clear();
            enemyProjectiles.clear();
            SpawnEnemies();
        }
    }
}


// ====================================================================
// Frame End
// ====================================================================
void ChickenInvaders::FrameEnd()
{
}


// ====================================================================
// Draw Scene
// ====================================================================
void ChickenInvaders::DrawScene(glm::mat3 visMatrix)
{
    // ***** Left Panel *****
    for (int i = 0; i < 4; i++) {
        // background
        modelMatrix = visMatrix * transform2D::Translate(0, 4.5f * i);
        modelMatrix *= transform2D::Scale(8.0f, 4.5f);
        RenderMesh2D(meshes["sidebarSquare"], shaders["VertexColor"], modelMatrix);

        // component previews
        glm::mat3 cellMatrix = visMatrix * transform2D::Translate(3.0f, 4.5f * i + 1.2f);
        cellMatrix *= transform2D::Scale(2.0f, 2.0f);

        switch (leftPanelBlocks[i]) {
            case BlockType::SOLID:
                RenderMesh2D(meshes["solidBlock"], shaders["VertexColor"], cellMatrix);
                break;
            case BlockType::ENGINE:
                cellMatrix *= transform2D::Translate(0.0f, 0.3f);
                RenderMesh2D(meshes["engineBlock"], shaders["VertexColor"], cellMatrix);
                break;
            case BlockType::CANNON:
                cellMatrix *= transform2D::Scale(0.6f, 0.6f);
                cellMatrix *= transform2D::Translate(0.3f, -0.8f);
                RenderMesh2D(meshes["cannonBlock"], shaders["VertexColor"], cellMatrix);
                break;
            case BlockType::BUMPER:
                cellMatrix *= transform2D::Scale(0.8f, 0.8f);
                cellMatrix *= transform2D::Translate(0.1f, -0.4f);
                RenderMesh2D(meshes["bumperBlock"], shaders["VertexColor"], cellMatrix);
                break;
            default:
                break;
        }
    }

    // ***** Component count *****
    for (int i = 0; i < blocksLeft; i++) {
        modelMatrix = visMatrix * transform2D::Translate(9.0f + 2.0f * i, 16.0f);
        modelMatrix *= transform2D::Scale(1.5f, 1.5f);
        RenderMesh2D(meshes["componentCountSquare"], shaders["VertexColor"], modelMatrix);
    }

    // ***** Grid Delimiter *****
    modelMatrix = visMatrix * transform2D::Translate(8.5f, 0.5f);
    modelMatrix *= transform2D::Scale(23.3f, 14.3f);
    RenderMesh2D(meshes["gridDelimiterSquare"], shaders["VertexColor"], modelMatrix);

    // ***** Grids squares *****
    float cellSize = 1.5f;
    float blockSize = 1.0f;
    float offset = (cellSize - blockSize) / 2.0f;

    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            glm::mat3 cellMatrix = visMatrix * transform2D::Translate(9.0f + cellSize * j, 1.0f + cellSize * i);

            if (grid[i][j].type == BlockType::NONE) {
                RenderMesh2D(meshes["gridSquare"], shaders["VertexColor"], cellMatrix);
            } else {
                // Patratele grid-urilor sunt mai mici,
                // tin cont de offset si scale pentru componente aici
                cellMatrix *= transform2D::Translate(-offset, -offset);
                cellMatrix *= transform2D::Scale(cellSize + 0.009f, cellSize + 0.009f);
                
                switch (grid[i][j].type) {
                    case BlockType::SOLID:
                        RenderMesh2D(meshes["solidBlock"], shaders["VertexColor"], cellMatrix);
                        break;
                    case BlockType::ENGINE:
                        RenderMesh2D(meshes["engineBlock"], shaders["VertexColor"], cellMatrix);
                        break;
                    case BlockType::CANNON:
                        RenderMesh2D(meshes["cannonBlock"], shaders["VertexColor"], cellMatrix);
                        break;
                    case BlockType::BUMPER:
                        RenderMesh2D(meshes["bumperBlock"], shaders["VertexColor"], cellMatrix);
                        break;
                    default: break;
                }
            }
        }
    }

    // ***** Ghost for dragging *****
    if (isDragging) {
        glm::mat3 dragMatrix = visMatrix * transform2D::Translate(mousePos.x, mousePos.y);
        dragMatrix *= transform2D::Scale(1.0f, 1.0f);
        switch (draggedBlock) {
        case BlockType::SOLID:
            RenderMesh2D(meshes["solidBlock"], shaders["VertexColor"], dragMatrix);
            break;
        case BlockType::CANNON:
            RenderMesh2D(meshes["cannonBlock"], shaders["VertexColor"], dragMatrix);
            break;
        case BlockType::ENGINE:
            RenderMesh2D(meshes["engineBlock"], shaders["VertexColor"], dragMatrix);
            break;
        case BlockType::BUMPER:
            RenderMesh2D(meshes["bumperBlock"], shaders["VertexColor"], dragMatrix);
            break;
        default: break;
        }
    }
}


// =========================================================
// Draw Start Button
// =========================================================
void ChickenInvaders::DrawStartButton(glm::mat3 visMatrix) {
    Mesh* startButton = IsGridValid() ? meshes["validStartButton"] : meshes["invalidStartButton"];
    modelMatrix = visMatrix * transform2D::Translate(29.5f, 16.0f);
    modelMatrix *= transform2D::Scale(2.0f, 1.5f);
    RenderMesh2D(startButton, shaders["VertexColor"], modelMatrix);
}


// =========================================================
// Mouse Drag and Drop Helpers
// =========================================================

// ***** ConvertScreenToLogic *****
// Converteste coordonatele ecranului in coordonate logice pentru scena
glm::vec2 ChickenInvaders::ConvertScreenToLogic(int mouseX, int mouseY) {
    float logicX = (mouseX / (float)window->GetResolution().x) * logicSpace.width;
    float logicY = (1.0f - mouseY / (float)window->GetResolution().y) * logicSpace.height;
    return glm::vec2(logicX, logicY);
}

// ***** GetGridCell *****
// Returneaza coordonatele celulei din grila corespunzatoare pozitiei mouse-ului
glm::ivec2 ChickenInvaders::GetGridCell(int mouseX, int mouseY) {
    glm::vec2 pos = ConvertScreenToLogic(mouseX, mouseY);

    int x = (int)((pos.x - 9.0f) / 1.5f); // calculeaza indexul pe coloana
    int y = (int)((pos.y - 1.0f) / 1.5f); // calculeaza indexul pe rand

    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return glm::ivec2(-1, -1);

    return glm::ivec2(x, y);
}

// ***** GetLeftPanelCell *****
// Verificam daca se poate selecta ceva din panoul din stanga
int ChickenInvaders::GetLeftPanelCell(int mouseX, int mouseY) {
    glm::vec2 logicMouse = ConvertScreenToLogic(mouseX, mouseY);

    float panelXMin = 0.0f;
    float panelXMax = 8.0f;
    float panelYMin = 0.0f;
    float panelYMax = 18.0f;

    // boundry check daca cursor-ul este in afara panoului
    if (logicMouse.x < panelXMin || logicMouse.x > panelXMax ||
        logicMouse.y < panelYMin || logicMouse.y > panelYMax)
        return -1;

    // calculam indexul pe verticala
    float cellHeight = 4.5f;
    int index = (int)((logicMouse.y) / cellHeight);
    if (index < 0 || index > 3)
        return -1;

    return index;
}

// ***** IsValidPlacement *****
// Verifica daca o componenta poate fi plasata intr-o anumita celula
bool ChickenInvaders::IsValidPlacement(glm::ivec2 cell, BlockType type) {
    // Daca celula este invalida sau deja ocupata, nu se poate plasa
    if (cell.x < 0 || cell.x >= gridWidth || cell.y < 0 || cell.y >= gridHeight)
        return false;

    // Celula trebuie sa fie libera
    if (grid[cell.y][cell.x].type != BlockType::NONE)
        return false;
        
    // Orice alta plasare este permisa
    return true;
}

// ***** IsGridValid *****
// Verifica conditiile de valabilite pentru butonul de start
bool ChickenInvaders::IsGridValid() {
    int totalBlocks = 0;
    glm::ivec2 enginePos(-1, -1);
    glm::ivec2 cannonPos(-1, -1);

    // Contorizeaza blocurile si gaseste pozitia motorului si a tunului
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            BlockType type = grid[y][x].type;
            if (type != BlockType::NONE) {
                totalBlocks++;
                if (type == BlockType::ENGINE) enginePos = glm::ivec2(x, y);
                if (type == BlockType::CANNON) cannonPos = glm::ivec2(x, y);
            }
        }
    }

    // Vehiculul trebuie sa aiba cel putin 1 bloc si maxim 10
    if (totalBlocks == 0 || totalBlocks > 10) return false;

    // Verifica conexiunea blocurilor (BFS)
    std::vector<std::vector<bool>> visited(gridHeight, std::vector<bool>(gridWidth, false));
    std::queue<glm::ivec2> q;

    // Gasim primul bloc ca punct de start
    bool found = false;
    for (int y = 0; y < gridHeight && !found; y++) {
        for (int x = 0; x < gridWidth && !found; x++) {
            if (grid[y][x].type != BlockType::NONE) {
                q.push(glm::ivec2(x, y));
                visited[y][x] = true;
                found = true;
            }
        }
    }

    int connectedCount = 0;
    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0, 0, 1, -1 };

    while (!q.empty()) {
        glm::ivec2 cur = q.front();
        q.pop();
        connectedCount++;

        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];
            if (nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                if (!visited[ny][nx] && grid[ny][nx].type != BlockType::NONE) {
                    visited[ny][nx] = true;
                    q.push(glm::ivec2(nx, ny));
                }
            }
        }
    }

    if (connectedCount != totalBlocks) return false;

    // Engine, trebuie sa nu aiba nimic sub el
    if (enginePos.x != -1) {
        for (int y = 0; y < enginePos.y; y++)
            if (grid[y][enginePos.x].type != BlockType::NONE) return false;
    }

    // Cannon, trebuie sa nu aiba nimic deaspura
    if (cannonPos.x != -1) {
        for (int y = cannonPos.y + 1; y < gridHeight; y++)
            if (grid[y][cannonPos.x].type != BlockType::NONE) return false;

        // M-am gandit ca nu ar fi corect sa se afla tunul absolut oriunde sub un bumper
        for (int dx = -1; dx <= 1; dx++) {
            int nx = cannonPos.x + dx;
            if (nx < 0 || nx >= gridWidth) continue;
            for (int y = cannonPos.y + 1; y < gridHeight; y++) {
                if (grid[y][nx].type == BlockType::BUMPER) return false;
            }
        }
    }

    // Bumper, trebuie sa nu aiba nimic deasupra*
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            if (grid[y][x].type == BlockType::BUMPER) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    if (nx >= 0 && nx < gridWidth) {
                        if (dx != 0 && grid[y][nx].type == BlockType::BUMPER) return false;
                        if (grid[y][nx].type == BlockType::CANNON) return false;
                        //if (grid[y][nx].type == BlockType::SOLID) return false;
                        //if (grid[y][nx].type == BlockType::ENGINE) return false;
                    }
                }

                // Sa nu fie nimic pe cele 3 col deaspura bumperului 
                // (*nu am considerat ca doua bumpere cu overlap doar la margini se pun)
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = x + dx;
                    if (nx < 0 || nx >= gridWidth) continue;
                    for (int yy = y + 1; yy < gridHeight; yy++) {
                        if (grid[yy][nx].type != BlockType::NONE)  return false;
                    }
                }
            }
        }
    }

    return true; // totul este valid
}


// =========================================================
// Game Functions
// =========================================================

// ***** RenderGame *****
// Folisit pentru a randa elemente jocului propriuzis
void ChickenInvaders::RenderGame() {
    // ***** SHIP RENDERING ***** 
    float blockSize = 1.5f;
    for (size_t i = 0; i < ship.blocks.size(); i++) {
        glm::vec2 blockPos = ship.position + ship.offsets[i] * blockSize;
        modelMatrix = visMatrix * transform2D::Translate(blockPos.x, blockPos.y);
        modelMatrix *= transform2D::Scale(blockSize, blockSize);

        switch (ship.blocks[i].type) {
            case BlockType::SOLID: RenderMesh2D(meshes["solidBlock"], shaders["VertexColor"], modelMatrix); break;
            case BlockType::ENGINE: RenderMesh2D(meshes["engineBlock"], shaders["VertexColor"], modelMatrix); break;
            case BlockType::CANNON: RenderMesh2D(meshes["cannonBlock"], shaders["VertexColor"], modelMatrix); break;
            case BlockType::BUMPER: RenderMesh2D(meshes["bumperBlock"], shaders["VertexColor"], modelMatrix); break;
            default: break;
        }
    }

    // ***** PROJECTILE RENDERING ***** 
    for (auto& p : projectiles) {
        modelMatrix = visMatrix * transform2D::Translate(p.position.x + 0.7f, p.position.y);
        modelMatrix *= transform2D::Scale(p.size, p.size);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }

    // ***** CHICKEN RENDERING ***** 
    for (auto& e : enemies) {
        modelMatrix = visMatrix * transform2D::Translate(e.position.x, e.position.y);
        modelMatrix *= transform2D::Scale(e.size * 2, e.size * 2);
        RenderMesh2D(meshes[e.meshName], shaders["VertexColor"], modelMatrix);
    }

    // ***** ENEMY PROJECTILE RENDERING ***** 
    for (auto& p : enemyProjectiles) {
        modelMatrix = visMatrix * transform2D::Translate(p.position.x, p.position.y);
        modelMatrix *= transform2D::Scale(p.size, p.size);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }

    // ***** PARTICLE RENDERING ***** 
    for (auto& p : particles) {
        modelMatrix = visMatrix * transform2D::Translate(p.position.x, p.position.y);
        modelMatrix *= transform2D::Scale(p.size, p.size);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    }
}

// ***** ShootProjectile *****
// Calculeaza variabilele pentru proiectilele user-ului
void ChickenInvaders::ShootProjectile() {
    float blockSize = 1.5f;
    for (size_t i = 0; i < ship.blocks.size(); i++) {
        if (ship.blocks[i].type == BlockType::CANNON) {
            glm::vec2 projPos = ship.position + ship.offsets[i] * blockSize + glm::vec2(blockSize / 2.0f);
            Projectile p;
            p.position = projPos;
            p.direction = glm::vec2(0, 1);
            p.speed = projectileSpeedCurrent;
            projectiles.push_back(p);
        }
    }
}

// ***** EnemyShootProjectile *****
// Calculeaza variabilele pentru proiectilele inamicului
void ChickenInvaders::EnemyShootProjectile(const Enemy& e) {
    Projectile p;
    p.position = e.position;

    glm::vec2 shipCenter = ship.position;
    p.direction = glm::vec2(0, -1);

    p.speed = 8.0f;
    p.isEnemy = true;

    enemyProjectiles.push_back(p);
}

// ***** SpawnEnemies *****
// Gestioneaza usor sistemul de runde
void ChickenInvaders::SpawnEnemies() {
    enemies.clear();
    int rows = std::min(1 + wave / 2, 5);
    int cols = std::min(5 + wave * 2, 12);
    float startX = 10.0f;
    float startY = logicSpace.height - 3.0f;
    float spacing = 8.0f;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Enemy e;
            e.position = glm::vec2(startX + c * spacing, startY - r * spacing);
            e.speed = 5.0f + 0.5f * wave;
            e.meshName = "enemy";
            enemies.push_back(e);
        }
    }
}

// ***** UpdateEnemyMovement *****
// Calculeaza drumul sinusoidal pe care il vor urma inamicii 
void ChickenInvaders::UpdateEnemyMovement(float deltaTimeSeconds) {
    static float timeElapsed = 0.0f;
    timeElapsed += deltaTimeSeconds;

    float amplitude = 2.0f;
    float frequency = 1.0f;
    float speedY = 1.5f;

    for (auto& e : enemies) {
        e.position.y -= speedY * deltaTimeSeconds;
        e.position.x += amplitude * sin(frequency * timeElapsed + e.position.y) * deltaTimeSeconds;
    }
}

// ***** UpdateEnemyShooting *****
// Calculeaza un interval randomizat in care vor trage inamicii
void ChickenInvaders::UpdateEnemyShooting(float deltaTime) {
    timeSinceLastShoot += deltaTime;
    if (timeSinceLastShoot >= shootInterval) {
        for (auto& e : enemies) {
            if (rand() % 2 == 0) {
                EnemyShootProjectile(e);
            }
        }
        timeSinceLastShoot = 0.0f;
    }
}

// ***** CheckCollision *****
// Verifica daca doua AABB-uri se intersecteaza
bool ChickenInvaders::CheckCollision(const AABB& a, const AABB& b) {
    if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
    if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
    return true;
}

// ***** GetShipAABB *****
// Returneaza AABB-ul navei, calculat in functie de pozitia si dimensiunile blocurilor
ChickenInvaders::AABB ChickenInvaders::GetShipAABB() {
    float blockSize = 1.5f;

    glm::vec2 min(FLT_MAX, FLT_MAX);
    glm::vec2 max(-FLT_MAX, -FLT_MAX);

    for (auto& offset : ship.offsets) {
        glm::vec2 pos = ship.position + offset * blockSize;
        glm::vec2 blockMax = pos + glm::vec2(blockSize);
        min.x = std::min(min.x, pos.x);
        min.y = std::min(min.y, pos.y);
        max.x = std::max(max.x, blockMax.x);
        max.y = std::max(max.y, blockMax.y);
    }

    AABB box;
    box.min = min;
    box.max = max;
    return box;
}

// ***** GetEnemyAABB *****
// Returneaza AABB-ul unui inamic
ChickenInvaders::AABB ChickenInvaders::GetEnemyAABB(const Enemy& e) {
    float size = e.size * 2.0f;

    float paddingX = 1.0f;
    float paddingY = 0.3f;

    glm::vec2 min = e.position - glm::vec2(size / 2.0f + paddingX, size / 2.0f);
    glm::vec2 max = e.position + glm::vec2(size / 2.0f, size / 2.0f);

    AABB box;
    box.min = min;
    box.max = max;
    return box;
}

// ***** SpawnParticles *****
// Creeaza particule atunci cand un inamic este distrus
void ChickenInvaders::SpawnParticles(glm::vec2 pos) {
    int numParticles = 10;
    for (int i = 0; i < numParticles; i++) {
        Particle p;
        p.position = pos;
        float angle = ((rand() % 360) / 180.0f) * 3.14159f;
        float speed = 2.0f + (rand() % 100) / 50.0f;
        p.velocity = glm::vec2(cos(angle), sin(angle)) * speed;
        p.lifetime = 0.5f + (rand() % 50) / 100.0f;
        p.size = 0.2f + (rand() % 10) / 50.0f;
        particles.push_back(p);
    }
}

// ***** DrawEndScreen *****
// Afiseaza ecranul de Game Over si scorul obtinut
void ChickenInvaders::DrawEndScreen(glm::mat3 visMatrix)
{
    std::string text = "GAME OVER";
    std::string text2 = "Score: " + std::to_string(score);
    std::string text3 = "Press SPACE to continue";

    textRenderer->RenderText(text, 300, 400, 1.5f, glm::vec3(1, 0, 0));
    textRenderer->RenderText(text2, 300, 340, 1.0f, glm::vec3(1, 1, 1));
    textRenderer->RenderText(text3, 300, 260, 0.8f, glm::vec3(1, 1, 1));
}


// =========================================================
// Inputs - These are callback functions.
// =========================================================
void ChickenInvaders::OnInputUpdate(float deltaTime, int mods)
{
    if (inGarage) return;

    float move = shipSpeedCurrent * deltaTime;
    if (window->KeyHold(GLFW_KEY_W)) ship.position.y += move;
    if (window->KeyHold(GLFW_KEY_S)) ship.position.y -= move;
    if (window->KeyHold(GLFW_KEY_A)) ship.position.x -= move;
    if (window->KeyHold(GLFW_KEY_D)) ship.position.x += move;

    float margin = 0.5f;
    ship.position.x = glm::clamp(ship.position.x, margin, logicSpace.width - margin);
    ship.position.y = glm::clamp(ship.position.y, margin, logicSpace.height - margin);
}


void ChickenInvaders::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        ShootProjectile();
    }

    // ***** ROUND RESET KEY *****
    if (key == GLFW_KEY_SPACE && gameOver) {
        gameOver = false;
        inGarage = true;
        enemies.clear();
        enemyProjectiles.clear();
        projectiles.clear();
        score = 0;
        wave = 0;
        logicSpace = LogicSpace(0, 0, 32, 18);

        shipSpeedCurrent = shipSpeedBase;
        projectileSpeedCurrent = projectileSpeedBase;

        shipSpeedLevel = 0;
        projectileSpeedLevel = 0;
    }

    // ***** UPGRADE KEYS *****
    if (score >= COST_SHIP_SPEED && key == GLFW_KEY_1) {
        score -= COST_SHIP_SPEED;
        shipSpeedLevel++;
        shipSpeedCurrent = shipSpeedBase + 1.0f * shipSpeedLevel;
    }

    if (score >= COST_PROJECTILE_SPEED && key == GLFW_KEY_2) {
        score -= COST_PROJECTILE_SPEED;
        projectileSpeedLevel++;
        projectileSpeedCurrent = projectileSpeedBase + 2.0f * projectileSpeedLevel;
    }
}


void ChickenInvaders::OnKeyRelease(int key, int mods)
{

}


void ChickenInvaders::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    mousePos = ConvertScreenToLogic(mouseX, mouseY);
}

// ***** OnMouseBtnPress *****
// Este apelata cand un buton de mouse este apasat
void ChickenInvaders::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Daca se tine apasat butonul stang
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        glm::vec2 logicMouse = ConvertScreenToLogic(mouseX, mouseY);

        // Verifica daca s-a luat o componenta din panoul din stanga
        int leftPanelIndex = GetLeftPanelCell(mouseX, mouseY);
        if (leftPanelIndex != -1) {
            // Incepe drag and drop
            isDragging = true;
            draggedBlock = leftPanelBlocks[leftPanelIndex];
            return;
        }
    } 
    // Daca se tine apasat butonul drept
    else if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::ivec2 cell = GetGridCell(mouseX, mouseY);

        // Daca celula este valida si contine un bloc, il sterge
        if (cell.x != -1 && grid[cell.y][cell.x].type != BlockType::NONE) {
            grid[cell.y][cell.x].type = BlockType::NONE;
            blocksLeft++;
            if (blocksLeft > 10) blocksLeft = 10; // Sa nu se dea pe afara la stergeri extra
        }
    }
}

// ***** OnMouseBtnRelease *****
// Este apelata cand un buton de mouse este eliberat
void ChickenInvaders::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Se elibereaza butonul stang si era in modul drag
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && isDragging) {
        glm::ivec2 cell = GetGridCell(mouseX, mouseY);
        // Daca celula este valida si plasarea este permisa, plaseaza blocul
        if (cell.x != -1 && IsValidPlacement(cell, draggedBlock)) {
            grid[cell.y][cell.x].type = draggedBlock;
            blocksLeft--;
        }
        // Altfel nu se intampla nimic si blocul este eliberat
        isDragging = false;
    }


    // ***** START GAME BUTTON *****
    glm::vec2 logicMouse = ConvertScreenToLogic(mouseX, mouseY);
    float btnX = 29.5f, btnY = 16.0f, btnW = 2.0f, btnH = 1.5f;
    if (logicMouse.x >= btnX && logicMouse.x <= btnX + btnW &&
        logicMouse.y >= btnY && logicMouse.y <= btnY + btnH)
    {
        if (IsGridValid()) {
            inGarage = false;
            
            // Am dublat rezolutia
            logicSpace = LogicSpace(0, 0, 108, 63);

            // Setez pozitia in centrul ecranului
            ship.position = glm::vec2(logicSpace.width / 2, logicSpace.height / 2);

            // Transferam blocurile din grid in joc
            ship.blocks.clear();
            ship.offsets.clear();
            for (int y = 0; y < gridHeight; y++) {
                for (int x = 0; x < gridWidth; x++)
                    if (grid[y][x].type != BlockType::NONE) {
                        ship.blocks.push_back(grid[y][x]);
                        ship.offsets.push_back(glm::vec2(x, y));
                    }
            }

            // Centra blocurile in jurul navei
            glm::vec2 center(0.0f);
            for (auto& o : ship.offsets) center += o;
            center /= (float)ship.offsets.size();
            for (auto& o : ship.offsets) o -= center;

            // Spawnez inamicii
            SpawnEnemies();
        }
    }
}


void ChickenInvaders::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
