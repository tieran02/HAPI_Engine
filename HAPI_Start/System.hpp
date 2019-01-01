#pragma once
#include <cstdint>
#include "Entity.hpp"

class ECSManager;

class System {
public:
	virtual ~System() = default;
	System(const uint64_t &signature) : m_signature(signature) {}

	/// <summary> Virtual update function to update any system </summary>
	/// <param name="entity"> refrence to the entity </param>
	/// <param name="components"> A vector of the components of the entity </param>
	virtual void Update(ECSManager& ecsManager, Entity & entity) = 0;

	/// <summary> Virtual update function to update any system </summary>
	/// <returns> A uint64 that is used a bitset to represent a signature of the system </returns>
	const std::bitset<64> &GetSignature() { return m_signature; }

private:
	const std::bitset<64> m_signature;
};