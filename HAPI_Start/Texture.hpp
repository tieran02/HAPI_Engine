#pragma once
#include <HAPI_lib.h>
#include "Vector2.hpp"
#include "Vector3.hpp"
using namespace HAPISPACE;

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	//Standard blit, faster (DOESN'T PROJECT PIXELS)
	void Blit(BYTE* screen, const Vector2i& screenSize, const Vector2i& pos);
	//Blit uses Vector3f and projects each pixel
	void Blit(BYTE* screen, const Vector2i& screenSize, const Vector3f& pos, const float& eyeDistance);

private:
	int m_width{ 0 }, m_height{ 0 };
	BYTE* m_texture{ nullptr };

	bool loadTexture(const std::string& path);
};

