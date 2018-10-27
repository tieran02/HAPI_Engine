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

void Renderer::Draw(const Sprite& sprite, const Vector2i& pos)
{
	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite.GetWidth(), 0, sprite.GetHeight());
	spriteRect.Translate(pos);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		if (sprite.HasAlpha()) 
		{
			sprite.BlitAlpha(m_screen,m_screenSize,pos);
		}
		else
		{
			sprite.Blit(m_screen, m_screenSize, pos);
		}
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-pos);

		spriteRect.Clamp(screenRect);

		if (sprite.HasAlpha()) 
		{
			sprite.BlitAlpha(m_screen, m_screenSize, pos, spriteRect);
		}
		else
		{
			sprite.Blit(m_screen, m_screenSize, pos, spriteRect);
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

void Renderer::DrawAnimation(const Sprite & sprite, const Vector2i & pos, int numRows, int numCols, int startFrame, int endFrame, int& currentFrame, float speed)
{
	int frameWidth = sprite.GetWidth() / numRows;
	int frameHeight = sprite.GetHeight() / numCols;

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, frameWidth, 0, frameHeight);
	spriteRect.Translate(pos);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	
	int currentRow = int((startFrame + currentFrame) / numRows);
	int currentColumn = (startFrame + currentFrame) % numRows;

	//speed to milliseconds
	int milliSpeed = speed * 1000.0f;
	int currentTime = HAPI.GetTime();
	if (currentTime % milliSpeed == 0)
		currentFrame = (currentFrame + 1) % (endFrame - startFrame + 1);

	Rect rect(currentColumn*frameWidth, (currentColumn + 1) * frameWidth, currentRow*frameHeight, (currentRow + 1) * frameHeight);
	
	if (screenRect.Contains(spriteRect))
	{
		if (sprite.HasAlpha())
		{
			sprite.BlitAlpha(m_screen, m_screenSize, pos, rect);
		}
		else
		{
			sprite.Blit(m_screen, m_screenSize, pos,rect);
		}
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-pos);

		spriteRect.Translate(Vector2i(currentColumn * frameWidth, currentRow * frameHeight));
		spriteRect.Clamp(screenRect);

		if (sprite.HasAlpha())
		{
			sprite.BlitAlpha(m_screen, m_screenSize, pos, spriteRect);
		}
		else
		{
			sprite.Blit(m_screen, m_screenSize, pos, spriteRect);
		}
	}
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
