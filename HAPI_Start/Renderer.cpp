#include "Renderer.hpp"
#include <HAPI_lib.h>
#include <algorithm>

#include "Sprite.hpp"
#include "AnimatedSprite.hpp"

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

void Renderer::LoadSprite(const std::string& spriteName, const std::string& textureName)
{
	if (m_sprites.find(spriteName) == m_sprites.end() && m_textures.find(textureName) != m_textures.end()) {
		Sprite* sprite = new Sprite;
		sprite->Load(m_textures[textureName]);
		m_sprites[spriteName] = sprite;
	}
	else if (m_sprites.find(spriteName) != m_sprites.end())
		std::cout << "Can't load sprite as it already exists with the name '" << spriteName << "'\n";
	else
		std::cout << "Can't load sprite as the texture with the name '" << textureName << "' doesn't exist!\n";
}

void Renderer::LoadAnimatedSprite(const std::string& animationName, const std::string& textureName, int rows, int columns, int startFrame, int endFrame)
{
	if (m_sprites.find(animationName) == m_sprites.end() && m_textures.find(textureName) != m_textures.end()) {
		//create new sprite
		AnimatedSprite* animated_sprite = new AnimatedSprite;
		animated_sprite->Load(m_textures[textureName], rows, columns, startFrame, endFrame);
		m_sprites[animationName] = animated_sprite;
	}
	else if (m_sprites.find(animationName) != m_sprites.end())
		std::cout << "Can't load animated sprite as it already exists with the name '" << animationName << "'\n";
	else
		std::cout << "Can't load animated sprite as the texture with the name '" << textureName << "' doesn't exist!\n";
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

void Renderer::Draw(const std::string& spriteName, const Vector2i& pos)
{
	Sprite* sprite = m_sprites[spriteName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw sprite '" << spriteName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite->GetWidth(), 0, sprite->GetHeight());
	spriteRect.Translate(pos);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen,m_screenSize,pos);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-pos);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pos, spriteRect);
	}
}

void Renderer::Draw(const std::string& spriteName, const Vector2f& pos)
{
	Vector2i veci = { (int)pos.x,(int)pos.y };
	Draw(spriteName, veci);
}

void Renderer::Draw(const std::string& spriteName, const Vector3f& pos)
{
	Draw(spriteName, projectPosition(pos));
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2i& pos, int& currentFrame, float speed)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite->GetWidth(), 0, sprite->GetHeight());
	spriteRect.Translate(pos);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, pos,currentFrame,speed);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-pos);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pos,currentFrame,speed, spriteRect);
	}
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, float speed)
{
	Vector2i veci = { (int)pos.x,(int)pos.y };
	DrawAnimation(animationName, veci,currentFrame,speed);
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector3f& pos, int& currentFrame, float speed)
{
	DrawAnimation(animationName, projectPosition(pos), currentFrame, speed);
}

void Renderer::LoadTexture(std::string name, const std::string& path)
{
	if(m_textures.find(name) == m_textures.end())
	{
		//add new sprite
		Texture* texture = new Texture();
		texture->Load(path);
		m_textures[name] = texture;
	}

}

void Renderer::Cleanup()
{
	//delete Textures
	for (auto& texture : m_textures)
	{
		delete texture.second;
	}
	m_textures.clear();
	//delete sprites
	for (auto& sprite : m_sprites)
	{
		delete sprite.second;
	}
	m_sprites.clear();

}
