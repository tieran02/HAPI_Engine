#include "Renderer.hpp"
#include <HAPI_lib.h>
#include <algorithm>

#include "Sprite.hpp"
#include "AnimatedSprite.hpp"
#include "Tilesheet.hpp"
#include "Time.h"

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

void Renderer::ClearScreen(const std::string& spriteName)
{
	//check if sprite is in the sprite map
	if(m_sprites.find(spriteName) != m_sprites.end())
	{
		m_sprites[spriteName]->Draw(m_screen, m_screenSize, Vector2i(0, 0));
	}
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

void Renderer::LoadTilesheet(const std::string& tilesheetName, const std::string& textureName, int rows, int columns)
{
	if (m_sprites.find(tilesheetName) == m_sprites.end() && m_textures.find(textureName) != m_textures.end()) {
		//create new sprite
		Tilesheet* tilesheet = new Tilesheet;
		tilesheet->Load(m_textures[textureName], rows, columns);
		m_sprites[tilesheetName] = tilesheet;
	}
	else if (m_sprites.find(tilesheetName) != m_sprites.end())
		std::cout << "Can't load tilesheet as it already exists with the name '" << tilesheetName << "'\n";
	else
		std::cout << "Can't load tilesheet as the texture with the name '" << textureName << "' doesn't exist!\n";
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
	spriteRect.Translate(pos + m_offset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen,m_screenSize,pos + m_offset);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-pos - m_offset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pos + m_offset, spriteRect);
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

void Renderer::DrawAnimation(const std::string& animationName, const Vector2i& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite->GetWidth(), 0, sprite->GetHeight());

	//Draw from center of the sprite
	Vector2i centerPos = { pos.x - sprite->GetWidth() / 2,pos.y - sprite->GetHeight() / 2 };
	spriteRect.Translate(centerPos + m_offset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, centerPos + m_offset,currentFrame,lastTime,speed);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - m_offset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, centerPos + m_offset, currentFrame, lastTime, speed, spriteRect);
	}
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed)
{
	Vector2i veci = { (int)pos.x,(int)pos.y };
	DrawAnimation(animationName, veci,currentFrame,lastTime,speed);
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector3f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed)
{
	DrawAnimation(animationName, projectPosition(pos), currentFrame, lastTime, speed);
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2i& pos, int currentFrame)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite->GetWidth(), 0, sprite->GetHeight());

	//Draw from center of the sprite
	Vector2i centerPos = { pos.x - sprite->GetWidth() / 2,pos.y - sprite->GetHeight() / 2 };
	spriteRect.Translate(centerPos + m_offset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen,m_screenSize, centerPos + m_offset, currentFrame);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - m_offset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, centerPos + m_offset, currentFrame, spriteRect);
	}
}

void Renderer::GetAnimationFrameData(const std::string& animationName, int& StartFrame, int& EndFrame)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't get animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	StartFrame = sprite->GetStartFrame();
	EndFrame = sprite->GetEndFrame();
}

void Renderer::DrawTile(const std::string& tilesheetName, const Vector2i& pos, int tileIndex)
{
	Tilesheet* sprite = (Tilesheet*)m_sprites[tilesheetName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << tilesheetName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect(0, sprite->GetWidth(), 0, sprite->GetHeight());

	//Draw from center of the sprite
	Vector2i centerPos = { pos.x - sprite->GetWidth() / 2,pos.y - sprite->GetHeight() / 2 };
	spriteRect.Translate(centerPos + m_offset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, centerPos + m_offset, tileIndex);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - m_offset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, centerPos + m_offset, tileIndex, spriteRect);
	}
}

void Renderer::DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex)
{
	Vector2i veci = { (int)pos.x,(int)pos.y };
	DrawTile(tilesheetName, veci, tileIndex);
}

void Renderer::DrawTile(const std::string& tilesheetName, const Vector3f& pos, int tileIndex)
{
	DrawTile(tilesheetName, projectPosition(pos), tileIndex);
}

void Renderer::InstanceDraw(int id, const std::string& spriteName, Vector2i& pos, float rotation)
{
	if(m_sprites.find(spriteName) == m_sprites.end())
	{
		std::cout << "Can not queue draw sprite '" << spriteName << "' as it doesn't the sprite doesn't exist\n";
		return;
	}

	//check if sprite exists
	if (m_instancedSprites.find(id) == m_instancedSprites.end())
	{
		//Insert new instance sprite to be drawn
		m_instancedSprites.emplace(id, InstancedSprite(InstancedSprite::Sprite, spriteName, pos, rotation));
	}
	else
	{
		//update existing sprite position
		m_instancedSprites.at(id).position = pos;
	}
}

void Renderer::InstanceDrawAnimation(int id, const std::string& spriteName, Vector2i& pos, int frame, int& startFrame, int& endFrame, float rotation)
{
	if (m_sprites.find(spriteName) == m_sprites.end())
	{
		std::cout << "Can not queue draw sprite '" << spriteName << "' as it doesn't the sprite doesn't exist\n";
		return;
	}
	//auto animatedSprite = std::forward_as_tuple(m_sprites.at(spriteName), spriteName, position, frame, lastTime, speed);

	endFrame = ((AnimatedSprite*)m_sprites.at(spriteName))->GetEndFrame();
	startFrame = ((AnimatedSprite*)m_sprites.at(spriteName))->GetStartFrame();

	//check if animiated exists
	if (m_instancedSprites.find(id) == m_instancedSprites.end())
	{
		//Insert new instance sprite to be drawn
		m_instancedSprites.emplace(id, InstancedSprite(InstancedSprite::Animated, spriteName, pos,rotation, frame));
	}
	else
	{
		//update existing sprite position
		m_instancedSprites.at(id).position = pos;
		m_instancedSprites.at(id).frame = frame;
	}
}

void Renderer::RemoveInstance(int id)
{
	if (m_instancedSprites.find(id) != m_instancedSprites.end())
		m_instancedSprites.erase(id);
}

void Renderer::DrawInstancedSprites()
{


	//Loop through all instance sprite and draw them
	for (auto& queued_sprite : m_instancedSprites)
	{
		if (queued_sprite.second.spriteType == InstancedSprite::Sprite)
		{
			Draw(*queued_sprite.second.spriteName, queued_sprite.second.position);
		}
		else if (queued_sprite.second.spriteType == InstancedSprite::Animated)
		{
			//Pass the current frame, lastTime, speed as pointer
			int& frame = queued_sprite.second.frame;
			DrawAnimation(*queued_sprite.second.spriteName, queued_sprite.second.position,queued_sprite.second.frame);
		}
	}
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
	m_instancedSprites.clear();

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
