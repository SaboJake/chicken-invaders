#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	// Create triangle with given 3 corners and color
	Mesh* CreateTriangle(const std::string& name, glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 color, bool fill = false);

	Mesh* CreateSemicircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false, int segments = 100);

	Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill = false, int segments = 100);

    Mesh* CreateRoundedTriangle(
        const std::string& name,
        glm::vec3 corner1,
        glm::vec3 corner2,
        glm::vec3 corner3,
        float cornerRadius,
        glm::vec3 color,
        bool fill);

    Mesh* CreateRoundedRectangle(
        const std::string& name,
        glm::vec3 bottomLeft,
        float width,
        float height,
        float cornerRadius,
        glm::vec3 color,
        bool fill);
}


