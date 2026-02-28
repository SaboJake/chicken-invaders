#include "lab_m1/tema1/editor/editor.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/my_utils/transform2D.h"
#include "lab_m1/tema1/my_utils/transform3D.h"

#include "lab_m1/tema1/my_utils/object2D.h"
#include "lab_m1/tema1/editor/objects/cannon.h"
#include "lab_m1/tema1/editor/objects/block.h"
#include "lab_m1/tema1/editor/objects/booster.h"
#include "lab_m1/tema1/editor/objects/bumper.h"
#include "lab_m1/tema1/game/game.h"
#include "components/text_renderer.h"


#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2


using namespace std;
using namespace m1;

bool clickCheck(int mouseX, int mouseY, float objX, float objY, float objWidth, float objHeight) {
    if (mouseX >= objX && mouseX <= objX + objWidth &&
        mouseY >= objY && mouseY <= objY + objHeight) {
        return true;
    }
    return false;
}

glm::vec2 getMouseLogicPos(int mouseX, int mouseY,
    Editor::ViewportSpace viewSpace, Editor::LogicSpace logicSpace)
{
    float scale = min((float)viewSpace.width / logicSpace.width,
                      (float)viewSpace.height / logicSpace.height);

    float marginX = (viewSpace.width - logicSpace.width * scale) / 2;
    float marginY = (viewSpace.height - logicSpace.height * scale) / 2;

	glm::vec2 logicPos;
    logicPos.x = logicSpace.x + (mouseX - viewSpace.x - marginX) / scale;
    logicPos.y = logicSpace.y + (mouseY - viewSpace.y - marginY) / scale;

	return logicPos;
}
/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
Editor::Editor()
{
	cout << "Editor constructor" << endl;
}
Editor::~Editor()
{
}

