#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

// =========================================================
// Basic Square - grid, left panel etc.
// =========================================================
Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

// =========================================================
// Unused Triangle
// =========================================================
Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float sideLength,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, sideLength, 0), color)
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

// =========================================================
// Start Button
// =========================================================
Mesh* object2D::CreateStartButton(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices =
    { 
        0, 3, 2,
        0, 1, 3
    };

    if (!fill) {
        square->SetDrawMode(GL_TRIANGLES);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

// =========================================================
// Engine Block
// =========================================================
Mesh* object2D::CreateEngine(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    float flameWidth = length / 3.0f;
    float flameHeight = length / 2.0f;
    glm::vec3 baseColor = glm::vec3(0.9f, 0.5f, 0);
    glm::vec3 flameColor = glm::vec3(1.0f, 0.5f, 0.0f);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, baseColor),
        VertexFormat(corner + glm::vec3(1, 0, 0), baseColor),
        VertexFormat(corner + glm::vec3(1, 1, 0), baseColor),
        VertexFormat(corner + glm::vec3(0, 1, 0), baseColor),

        VertexFormat(corner + glm::vec3(0, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(flameWidth, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(flameWidth / 2 - 0.2f, -flameHeight, 0), flameColor),

        VertexFormat(corner + glm::vec3(flameWidth, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(2 * flameWidth, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(1.5f * flameWidth, -flameHeight, 0), flameColor),

        VertexFormat(corner + glm::vec3(2 * flameWidth, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(3 * flameWidth, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(2.5f * flameWidth + 0.2f, -flameHeight, 0), flameColor),

        VertexFormat(corner + glm::vec3(flameWidth * 0.75f, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(flameWidth * 1.25f, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(flameWidth, -flameHeight * 0.8f, 0), flameColor),

        VertexFormat(corner + glm::vec3(flameWidth * 1.75f, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(flameWidth * 2.25f, 0, 0), flameColor),
        VertexFormat(corner + glm::vec3(2 * flameWidth, -flameHeight * 0.8f, 0), flameColor)
    };

    Mesh* engine = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,  2, 3, 0,
        4, 5, 6,
        7, 8, 9,
        10, 11, 12,
        13, 14, 15,
        16, 17, 18
    };

    engine->SetDrawMode(GL_TRIANGLES);
    engine->InitFromData(vertices, indices);
    return engine;
}

// =========================================================
// Solid Block
// =========================================================
Mesh* object2D::CreateSolidBlock(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    glm::vec3 lightGray(0.8f, 0.8f, 0.8f);
    glm::vec3 darkGray(0.4f, 0.4f, 0.4f);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, lightGray),
        VertexFormat(corner + glm::vec3(length, 0, 0), darkGray),
        VertexFormat(corner + glm::vec3(length, length, 0), lightGray),
        VertexFormat(corner + glm::vec3(0, length, 0), darkGray),
        VertexFormat(glm::vec3(length / 2, length / 2 , 0), glm::vec3(0.6f))
    };

    Mesh* solidBlock = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        solidBlock->SetDrawMode(GL_LINE_LOOP);
        indices = { 0, 1, 2, 3 };
    } else {
        indices = {
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
        };
    }

    solidBlock->InitFromData(vertices, indices);
    return solidBlock;
}

// =========================================================
// Bumper Block
// =========================================================
Mesh* object2D::CreateBumper(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    Mesh* bumper = new Mesh(name);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Base
    vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    if (!fill) {
        bumper->SetDrawMode(GL_LINE_LOOP);
        indices = { 0, 1, 2, 3 };
    }
    else {
        indices = { 0, 1, 2, 2, 3, 0 };
    }

    // Semicircle
    int numSegments = 20;
    float radius = 1.5f;
    glm::vec3 center = corner + glm::vec3(length / 2.0f, length, 0);

    int startIndex = vertices.size();
    vertices.push_back(VertexFormat(center, glm::vec3(0.8f, 0.6f, 0.6f)));

    for (int i = 0; i <= numSegments; i++) {
        float angle = M_PI * i / numSegments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle) * 0.7f;
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), glm::vec3(0.7f, 0.6f, 0.4f)));

        if (fill && i > 0) {
            indices.push_back(startIndex);
            indices.push_back(startIndex + i);
            indices.push_back(startIndex + i + 1);
        }
    }

    bumper->InitFromData(vertices, indices);
    return bumper;
}

// =========================================================
// Cannon Block
// =========================================================
Mesh* object2D::CreateCannon(const std::string& name, glm::vec3 corner, float length, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 baseColor = glm::vec3(0.8f);

    float radius = 0.5f;
    float yScale = 0.5f;

    float baseHeight = 0.8f;
    float barrelHeight = 2.2f;
    float barrelInset = 0.1f;

    // Base
    vertices.push_back(VertexFormat(corner, baseColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), baseColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(length, baseHeight, 0), baseColor));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, baseHeight, 0), baseColor));
    indices.insert(indices.end(), { 0,1,2, 2,3,0 });

    // Semicircle base
    glm::vec3 center = corner + glm::vec3(length / 2.0f, baseHeight, 0);
    int segments = 12;
    int startIndex = vertices.size();
    for (int i = 0; i <= segments; i++) {
        float angle = M_PI * i / segments;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle) * yScale;
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), baseColor));
    }
    for (int i = 0; i < segments; i++) {
        indices.push_back(2);
        indices.push_back(startIndex + i);
        indices.push_back(startIndex + i + 1);
    }

    // Barrel
    glm::vec3 barrelBottomColor = glm::vec3(0.3f);
    glm::vec3 barrelTopColor = glm::vec3(0.4f);

    glm::vec3 barrelBottom = corner + glm::vec3(0, baseHeight, 0);
    vertices.push_back(VertexFormat(barrelBottom, barrelBottomColor));
    vertices.push_back(VertexFormat(barrelBottom + glm::vec3(length, 0, 0), barrelBottomColor));
    vertices.push_back(VertexFormat(barrelBottom + glm::vec3(length - barrelInset, barrelHeight, 0), barrelTopColor));
    vertices.push_back(VertexFormat(barrelBottom + glm::vec3(barrelInset, barrelHeight, 0), barrelTopColor));

    int barrelStart = vertices.size() - 4;
    indices.push_back(barrelStart);
    indices.push_back(barrelStart + 1);
    indices.push_back(barrelStart + 2);
    indices.push_back(barrelStart + 2);
    indices.push_back(barrelStart + 3);
    indices.push_back(barrelStart);

    // Muzzle
    float muzzleLength = 0.1f;
    float muzzleHeight = 0.2f;

    int muzzleStart = vertices.size();
    vertices.push_back(VertexFormat(barrelBottom + glm::vec3(barrelInset, barrelHeight, 0), barrelTopColor));
    vertices.push_back(VertexFormat(barrelBottom + glm::vec3(length - barrelInset, barrelHeight, 0), barrelTopColor));
    vertices.push_back(VertexFormat(
        barrelBottom +
        glm::vec3(length - barrelInset, barrelHeight, 0) +
        glm::vec3(muzzleLength, muzzleHeight, 0),
        barrelTopColor)
    );
    vertices.push_back(VertexFormat(
        barrelBottom + glm::vec3(barrelInset, barrelHeight, 0) +
        glm::vec3(-muzzleLength, muzzleHeight, 0),
        barrelTopColor)
    );

    indices.push_back(muzzleStart);
    indices.push_back(muzzleStart + 1);
    indices.push_back(muzzleStart + 2);
    indices.push_back(muzzleStart + 2);
    indices.push_back(muzzleStart + 3);
    indices.push_back(muzzleStart);

    Mesh* cannon = new Mesh(name);
    cannon->InitFromData(vertices, indices);

    if (!fill) cannon->SetDrawMode(GL_LINE_LOOP);

    return cannon;
}

