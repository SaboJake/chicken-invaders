#include "object2D.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

#include <glm/gtx/vector_angle.hpp>
#include <vector>

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
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(
    const std::string& name,
    glm::vec3 corner1,
    glm::vec3 corner2,
    glm::vec3 corner3,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color)
	};

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };
    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* object2D::CreateSemicircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill,
    int segments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    
    vertices.push_back(VertexFormat(center, color));

    for (int i = 0; i <= segments; i++) {
        float theta = glm::pi<float>() * i / segments + glm::pi<float>();
        float x = center.x + radius * cos(theta);
        float y = center.y + radius * sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }
    for (int i = 1; i <= segments; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    Mesh* semicircle = new Mesh(name);
    if (!fill) {
        semicircle->SetDrawMode(GL_LINE_LOOP);
    }
    semicircle->InitFromData(vertices, indices);
    return semicircle;
}

Mesh* object2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    bool fill,
    int segments)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i < segments; i++) {
		float theta = glm::pi<float>() * 2.0f * i / segments;
        float x = center.x + radius * cos(theta);
        float y = center.y + radius * sin(theta);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }

    for (int i = 1; i <= segments; i++) {
        indices.push_back(0);
		indices.push_back(i);
        indices.push_back(i + 1);
	}
    Mesh* circle = new Mesh(name);
    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateRoundedTriangle(
    const std::string& name,
    glm::vec3 corner1,
    glm::vec3 corner2,
    glm::vec3 corner3,
    float cornerRadius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 v1 = corner2 - corner1;
    glm::vec3 v2 = corner3 - corner1;
    float crossProd = v1.x * v2.y - v1.y * v2.x;

    // Fix the order of the corners
    if (crossProd < 0.0f) {
        std::swap(corner2, corner3);
    }

    float minEdge = std::min({glm::length(corner1 - corner2), glm::length(corner2 - corner3), glm::length(corner3 - corner1)});
    float safeR = std::min(cornerRadius, minEdge * 0.45f);

    auto GenerateCornerArc = [&](glm::vec3 p_curr, glm::vec3 p_prev, glm::vec3 p_next)
    {
        glm::vec3 prevDir = glm::normalize(p_prev - p_curr);
        glm::vec3 nextDir = glm::normalize(p_next - p_curr);

        float angle = glm::angle(prevDir, nextDir);
        float halfAngle = 0.5f * angle;

        if (halfAngle < 0.01f) {
            halfAngle = 0.01f;
        }

        float centerDist = safeR / sin(halfAngle);
        float tangentDist = safeR / tan(halfAngle);

        glm::vec3 dirToCenter = glm::normalize(prevDir + nextDir);
        glm::vec3 center = p_curr + dirToCenter * centerDist;

        // Tangent points
        glm::vec3 prevTangent = p_curr + prevDir * tangentDist;
        glm::vec3 nextTangent = p_curr + nextDir * tangentDist;

        int segments = 8;
        for (int i = 0; i <= segments; i++)
        {
            float t = (float)i / (float)segments;
            glm::vec3 prevVec = prevTangent - center;
            glm::vec3 nextVec = nextTangent - center;

            glm::vec3 interpolatedDir = glm::normalize(glm::mix(prevVec, nextVec, t));
            glm::vec3 arcPoint = center + interpolatedDir * safeR;

            vertices.push_back(VertexFormat(arcPoint, color));
        }
    };

    GenerateCornerArc(corner1, corner3, corner2);
    GenerateCornerArc(corner2, corner1, corner3);
    GenerateCornerArc(corner3, corner2, corner1);

    Mesh* roundedTriangle = new Mesh(name);

    if (fill) {
        glm::vec3 centroid = (corner1 + corner2 + corner3) / 3.0f;
        vertices.insert(vertices.begin(), VertexFormat(centroid, color));

        for (unsigned int i = 1; i < vertices.size(); i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.back() = 1;

        roundedTriangle->SetDrawMode(GL_TRIANGLES);
    }
    else {
        for (unsigned int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }
        roundedTriangle->SetDrawMode(GL_LINE_LOOP);
    }

    //for (auto& v : vertices) v.color = color;

    roundedTriangle->InitFromData(vertices, indices);
    return roundedTriangle;
}

Mesh* object2D::CreateRoundedRectangle(
    const std::string& name,
    glm::vec3 bottomLeft,
    float width,
    float height,
    float cornerRadius,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float safeR = std::min(cornerRadius, std::min(width, height) / 2.0f);

    float z = bottomLeft.z;
    glm::vec3 centers[4] = {
        bottomLeft + glm::vec3(width - safeR, safeR, 0),  // Bottom-Right
        bottomLeft + glm::vec3(width - safeR, height - safeR, 0), // Top-Right
        bottomLeft + glm::vec3(safeR, height - safeR, 0), // Top-Left
        bottomLeft + glm::vec3(safeR, safeR, 0)           // Bottom-Left
    };

    float startAngles[4] = {3.0f * glm::pi<float>() / 2.0f, 0.0f, glm::pi<float>() / 2.0f, glm::pi<float>()};

    int segments = 10;

    // Generate the boundary vertices
    for (int c = 0; c < 4; c++)
    {
        for (int i = 0; i <= segments; i++)
        {
            float currentAngle = startAngles[c] + (glm::pi<float>() / 2.0f) * ((float)i / segments);

            glm::vec3 point = centers[c] + glm::vec3(cos(currentAngle) * safeR, sin(currentAngle) * safeR, 0);
            vertices.push_back(VertexFormat(point, color));
        }
    }

    Mesh* roundedRect = new Mesh(name);

    if (fill) {
        glm::vec3 center = bottomLeft + glm::vec3(width / 2.0f, height / 2.0f, 0);
        vertices.insert(vertices.begin(), VertexFormat(center, color));

        for (unsigned int i = 1; i < vertices.size(); i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.back() = 1;
        roundedRect->SetDrawMode(GL_TRIANGLES);
    }
    else {
        for (unsigned int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }
        roundedRect->SetDrawMode(GL_LINE_LOOP);
    }

    //for (auto& v : vertices) v.color = color;

    roundedRect->InitFromData(vertices, indices);
    return roundedRect;
}