void Editor::Init()
{
	cout << "Editor Init" << endl;

    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1920;   // logic width
    logicSpace.height = 1080;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    L = 64;
    gridL = 0.9 * L;

    Mesh* square1 = object2D::CreateSquare("square1", corner, gridL, glm::vec3(0.0f, 0.0f, 0.25f), true);
	Mesh* square2 = object2D::CreateSquare("square2", corner, L, glm::vec3(0, 1, 0), true);
    Mesh* square3 = object2D::CreateSquare("square3", corner, L, glm::vec3(0, 0, 1), true);
	Mesh* square5 = object2D::CreateSquare("square5", corner, L, glm::vec3(1, 0, 0), true);

	Mesh* blockSquare = object2D::CreateSquare("blockSquare", corner, L, glm::vec3(0.55f, 0.55f, 0.55f), true);

	Mesh* countSquare = object2D::CreateSquare("countSquare", corner, L, glm::vec3(0, 1, 0), true);

    Mesh* engineBase = object2D::CreateSquare("engineBase", corner, L, glm::vec3(1, 0.647f, 0), true);
    Mesh* eqiTriangleDown = object2D::CreateTriangle(
        "eqiTriangleDown",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(L, 0.0f, 0.0f),
        glm::vec3(0.5 * L, L * sqrt(3) / 2.0f, 0.0f),
        glm::vec3(1, 0.5f, 0),
		true);
    Mesh* rightTriangleDownLeft = object2D::CreateTriangle(
        "rightTriangleDownLeft",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(L, 0.0f, 0.0f),
        glm::vec3(0.0f, L, 0.0f),
		glm::vec3(1, 0.5f, 0),
		true);
    Mesh* rightTriangleDownRight = object2D::CreateTriangle(
        "rightTriangleDownRight",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(L, 0.0f, 0.0f),
		glm::vec3(L, L, 0.0f),
		glm::vec3(1, 0.5f, 0),
		true);

	Mesh* cannonBaseRect = object2D::CreateSquare("cannonBaseRect", corner, L, glm::vec3(0.5f, 0.5f, 0.5f), true);
	Mesh* cannonBaseSemicircle = object2D::CreateSemicircle("cannonBaseSemicircle", glm::vec3(0.5f * L, L, 0.0f), 0.5f * L, glm::vec3(0.6f, 0.6f, 0.6f), true, 50);
	Mesh* cannonBarrelRect = object2D::CreateSquare("cannonBarrelRect", corner, L, glm::vec3(0.3f, 0.3f, 0.3f), true);
    Mesh* cannonBarrelTriangle = object2D::CreateTriangle(
        "cannonBarrelTriangle",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(L, 0.0f, 0.0f),
        glm::vec3(0.5 * L, L * sqrt(3) / 2.0f, 0.0f),
		glm::vec3(0.35f, 0.35f, 0.35f),
		true);

	Mesh* bumperBase = object2D::CreateSquare("bumperBase", corner, L, glm::vec3(0.5f, 0.5f, 0.5f), true);
	Mesh* bumperTop = object2D::CreateSemicircle("bumperTop", glm::vec3(0.5f * L, L, 0.0f), 0.5f * L, glm::vec3(0.815f, 0.815f, 0.85f), true, 50);

	Mesh* boundingBox1 = object2D::CreateSquare("boundingBox1", corner, L, glm::vec3(1, 0, 0), false);
	Mesh* boundingBox2 = object2D::CreateSquare("boundingBox2", corner, gridL, glm::vec3(0, 0, 0.9), false);

    AddMeshToList(square1);
	AddMeshToList(square2);
    AddMeshToList(square3);
	AddMeshToList(square5);

	AddMeshToList(countSquare);

	AddMeshToList(blockSquare);

	AddMeshToList(engineBase);
	AddMeshToList(eqiTriangleDown);
    AddMeshToList(rightTriangleDownLeft);
	AddMeshToList(rightTriangleDownRight);

	AddMeshToList(cannonBaseRect);
	AddMeshToList(cannonBaseSemicircle);
	AddMeshToList(cannonBarrelRect);
	AddMeshToList(cannonBarrelTriangle);

    AddMeshToList(bumperBase);
	AddMeshToList(bumperTop);

	AddMeshToList(boundingBox1);
	AddMeshToList(boundingBox2);

	// Create a grid of slots
	grid = new Grid(gridL, 10, 20, L, L, square1);
	gridOffsetX = 8 * L;
	gridOffsetY = 4 * L;
    cout << "Initialized grid" << endl;

    draggedObject = NULL;

    armory.push_back(new Block({blockSquare},
        {glm::mat4(1.0f)},
        transform3D::Translate(-0.5 * L, -0.5 * L, 0),
        2 * L, 1.5 * L, 1.0f));

	armory.push_back(new Cannon({cannonBaseRect, cannonBaseSemicircle, cannonBarrelRect, cannonBarrelTriangle},
        {
            transform3D::Scale(1.0f, 0.5f, 1.0f) * transform3D::Translate(0.0f, L, 0.0f),
			transform3D::Translate(0.0f, - 0.5 * L, 0.0f),
            transform3D::Scale(0.75f, 2.75f, 1.0f) * transform3D::Translate(0.1667f * L, -(2.0f / 3.0f) * L, 0.0f),
            transform3D::Translate(0.0f, -2.0f * L, 0.0f)
        },
        transform3D::Translate(-0.5 * L, -0.5 * L, 0.0f),
        2 * L, 10.5 * L, 1.0f));

    armory.push_back(new Booster({engineBase, eqiTriangleDown, rightTriangleDownLeft, rightTriangleDownRight},
        {
            glm::mat4(1.0f),
            transform3D::Translate(0.0f, L, 0.0f),
            transform3D::Translate(0.0f, L, 0.0f),
            transform3D::Translate(0.0f, L, 0.0f)
        },
        transform3D::Translate(-0.5 * L, - 0.5 * L, 0.0f),
        2 * L, 13 * L, 1.0f));

    armory.push_back(new Bumper({bumperBase, bumperTop},
        {
            glm::mat4(1),
            transform3D::Translate(-L, -2.0f * L, 0.0f) * transform3D::Scale(3.0f, 2.0f, 1.0)
        },
		transform3D::Translate(-0.5 * L, -0.5 * L, 0.0f),
		2 * L, 6 * L, 1.0f));

	maxParts = 20;
	partsUsed = 0;
	counterOffsetX = 8 * L;
	counterOffsetY = L;
	counterScaleSpaceX = L;
	counterScaleSpaceY = 0.5f * L;

	validShip = false;
	indicatorOffsetX = 1920 - 2 * L;
	indicatorOffsetY = 0.75 * L;

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, logicSpace.width, logicSpace.height);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 100);

	// Enable depth test
    glEnable(GL_DEPTH_TEST);
}


glm::mat3 Editor::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    smin = (sx < sy) ? sx : sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y + viewSpace.height - smin * logicSpace.y - (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, -smin, ty, // Invert the Y-axis scaling
        0.0f, 0.0f, 1.0f));
}


