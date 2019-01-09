#include "Component.hpp"

static uint64_t ComponentID = 0;

uint64_t BaseComponent::NextID() {

	return (uint64_t)1 << ComponentID++;

}