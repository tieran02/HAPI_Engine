#pragma once
#include <HAPI_lib.h>

class Sprite
{
public:
	Sprite();
	~Sprite();

	//load texture from file
	void Load(const std::string& path);

	const int& GetWidth() const { return m_width; }
	const int& GetHeight() const { return m_height; }

	const HAPISPACE::BYTE* GetTexture() const { return m_texture; }
	bool HasAlpha() const { return m_hasAlpha; }

private:
	int m_width{ 0 }, m_height{ 0 };
	HAPISPACE::BYTE* m_texture{ nullptr };
	bool m_hasAlpha;

	bool loadTexture(const std::string& path);
	//Go through each pixel of the texture and check if it has an alpha channel
	bool checkAlpha();
};