void Editor::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Editor::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Editor::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    visMatrix4D = glm::mat4(
        visMatrix[0][0], visMatrix[0][1], 0.0f, 0.0f,
        visMatrix[1][0], visMatrix[1][1], 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        visMatrix[2][0], visMatrix[2][1], 0.0f, 1.0f
    );
    DrawScene(visMatrix);
}


void Editor::FrameEnd()
{
}


void Editor::DrawScene(glm::mat3 visMatrix)
{
	// Render the grid of slots
	glm::mat4 gridOffset = transform3D::Translate(gridOffsetX, gridOffsetY, 0.0f);
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            Slot& slot = grid->slots[i][j];
            //modelMatrix4D = visMatrix4D * transform3D::Translate(slot.x, slot.y, slot.z);
			modelMatrix4D = visMatrix4D * gridOffset * transform3D::Translate(slot.x, slot.y, slot.z);
			modelMatrix4D = modelMatrix4D * transform3D::Translate((L - grid->l) / 2, (L - grid->l) / 2, 0.0f);
            RenderMesh(slot.square, shaders["VertexColor"], modelMatrix4D);
        }
	}

	// Render the dragged object
    if (draggedObject) {
        glm::mat4 dragOffset = transform3D::Translate(draggedObject->x, draggedObject->y, draggedObject->z);
        modelMatrix4D = visMatrix4D * dragOffset;
        if (draggedObject->isDragging) {
            modelMatrix4D = modelMatrix4D * draggedObject->dragOffset;
        }
        for (int k = 0; k < draggedObject->meshes.size(); k++) {
            RenderMesh(draggedObject->meshes[k], shaders["VertexColor"], modelMatrix4D * draggedObject->offsets[k]);
		}
    }

    for (Placed *object: armory) {
        modelMatrix4D = visMatrix4D * transform3D::Translate(object->x, object->y, object->z);
        for (int k = 0; k < object->meshes.size(); k++) {
            RenderMesh(object->meshes[k], shaders["VertexColor"], modelMatrix4D * object->offsets[k]);
        }
    }

    for (Placed *object : placedObjects) {
        modelMatrix4D = visMatrix4D * transform3D::Translate(object->x, object->y, object->z);
        for (int k = 0; k < object->meshes.size(); k++) {
            RenderMesh(object->meshes[k], shaders["VertexColor"], modelMatrix4D * object->offsets[k]);
		}
	}

	// Render parts counter
    modelMatrix4D = visMatrix4D * transform3D::Translate(counterOffsetX, counterOffsetY, 0.0f);
    for (int i = 0; i < maxParts - partsUsed; i++) {
        glm::mat4 offset = transform3D::Translate(i * counterScaleSpaceX, 0.0f, 0.0f) *
                          transform3D::Scale(0.5f, 0.5f, 1.0f);
        RenderMesh(meshes["countSquare"], shaders["VertexColor"], modelMatrix4D * offset);
	}

	// Render valid ship indicator
    RenderMesh(validShip ? meshes["square2"] : meshes["square5"],
        shaders["VertexColor"],
		visMatrix4D * transform3D::Translate(indicatorOffsetX, indicatorOffsetY, 0.0f));

	// Render bounding boxes arround armory objects
	glm::mat4 boundingOffset = visMatrix4D * transform3D::Translate(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < armory.size(); i++) {
		float x = 4.0f;
        modelMatrix4D = boundingOffset * transform3D::Scale(5.0f, x, 0.0f);
        RenderMesh(meshes["boundingBox1"], shaders["VertexColor"], modelMatrix4D);
		boundingOffset = boundingOffset * transform3D::Translate(0.0f, x * L, 0.0f);
	}

	// Render bounding box arround grid
    modelMatrix4D = visMatrix4D * transform3D::Translate(gridOffsetX - L, gridOffsetY - L, 0.0f) *
                    transform3D::Scale((grid->cols + 2) * grid->space_x / gridL, (grid->rows + 2) * grid->space_y / gridL, 1.0f);
	RenderMesh(meshes["boundingBox2"], shaders["VertexColor"], modelMatrix4D);

    // Render drag and drop button
    if (dragAndDrop) {
        RenderMesh(meshes["square2"], shaders["VertexColor"],
			visMatrix4D * transform3D::Translate(logicSpace.width - 2 * L, logicSpace.height - 1.5 * L, 0.0f));
    } else {
		RenderMesh(meshes["square5"], shaders["VertexColor"],
			visMatrix4D * transform3D::Translate(logicSpace.width - 2 * L, logicSpace.height - 1.5 * L, 0.0f));
    }

	// Render drag and drop text
    string dragText = "Drag & Drop: ";
    dragText += (dragAndDrop ? "ON" : "OFF");
    textRenderer->RenderText(dragText,
        logicSpace.width - 10.0f * L,
        logicSpace.height - 1.25f * L,
        0.5f,
		glm::vec3(1, 1, 1));
 }


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Editor::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Move the logic window with W, A, S, D (up, left, down, right)

    // TODO(student): Zoom in and zoom out logic window with Z and X

}


