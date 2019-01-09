#pragma once
#include <string>
#include <HAPI_InputCodes.h>
#include <HAPI_Types.h>

class Utilities
{
public:
	Utilities();
	~Utilities();

	static void LoadSound(const std::string& path);
	static void PlaySound(const std::string& path, const HAPISPACE::HAPI_TSoundOptions& options);
};

