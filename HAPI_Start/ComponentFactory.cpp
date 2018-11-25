#include "ComponentFactory.hpp"
#include <iostream>


ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

void ComponentFactory::Clear()
{
	m_components.clear();
}

std::vector<std::shared_ptr<BaseComponent>> ComponentFactory::MakeComponents(std::vector<std::string> componentNames) const
{
	std::vector<std::shared_ptr<BaseComponent>> components;

	if (m_components.empty())
		return components;
	//search component map for the component names and create a shared pointer of that basecomponent
	components.resize(componentNames.size());
	for (int i = 0; i < componentNames.size(); ++i)
	{
		//check if name is withing the component map
		if(m_components.find(componentNames[i]) == m_components.end())
		{
			std::cout << "Can't find component with the name '" << componentNames[i].c_str() << "' in the collection of components (returning empty vector)\n";
			components.clear();
			return components;
		}

		//create new instace of that component
		components[i] = m_components.at(componentNames[i]).get()->Make();
	}
	return components;
}