void Editor::OnKeyPress(int key, int mods)
{
}


void Editor::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Editor::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (draggedObject && draggedObject->isDragging) {
        glm::vec2 logicPos = getMouseLogicPos(mouseX, mouseY, viewSpace, logicSpace);


        draggedObject->Drag(logicPos.x, logicPos.y);
    }
}


void Editor::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == MOUSE_BUTTON_LEFT) {
        glm::vec2 logicPos = getMouseLogicPos(mouseX, mouseY, viewSpace, logicSpace);
		// Check if drag and drop button is clicked
        if (clickCheck(logicPos.x, logicPos.y, logicSpace.width - 2 * L, logicSpace.height - 1.5 * L, L, L)) {
            dragAndDrop = !dragAndDrop;
            return;
		}

		// Check if indicator is clicked
		if (validShip
            && clickCheck(logicPos.x, logicPos.y, indicatorOffsetX, indicatorOffsetY, L, L)) {
            cout << "Indicator clicked!" << endl;
            // Change scene
			Game* gameScene = new m1::Game();
            for (Placed* obj : placedObjects) {
                gameScene->placedObjects.push_back(obj->Clone());
            }
            gameScene->SetActive(false);
            manager->SwitchScene(gameScene);
			return;
        }
        
        // Check if any armory object is clicked
        for (Placed *object : armory) {
            if (object->clickCheck(logicPos.x, logicPos.y, L)) {
				cout << "Armory object selected!" << endl;
				draggedObject = object->Clone();
				draggedObject->isDragging = true;
				draggedObject->Drag(logicPos.x, logicPos.y);
                selectedObject = true;
                return;
            }
		}

        // Check if the dragged object is clicked
        if (draggedObject && !selectedObject) {
            if (clickCheck(logicPos.x, logicPos.y, draggedObject->x, draggedObject->y, L, L)) {
                cout << "Dragged object selected!" << endl;
                draggedObject->isDragging = true;
				draggedObject->x = logicPos.x;
				draggedObject->y = logicPos.y;
                selectedObject = true;
                return;
            }
        }


        // Adjust for grid offset
        logicPos.x -= gridOffsetX;
        logicPos.y -= gridOffsetY;


        // Determine the slot indices
        int col = floor(logicPos.x / grid->space_x);
        int row = floor(logicPos.y / grid->space_y);

        // Check if the click is within the grid bounds
        if (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols) {
            Slot& clickedSlot = grid->slots[row][col];
            cout << "Clicked slot at row " << row << ", column " << col << endl;
			// Place the dragged object into the clicked slot
            if (draggedObject && draggedObject->PlaceCheck(grid, row, col)\
                && selectedObject && !clickedSlot.isFilled && partsUsed <= maxParts) {
				// Create a new Placed object in the grid
                Placed* placedObject = draggedObject->Clone();
                placedObject->x = clickedSlot.x + gridOffsetX;
                placedObject->y = clickedSlot.y + gridOffsetY;

				placedObjects.push_back(placedObject);
				clickedSlot.object = placedObject;
				clickedSlot.isFilled = true;
				clickedSlot.connected = true;

				placedObject->SetSlots(grid, row, col, true);

				delete draggedObject;
				draggedObject = NULL;
				selectedObject = false;

				partsUsed++;

				validShip = grid->checkConnected();
            }
            // Select the object in the clicked slot
            else if (!selectedObject && clickedSlot.isFilled) {
				glm::vec2 objPos = grid->SearchObject(clickedSlot.type, row, col);
                cout << "objPos: " << objPos.x << " " << objPos.y << endl;
				Slot &objectSlot = grid->slots[(int)objPos.x][(int)objPos.y];
                row = (int)objPos.x; col = (int)objPos.y;
				// Start dragging the object from the slot
                objectSlot.object->SetSlots(grid, row, col, false);
				draggedObject = objectSlot.object->Clone();
				draggedObject->x = logicPos.x + gridOffsetX;
				draggedObject->y = logicPos.y + gridOffsetY;

				draggedObject->isDragging = true;
				selectedObject = true;
				// Remove the object from the slot
				placedObjects.erase(std::remove(placedObjects.begin(), placedObjects.end(), objectSlot.object), placedObjects.end());
                objectSlot.object = NULL;
                objectSlot.isFilled = false;
				objectSlot.connected = false;

				partsUsed--;

				validShip = grid->checkConnected();
				cout << "validShip: " << validShip << endl;
            }
        }
        else {
            cout << "Click is outside the grid!" << endl;
			// If clicked outside the grid, release the dragged object
            if (draggedObject && selectedObject) {
                delete draggedObject;
                draggedObject = NULL;
                selectedObject = false;
            }
        }

		logicPos.x += gridOffsetX;
		logicPos.y += gridOffsetY;
    } else if (button == MOUSE_BUTTON_RIGHT) {
        glm::vec2 logicPos = getMouseLogicPos(mouseX, mouseY, viewSpace, logicSpace);
		// Adjust for grid offset
		logicPos.x -= gridOffsetX;
		logicPos.y -= gridOffsetY;
		// Determine the slot indices
		int col = floor(logicPos.x / grid->space_x);
		int row = floor(logicPos.y / grid->space_y);

        if (draggedObject && selectedObject) {
			delete draggedObject;
			draggedObject = NULL;
			selectedObject = false;
        }

        if (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols) {
            Slot& clickedSlot = grid->slots[row][col];
            cout << "Right-clicked slot at row " << row << ", column " << col << endl;
            // Remove the object from the clicked slot
            if (clickedSlot.isFilled && !selectedObject) {
				glm::vec2 objPos = grid->SearchObject(clickedSlot.type, row, col);
                cout << "objPos: " << objPos.x << " " << objPos.y << endl;
				row = (int)objPos.x; col = (int)objPos.y;
				Slot& objectSlot = grid->slots[(int)objPos.x][(int)objPos.y];

				objectSlot.object->SetSlots(grid, row, col, false);
                placedObjects.erase(std::remove(placedObjects.begin(), placedObjects.end(), objectSlot.object), placedObjects.end());
                objectSlot.object = NULL;
                objectSlot.isFilled = false;
				objectSlot.connected = false;

				partsUsed--;

				validShip = grid->checkConnected();
            }
        }
        logicPos.x += gridOffsetX;
        logicPos.y += gridOffsetY;
	}
}


