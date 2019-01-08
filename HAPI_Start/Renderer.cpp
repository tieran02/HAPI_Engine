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

void Renderer::SetOffset(const Vector2f& offset)
{
	m_lastOffset = m_offset;
	m_targetOffset = offset;
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

Vector2f Renderer::Lerp(const Vector2f& pos, const Vector2f& lastPos, float s)
{
	if(pos == lastPos)
		return pos;

	Vector2f interPos{ lastPos + (pos - lastPos) * s };

	return interPos;
}

void Renderer::Draw(const std::string& spriteName, const Vector2f& pos)
{
	Draw(spriteName, pos, GetSpriteRect(spriteName));
}

void Renderer::Draw(const std::string& spriteName, const Vector2f& pos, Rect area)
{
	Sprite* sprite = m_sprites[spriteName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw sprite '" << spriteName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Vector2f veciOffset = Vector2f{m_offset.x,m_offset.y };
	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect = area;
	Vector2f centerPos = { pos.x - (float)sprite->GetWidth() / 2, pos.y - (float)sprite->GetHeight() / 2 };
	Vector2i pixelPos = Vector2i((int)(centerPos + veciOffset).x, (int)(centerPos + veciOffset).y);

	spriteRect.Translate(centerPos + veciOffset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, pixelPos, area);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - veciOffset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pixelPos, spriteRect);
	}
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed)
{
	DrawAnimation(animationName, pos, currentFrame, lastTime, speed, GetSpriteRect(animationName));
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame,
	HAPISPACE::DWORD& lastTime, float speed, const Rect& area)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect = area;

	//Draw from center of the sprite
	Vector2f centerPos = { pos.x - sprite->GetWidth() / 2, pos.y - sprite->GetHeight() / 2 };
	Vector2f veciOffset = Vector2f{ m_offset.x,m_offset.y };
	Vector2i pixelPos = Vector2i((int)(centerPos + veciOffset).x, (int)(centerPos + veciOffset).y);

	spriteRect.Translate(centerPos + veciOffset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, pixelPos, currentFrame, lastTime, speed, area);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - veciOffset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pixelPos, currentFrame, lastTime, speed, spriteRect);
	}
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int currentFrame)
{
	DrawAnimation(animationName, pos, currentFrame, GetSpriteRect(animationName));
}

void Renderer::DrawAnimation(const std::string& animationName, const Vector2f& pos, int currentFrame, const Rect& area)
{
	AnimatedSprite* sprite = (AnimatedSprite*)m_sprites[animationName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << animationName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect = area;

	//Draw from center of the sprite
	Vector2f centerPos = {pos.x - sprite->GetWidth() / 2, pos.y - sprite->GetHeight() / 2 };
	Vector2f veciOffset = Vector2f{m_offset.x,m_offset.y };
	Vector2i pixelPos = Vector2i((int)(centerPos + veciOffset).x, (int)(centerPos + veciOffset).y);

	spriteRect.Translate(centerPos + veciOffset);


	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, pixelPos, currentFrame, area);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - veciOffset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pixelPos, currentFrame, spriteRect);
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

void Renderer::DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex)
{
	DrawTile(tilesheetName, pos, tileIndex, GetSpriteRect(tilesheetName));
}

void Renderer::DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex, const Rect& rect)
{
	Tilesheet* sprite = (Tilesheet*)m_sprites[tilesheetName];
	//check if sprite is null and if so return out of this function
	if (sprite == nullptr) {
		std::cout << "Can't draw animated sprite '" << tilesheetName << "' as it doesn't exist (Make sure you're loading the sprite before)\n";
		return;
	}

	Rect screenRect(0, m_screenSize.x, 0, m_screenSize.y);
	Rect spriteRect = rect;

	//Draw from center of the sprite
	Vector2f centerPos = { pos.x - sprite->GetWidth() / 2, pos.y - sprite->GetHeight() / 2 };
	Vector2f veciOffset = Vector2f{ m_offset.x,m_offset.y };
	Vector2i pixelPos = Vector2i((int)(centerPos + veciOffset).x, (int)(centerPos + veciOffset).y);


	spriteRect.Translate(centerPos + veciOffset);

	//Clipping
	if (spriteRect.Outside(screenRect))
	{
		return;
	}
	else if (screenRect.Contains(spriteRect))
	{
		sprite->Draw(m_screen, m_screenSize, pixelPos, tileIndex, rect);
	}
	else
	{
		spriteRect.ClipTo(screenRect);
		spriteRect.Translate(-centerPos - veciOffset);

		spriteRect.Clamp(screenRect);

		sprite->Draw(m_screen, m_screenSize, pixelPos, tileIndex, spriteRect);
	}
}

