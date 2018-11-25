#pragma once
#include <memory>
#include <unordered_map>
#include "Component.hpp"

class ComponentFactory
{
public:
	ComponentFactory();
	~ComponentFactory();

	void Clear();

	template <class T>
	void AddComponent(const std::string& name);

	std::vector<std::shared_ptr<BaseComponent>> MakeComponents(std::vector<std::string>  componentNames) const;
private:
	std::unordered_map<std::string, std::unique_ptr<BaseComponent>> m_components;
};

template <class T>
void ComponentFactory::AddComponent(const std::string& name)
{
	if(m_components.find(name) != m_components.end())
		return;

	m_components[name] = std::make_unique<T>();
}

