#include "lab_m1/tema1/game/projectiles/neutron_gun.h"

glm::vec2 NeutronGun::getCenter() {
	return glm::vec2(x + 0.5 * 64, y + 0.75 * 64);
}