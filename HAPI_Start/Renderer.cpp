#include "Renderer.hpp"
#include <HAPI_lib.h>
#include <algorithm>

using namespace HAPISPACE;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Intialise(const Vector2i& screenSize, const std::string& windowName)
{
	m_screenSize = screenSize;

	if (!HAPI.Initialise(m_screenSize.x, m_screenSize.y, windowName))
		return;

	m_screen = HAPI.GetScreenPointer();
}

void Renderer::ClearScreen(const HAPISPACE::HAPI_TColour& colour)
{
	// Get the screen pointer
	BYTE* screen = HAPI.GetScreenPointer();

	//Loop through all the pixels within the screen buffer
	for (size_t x = 0; x < static_cast<size_t>(m_screenSize.x) * static_cast<size_t>(m_screenSize.y) * 4; x += 4)
	{
		//calculate the offset pointer of the pixel
		BYTE* offset_ptr = screen + x;
		//Cast the pixel to a colour and set it to the new colour value
		*(HAPI_TColour*)offset_ptr = colour;
	}
}

void Renderer::ClearScreen(HAPISPACE::BYTE grayScale)
{
	//Get the screen pointer
	BYTE* screen = HAPI.GetScreenPointer();
	//set the full screen buffer with the grayscale value
	memset(screen, grayScale, m_screenSize.x * m_screenSize.y * 4);
}

void Renderer::SetPixel(const Vector2i& vec, const HAPISPACE::HAPI_TColour& colour)
{
	//Call the set pixel function and pass the vector values as separate values
	SetPixel(vec.x, vec.y, colour);
}

void Renderer::SetPixel(int x, int y, const HAPISPACE::HAPI_TColour& colour)
{
	//check if the position is within the screen buffer
	if (x >= 0 && x < m_screenSize.x && y >= 0 && y < m_screenSize.y) {
		//Get the screen pointer
		BYTE* screen = HAPI.GetScreenPointer();

		//calculate the offset
		const unsigned int offset = (x + y * m_screenSize.x) * 4;
		//Add the offset to the screen pixel
		BYTE* offset_ptr = screen + offset;
		//Cast the pixel to a colour and set it to the new colour value
		*(HAPI_TColour*)offset_ptr = colour;
	}
}

Vector2i Renderer::projectPosition(const Vector3f& sourcePos)
{
	//Check if the Z is less than or equal to 0, If true then return the source position without doing any calculation
	if (sourcePos.z <= 0)
		return Vector2i(static_cast<int>(sourcePos.x), static_cast<int>(sourcePos.y));

	//Get the center of the screen as a vector2 int
	const Vector2i center_screen = m_screenSize / 2;

	//Calculate the projected screen position from a 3D world position
	Vector2i projection;
	projection.x = static_cast<int>((m_eyeDistance * (sourcePos.x - center_screen.x)) / (sourcePos.z + m_eyeDistance)) + center_screen.x;
	projection.y = static_cast<int>((m_eyeDistance * (sourcePos.y - center_screen.y)) / (sourcePos.z + m_eyeDistance)) + center_screen.y;

	return projection;
}

void Renderer::Blit(const Sprite& texture, const Vector2i& pos)
{
	const auto& width = texture.GetWidth();
	const auto& height = texture.GetHeight();

	BYTE* screenPnter = m_screen + (pos.x + pos.y * m_screenSize.x) * 4;
	const BYTE* texturePnter = texture.GetTexture();

	for (size_t y = 0; y < height; y++)
	{
		memcpy(screenPnter, texturePnter, width * 4);
		//move texture pointer to next line
		texturePnter += width * 4;
		//move screen to pointer
		screenPnter += m_screenSize.x * 4;
	}
}

void Renderer::Blit(const Sprite& sprite, const Vector2i& pos, const Rect& area)
{
	const BYTE* currentTexturePixel = sprite.GetTexture();
	currentTexturePixel += (area.Left + (sprite.GetHeight() * area.Top)) * 4;
	const Vector2i center_screen = m_screenSize / 2;

	const int width = area.Right - area.Left;
	const int height = area.Bottom - area.Top;

	BYTE* currentScreenPixel = m_screen + (std::clamp(pos.x, 0, m_screenSize.x) + std::clamp(pos.y, 0, m_screenSize.y) * m_screenSize.x) * 4;

	for (size_t y = 0; y < height; y++)
	{
		memcpy(currentScreenPixel, currentTexturePixel, width * 4);
		//add the rest of the sprite width to get to the next line of the texture
		currentTexturePixel += sprite.GetWidth() * 4;
		//move screen to pointer
		currentScreenPixel += m_screenSize.x * 4;
	}
}