void Renderer::InstanceDraw(int id, const std::string& spriteName, const Vector2f& pos, const Vector2f& lastPos, float percentage)
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
		m_instancedSprites.emplace(id, InstancedSprite(InstancedSprite::Sprite, spriteName, pos, lastPos,percentage));
	}
	else
	{
		//update existing sprite position
		m_instancedSprites.at(id).position = pos;
		m_instancedSprites.at(id).lastPosition = lastPos;
		m_instancedSprites.at(id).percentage = percentage;
	}
}

void Renderer::InstanceDrawTile(int id, const std::string& spriteName, const Vector2f& pos, const Vector2f& lastPos,
	int index, float percentage)
{
	if (spriteName.empty())
		return;

	if (m_sprites.find(spriteName) == m_sprites.end())
	{
		std::cout << "Can not queue draw sprite '" << spriteName << "' as it doesn't the sprite doesn't exist\n";
		return;
	}

	//check if tile exists
	if (m_instancedSprites.find(id) == m_instancedSprites.end())
	{
		//Insert new instance sprite to be drawn
		m_instancedSprites.emplace(id, InstancedSprite(InstancedSprite::Tile, spriteName, pos, lastPos, percentage, index));
	}
	else
	{
		//update existing sprite position
		m_instancedSprites.at(id).position = pos;
		m_instancedSprites.at(id).lastPosition = lastPos;
		m_instancedSprites.at(id).frame = index;
		m_instancedSprites.at(id).percentage = percentage;
	}
}

void Renderer::InstanceDrawAnimation(int id, const std::string& spriteName,const Vector2f& pos, const Vector2f& lastPos, int frame, int& startFrame, int& endFrame, float percentage)
{
	if(spriteName.empty())
		return;

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
		m_instancedSprites.emplace(id, InstancedSprite(InstancedSprite::Animated, spriteName, pos, lastPos,percentage, frame));
	}
	else
	{
		//update existing sprite position
		m_instancedSprites.at(id).position = pos;
		m_instancedSprites.at(id).lastPosition = lastPos;
		m_instancedSprites.at(id).frame = frame;
		m_instancedSprites.at(id).percentage = percentage;
	}
}

void Renderer::RemoveInstance(int id)
{
	if (m_instancedSprites.find(id) != m_instancedSprites.end())
		m_instancedSprites.erase(id);
}

void Renderer::DrawInstancedSprites(float s)
{
	m_lerpValue = s;

	//update offset (Camera)
	Vector2f interpolatedPosition = Lerp(m_targetOffset, m_lastOffset, s*.25f);
	m_offset = interpolatedPosition;

	//Loop through all instance sprite and draw them
	for (auto& queued_sprite : m_instancedSprites)
	{
		interpolatedPosition = Lerp(queued_sprite.second.position, queued_sprite.second.lastPosition, s);

		if(queued_sprite.second.percentage != 1.0f)
		{
			Rect rect = GetSpriteRect(*queued_sprite.second.spriteName);
			float width = (float)rect.Width() * queued_sprite.second.percentage;
			rect.Right = width;

			switch (queued_sprite.second.spriteType)
			{
			case InstancedSprite::Sprite:
				Draw(*queued_sprite.second.spriteName, interpolatedPosition, rect);
				break;
			case InstancedSprite::Animated:
				DrawAnimation(*queued_sprite.second.spriteName, interpolatedPosition, queued_sprite.second.frame, rect);
				break;
			case InstancedSprite::Tile:
				DrawTile(*queued_sprite.second.spriteName, interpolatedPosition, queued_sprite.second.frame, rect);
				break;
			default:
				return;
			}
		}
		else 
		{
			switch (queued_sprite.second.spriteType)
			{
			case InstancedSprite::Sprite:
				Draw(*queued_sprite.second.spriteName, interpolatedPosition);
				break;
			case InstancedSprite::Animated:
				DrawAnimation(*queued_sprite.second.spriteName, interpolatedPosition, queued_sprite.second.frame);
				break;
			case InstancedSprite::Tile:
				DrawTile(*queued_sprite.second.spriteName, interpolatedPosition, queued_sprite.second.frame);
				break;
			default:
				return;
			}
		}
	}

	//Set Last Frame time
	m_lastTime = HAPI.GetTime();
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

Rect Renderer::GetSpriteRect(const std::string& spriteName) const
{
	if (m_sprites.find(spriteName) != m_sprites.end()) {
		Sprite* sprite = m_sprites.at(spriteName);
		return Rect(0, sprite->GetWidth(), 0, sprite->GetHeight());
	}
	return Rect();
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