Mesh* object2D::CreateProjectile(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    int numTriangles = 30;
    for (int i = 0; i <= numTriangles; i++) {
        float theta = 2.0f * glm::pi<float>() * i / numTriangles;
        float x = center.x + radius * cos(theta);
        float y = center.y + radius * sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        if (i > 0) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }

    Mesh* projectile = new Mesh(name);
    projectile->InitFromData(vertices, indices);
    projectile->SetDrawMode(GL_TRIANGLES);
    return projectile;
}

Mesh* object2D::CreateChicken(
    const std::string& name,
    glm::vec3 position,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // ***** CORP *****
    float bodySize = 0.6f;
    vertices.push_back(VertexFormat(position + glm::vec3(-bodySize, -bodySize, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(position + glm::vec3(bodySize, -bodySize, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(position + glm::vec3(bodySize, bodySize, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(VertexFormat(position + glm::vec3(-bodySize, bodySize, 0), glm::vec3(0, 0, 1)));
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // ***** CAP *****
    int tris = 20;
    float radius = 0.4f;
    glm::vec3 capPos = position + glm::vec3(0.1f, bodySize + radius, -0.1f);
    int startIndex = vertices.size();
    for (int i = 0; i < tris; i++) {
        float angle = 2 * M_PI * i / tris;
        vertices.push_back(VertexFormat(capPos + glm::vec3(cos(angle) * radius, sin(angle) * radius, 0), glm::vec3(0.5f, 0.5f, 0.5f)));
    }
    vertices.push_back(VertexFormat(capPos, glm::vec3(0.5f, 0.5f, 0.5f)));
    int center = vertices.size() - 1;
    for (int i = 0; i < tris; i++) {
        indices.push_back(startIndex + i);
        indices.push_back(startIndex + (i + 1) % tris);
        indices.push_back(center);
    }

    // ***** CREASTA *****
    float crestWidth = 0.3f;
    float crestHeight = 0.25f;

    startIndex = vertices.size();
    // creasta stanga
    vertices.push_back(VertexFormat(capPos + glm::vec3(-crestWidth / 2, radius - 0.3f, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(crestWidth / 2, radius - 0.2f, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(0, radius + crestHeight, 0), glm::vec3(1, 0, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    startIndex = vertices.size();
    // creasta dreapta
    vertices.push_back(VertexFormat(capPos + glm::vec3(-crestWidth / 2 - 0.1f, radius - 0.3f, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(crestWidth / 2, radius - 0.2f, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(0.3f, radius + crestHeight, 0), glm::vec3(1, 0, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    // ***** CIOC *****
    vertices.push_back(VertexFormat(capPos + glm::vec3(-0.1f, -0.1f, 0.1f), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(0.0f, 0.1f, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(capPos + glm::vec3(0.3f, -0.2f, 0), glm::vec3(1, 0.5f, 0)));
    int beakStart = vertices.size() - 3;
    indices.push_back(beakStart); indices.push_back(beakStart + 1); indices.push_back(beakStart + 2);

    float eyeRadius = 0.07f;
    glm::vec3 leftEye = capPos + glm::vec3(-0.2f, 0.1f, 0.1f);
    glm::vec3 rightEye = capPos + glm::vec3(0.2f, 0.12f, 0.1f);
    auto addEye = [&](glm::vec3 centerPos) {
        int startIdx = vertices.size();
        for (int i = 0; i < tris; i++) {
            float angle = 2 * M_PI * i / tris;
            vertices.push_back(VertexFormat(centerPos + glm::vec3(cos(angle) * eyeRadius, sin(angle) * eyeRadius, 0), glm::vec3(0, 0, 0)));
        }
        vertices.push_back(VertexFormat(centerPos, glm::vec3(0, 0, 0)));
        int c = vertices.size() - 1;
        for (int i = 0; i < tris; i++) {
            indices.push_back(startIdx + i);
            indices.push_back(startIdx + (i + 1) % tris);
            indices.push_back(c);
        }
    };
    addEye(leftEye);
    addEye(rightEye);

    // ***** PICIOARE *****
    glm::vec3 leftLegPos = position + glm::vec3(-0.3f, -bodySize - 0.5f, 0.1f);
    glm::vec3 rightLegPos = position + glm::vec3(0.3f, -bodySize - 0.5f, 0.1f);
    float legSize = 0.25f;
    float legWidth = 0.07f;

    // picior stang
    startIndex = vertices.size();
    vertices.push_back(VertexFormat(leftLegPos + glm::vec3(0, 0, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(leftLegPos + glm::vec3(legSize, legSize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(leftLegPos + glm::vec3(-legSize, legSize, 0), glm::vec3(1, 0.5f, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    // picior drept
    startIndex = vertices.size();
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(0, 0, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(legSize, legSize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(-legSize, legSize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(-legSize, legSize, 0), glm::vec3(1, 0.5f, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);

    startIndex = vertices.size();
    // picior stang - 2
    vertices.push_back(VertexFormat(glm::vec3(leftLegPos.x - legWidth, position.y - bodySize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(glm::vec3(leftLegPos.x + legWidth, position.y - bodySize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(leftLegPos + glm::vec3(-legWidth, 0.1f, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(leftLegPos + glm::vec3(legWidth, 0.1f, 0), glm::vec3(1, 0.5f, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);

    startIndex = vertices.size();
    // picior drept - 2
    vertices.push_back(VertexFormat(glm::vec3(rightLegPos.x - legWidth, position.y - bodySize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(glm::vec3(rightLegPos.x + legWidth, position.y - bodySize, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(-legWidth, 0.1f, 0), glm::vec3(1, 0.5f, 0)));
    vertices.push_back(VertexFormat(rightLegPos + glm::vec3(legWidth, 0.1f, 0), glm::vec3(1, 0.5f, 0)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);
    indices.push_back(startIndex + 3);

    // ***** ARIPI *****
    glm::vec3 leftWingPos = position + glm::vec3(-bodySize + 0.1f, 0.2f, 0);
    glm::vec3 rightWingPos = position + glm::vec3(bodySize - 0.1f, 0.2f, 0);
    float wingWidth = 0.6f, wingHeight = 0.4f;

    // aripa stanga
    startIndex = vertices.size();
    vertices.push_back(VertexFormat(leftWingPos + glm::vec3(0, -0.5f, 0.1f), glm::vec3(0.6f, 0.6f, 0.6f)));
    vertices.push_back(VertexFormat(leftWingPos + glm::vec3(-wingWidth - 0.2f, wingHeight + 0.8f, 0), glm::vec3(0.6f, 0.6f, 0.6f)));
    vertices.push_back(VertexFormat(leftWingPos + glm::vec3(-wingWidth - 0.5f, -wingHeight + 0.3f, 0), glm::vec3(0.6f, 0.6f, 0.6f)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);

    // aripa dreapta
    startIndex = vertices.size();
    vertices.push_back(VertexFormat(rightWingPos + glm::vec3(0, -0.5, 0.1f), glm::vec3(0.6f, 0.6f, 0.6f)));
    vertices.push_back(VertexFormat(rightWingPos + glm::vec3(wingWidth + 0.2f, wingHeight + 0.8f, 0), glm::vec3(0.6f, 0.6f, 0.6f)));
    vertices.push_back(VertexFormat(rightWingPos + glm::vec3(wingWidth + 0.5f, -wingHeight + 0.3f, 0), glm::vec3(0.6f, 0.6f, 0.6f)));
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);
    indices.push_back(startIndex + 2);



    Mesh* chicken = new Mesh(name);
    chicken->InitFromData(vertices, indices);
    return chicken;
}