void Renderer::Blit3D(const Sprite& texture, const Vector3f& pos)
{
	const BYTE* currentTexturePixel = texture.GetTexture();
	const Vector2i center_screen = m_screenSize / 2;

	//for each pixel
	int currentPixelOnRow = 0;
	int currentHeight = 0;

	const int width = texture.GetWidth();
	const int height = texture.GetHeight();
	for (int i = 0; i < width*height * 4; i += 4)
	{
		//project
		Vector2i projection;
		projection.x = static_cast<int>(m_eyeDistance * (pos.x + currentPixelOnRow - center_screen.x) / (pos.z + m_eyeDistance) + center_screen.x);
		projection.y = static_cast<int>(m_eyeDistance * (pos.y + currentHeight - center_screen.y) / (pos.z + m_eyeDistance) + center_screen.y);

		BYTE* currentScreenPixel = m_screen + ((projection.x) + (projection.y) * m_screenSize.x) * 4;

		//Get screen and texture pointer
		if (projection.x >= 0 && projection.x + width < m_screenSize.x && projection.y >= 0 && projection.y + height < m_screenSize.y) {
			memcpy(currentScreenPixel, currentTexturePixel, 4);
		}

		currentPixelOnRow++;
		if (currentPixelOnRow >= width)
		{
			currentHeight++;
			currentPixelOnRow = 0;
		}
			currentTexturePixel += 4;
	}
}

void Renderer::BlitAlpha(const Sprite& sprite, const Vector2i& pos)
{
	const BYTE* currentTexturePixel = sprite.GetTexture();
	const Vector2i center_screen = m_screenSize / 2;

	const int width = sprite.GetWidth();
	const int height = sprite.GetHeight();

	BYTE* currentScreenPixel = m_screen + (pos.x+ pos.y * m_screenSize.x) * 4;
	int endOfLineScreenIncrement = (m_screenSize.x - width) * 4;

	for (int i = 0; i < (width*height * 4); i += 4)
	{
		if (i != 0 && i % (width * 4) == 0)
		{
			currentScreenPixel += endOfLineScreenIncrement;
		}

		//Get screen and texture pointer
		BYTE blue = currentTexturePixel[0];
		BYTE green = currentTexturePixel[1];
		BYTE red = currentTexturePixel[2];
		BYTE alpha = currentTexturePixel[3];

		if(alpha == (BYTE)0)
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

void Renderer::BlitAlpha(const Sprite& sprite, const Vector2i& pos, const Rect& area)
{
	const BYTE* currentTexturePixel = sprite.GetTexture();
	currentTexturePixel += (area.Left + (sprite.GetHeight() * area.Top)) * 4;
	const Vector2i center_screen = m_screenSize / 2;

	const int width = area.Right - area.Left;
	const int height = area.Bottom - area.Top;

	BYTE* currentScreenPixel = m_screen + (std::clamp(pos.x,0,m_screenSize.x) + std::clamp(pos.y, 0, m_screenSize.y) * m_screenSize.x) * 4;
	int endOfLineScreenIncrement = (m_screenSize.x - width) * 4;

	for (int i = 0; i < (width*height * 4); i += 4)
	{
		if (i != 0 && i % (width * 4) == 0)
		{
			currentScreenPixel += endOfLineScreenIncrement;
			//add the rest of the sprite width to get to the next line of the texture
			currentTexturePixel += (sprite.GetWidth() - width) * 4;
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

void Renderer::Draw(const Sprite& sprite, const Vector2i& pos)
{
	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(pos.x, pos.x + 64, pos.y, pos.y + 64);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		if (sprite.HasAlpha()) 
		{
			BlitAlpha(sprite, pos);
		}
		else
		{
			Blit(sprite, pos);
		}
	}
	else
	{
		spriteRect.ClipTo(screenRect);

		Rect sourceSpace = Rect(std::clamp(spriteRect.Left - pos.x, 0, screenRect.Right),
								std::clamp(spriteRect.Right - pos.x, 0, screenRect.Right),
								std::clamp(spriteRect.Top - pos.y, 0, screenRect.Bottom),
								std::clamp(spriteRect.Bottom - pos.y, 0, screenRect.Bottom));

		if (sprite.HasAlpha()) 
		{
			BlitAlpha(sprite, pos, sourceSpace);
		}
		else
		{
			Blit(sprite, pos, sourceSpace);
		}
	}
}

void Renderer::Draw(const Sprite& sprite, const Vector2f& pos)
{
	Vector2i veci = { (int)pos.x,(int)pos.y };
	Draw(sprite, veci);
}

void Renderer::Draw(const Sprite& sprite, const Vector3f& pos)
{
	Draw(sprite, projectPosition(pos));
}

Sprite* Renderer::LoadSprite(std::string name, const std::string& path)
{
	if(m_sprites.find(name) == m_sprites.end())
	{
		//add new sprite
		Sprite* sprite = new Sprite();
		sprite->Load(path);
		m_sprites[name] = sprite;

		return sprite;
	}
	return m_sprites[name];
}

void Renderer::Cleanup()
{
	for (auto& sprite : m_sprites)
	{
		delete sprite.second;
	}
	m_sprites.clear();
}
