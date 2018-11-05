#include "Texture.hpp"
#include <algorithm>

using namespace HAPISPACE;

Texture::~Texture()
{
	if(!m_isSubTexture)
		delete[] m_texture;
}

void Texture::Load(const std::string& path)
{
	if (!loadTexture(path)) {
		std::cerr << "TEXTURE::Failed to load texture from: " << path << std::endl;
		HAPI.UserMessage("Texture at path '" + path + "' couldn't open!", "ERROR");
		HAPI.Close();
	}
	//check whether sprite has an alpha channel
	m_hasAlpha = checkAlpha();
}

void Texture::Load(Texture* mainTexture, int offset)
{
	m_texture = mainTexture->m_texture + offset;
	m_width = mainTexture->m_width;
	m_height = mainTexture->m_height;;
	m_hasAlpha = mainTexture->m_hasAlpha;;
	m_isSubTexture = true;
}

void Texture::BlitFast(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const
{
	const BYTE* currentTexturePixel = m_texture;
	currentTexturePixel += (area.Left + (m_width * area.Top)) * 4;
	const Vector2i center_screen = screenSize / 2;

	const int width = area.Right - area.Left;
	const int height = area.Bottom - area.Top;

	BYTE* currentScreenPixel = screen + (std::clamp(pos.x, 0, screenSize.x) + std::clamp(pos.y, 0, screenSize.y) * screenSize.x) * 4;

	for (size_t y = 0; y < height; y++)
	{
		memcpy(currentScreenPixel, currentTexturePixel, width * 4);
		//add the rest of the sprite width to get to the next line of the texture
		currentTexturePixel += m_width * 4;
		//move screen to pointer
		currentScreenPixel += screenSize.x * 4;
	}
}

void Texture::BlitAlpha(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const
{
	const BYTE* currentTexturePixel = m_texture;
	auto offset = (area.Left + (m_width * area.Top));
	currentTexturePixel += offset * 4;
	const Vector2i center_screen = screenSize / 2;

	const int width = area.Right - area.Left;
	const int height = area.Bottom - area.Top;

	BYTE* currentScreenPixel = screen + (std::clamp(pos.x, 0, screenSize.x) + std::clamp(pos.y, 0, screenSize.y) * screenSize.x) * 4;
	int endOfLineScreenIncrement = (screenSize.x - width) * 4;

	for (int i = 0; i < (width*height * 4); i += 4)
	{
		if (i != 0 && i % (width * 4) == 0)
		{
			currentScreenPixel += endOfLineScreenIncrement;
			//add the rest of the sprite width to get to the next line of the texture
			currentTexturePixel += (m_width - width) * 4;
		}

		//Get screen and texture pointer
		BYTE blue = currentTexturePixel[0];
		BYTE green = currentTexturePixel[1];
		BYTE red = currentTexturePixel[2];
		BYTE alpha = currentTexturePixel[3];

		if (alpha == (BYTE)0)
		{
			//do nothing
		}
		else if (alpha == (BYTE)255)
		{
			//set screen pixel to texture
			currentScreenPixel[0] = currentTexturePixel[0];
			currentScreenPixel[1] = currentTexturePixel[1];
			currentScreenPixel[2] = currentTexturePixel[2];
		}
		else
		{
			currentScreenPixel[0] = currentScreenPixel[0] + ((alpha*(blue - currentScreenPixel[0])) >> 8);
			currentScreenPixel[1] = currentScreenPixel[1] + ((alpha*(green - currentScreenPixel[1])) >> 8);
			currentScreenPixel[2] = currentScreenPixel[2] + ((alpha*(red - currentScreenPixel[2])) >> 8);
		}

		currentScreenPixel += 4;
		currentTexturePixel += 4;
	}
}

bool Texture::loadTexture(const std::string& path)
{
	return HAPI.LoadTexture(path, &m_texture, m_width, m_height);
}

bool Texture::checkAlpha()
{
	//check each pixel for alpha
	if (m_texture == nullptr)
		return false;

	const BYTE* currentTexturePixel = m_texture;
	for (int i = 0; i < (m_width*m_height * 4); i += 4)
	{
		BYTE alpha = currentTexturePixel[3];

		if (alpha < 255)
			return true;

		currentTexturePixel += 4;
	}
	return false;
}
