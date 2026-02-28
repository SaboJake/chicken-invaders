#include "dragable.h"

void Dragable::Drag(int mouseX, int mouseY) {
    x = static_cast<float>(mouseX);
    y = static_cast<float>(mouseY);
    isDragging = true;
}