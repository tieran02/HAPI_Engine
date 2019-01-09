#include "Utilities.hpp"
#include "HAPI_lib.h"


Utilities::Utilities()
{
}


Utilities::~Utilities()
{
}

void Utilities::LoadSound(const std::string& path)
{
	if(!HAPI.LoadSound("Data\\"+path))
	{
		HAPI.UserMessage("Failed to load sound " + path, "FILE ERROR");
		HAPI.Close();
	}
}

void Utilities::PlaySound(const std::string& path, const HAPISPACE::HAPI_TSoundOptions& options)
{
	if (!HAPI.PlaySound("Data\\" + path,options))
	{
		HAPI.UserMessage("Failed to play sound " + path, "FILE ERROR");
		HAPI.Close();
	}
}
