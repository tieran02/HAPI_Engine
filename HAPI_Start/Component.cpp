#include "Component.hpp"

static uint32_t ComponentID = 0;

const uint64_t BaseComponent::NextID() {

	return 1 << ComponentID++;

}