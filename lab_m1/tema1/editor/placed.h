#pragma once

#include <vector>
#include "components/simple_scene.h"

// Forward declaration of Grid
class Grid;

class Placed {
public:
    std::vector<Mesh*> meshes;
	std::vector<glm::mat4> offsets;
	glm::mat4 dragOffset;
    float x;
    float y;
    float z;
    bool isDragging;

    int type;

    virtual ~Placed() = default;

    virtual Placed* Clone() const = 0;

    Placed(std::vector<Mesh*> meshes, float x, float y, float z)
        : meshes(meshes), x(x), y(y), z(z), isDragging(false), type(0) {
        for (size_t i = 0; i < meshes.size(); i++) {
            offsets.push_back(glm::mat4(1.0f));
		}
		dragOffset = glm::mat4(1.0f);
    }

    Placed(Mesh* mesh, float x, float y, float z)
        : x(x), y(y), z(z), isDragging(false), type(0) {
        meshes.push_back(mesh);
		offsets.push_back(glm::mat4(1.0f));
		dragOffset = glm::mat4(1.0f);
    }

    Placed(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, glm::mat4 dragOffset, float x, float y, float z)
        : meshes(meshes), offsets(offsets), dragOffset(dragOffset), x(x), y(y), z(z), isDragging(false), type(0) {
	}

    void Drag(int mouseX, int mouseY);

    virtual void SetSlots(Grid* grid, int i, int j, bool set);

    virtual bool PlaceCheck(Grid* grid, int i, int j);

    virtual glm::vec2 SearchGridPosition(Grid* grid, int i, int j);

    virtual bool clickCheck(float mouseX, float mouseY, float unit);
};