void Editor::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == MOUSE_BUTTON_LEFT) {
        if (!dragAndDrop) return;

        glm::vec2 logicPos = getMouseLogicPos(mouseX, mouseY, viewSpace, logicSpace);
        // Adjust for grid offset
        logicPos.x -= gridOffsetX;
        logicPos.y -= gridOffsetY;


        // Determine the slot indices
        int col = floor(logicPos.x / grid->space_x);
        int row = floor(logicPos.y / grid->space_y);

        // Check if the click is within the grid bounds
        if (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols) {
            Slot& clickedSlot = grid->slots[row][col];
            cout << "Clicked slot at row " << row << ", column " << col << endl;
            // Place the dragged object into the clicked slot
            if (draggedObject && draggedObject->PlaceCheck(grid, row, col)\
                && selectedObject && !clickedSlot.isFilled && partsUsed <= maxParts) {
                // Create a new Placed object in the grid
                Placed* placedObject = draggedObject->Clone();
                placedObject->x = clickedSlot.x + gridOffsetX;
                placedObject->y = clickedSlot.y + gridOffsetY;

                placedObjects.push_back(placedObject);
                clickedSlot.object = placedObject;
                clickedSlot.isFilled = true;
                clickedSlot.connected = true;

                placedObject->SetSlots(grid, row, col, true);

                delete draggedObject;
                draggedObject = NULL;
                selectedObject = false;

                partsUsed++;

                validShip = grid->checkConnected();
            }
            else {
                // If placement is invalid, release the dragged object
                if (draggedObject && selectedObject) {
                    delete draggedObject;
                    draggedObject = NULL;
                    selectedObject = false;
				}
            }
        } else {
            cout << "Click is outside the grid!" << endl;
            // If clicked outside the grid, release the dragged object
            if (draggedObject && selectedObject) {
                delete draggedObject;
                draggedObject = NULL;
                selectedObject = false;
            }
        }
	}
}


void Editor::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}
