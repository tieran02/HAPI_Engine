#pragma once
#include <cstdint>
#include <memory>


struct BaseComponent {
	virtual ~BaseComponent() = default;
	static const uint64_t NextID();
	virtual std::shared_ptr<BaseComponent> Make() const = 0;
	virtual std::shared_ptr<BaseComponent> Clone() const = 0;

	uint64_t id;
};

template<typename T>
struct Component : public BaseComponent {
	Component() { id = ID;}

	static const uint64_t ID;

	std::shared_ptr<BaseComponent> Make() const override
	{
		auto component = std::make_shared<T>();
		return component;
	}
};

//Give a component a unique ID by using a static number that increments each time you derive from base component
template<typename T>
const uint64_t Component<T>::ID(BaseComponent::NextID());
