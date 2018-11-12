#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <unordered_map>

#include <HAPI_lib.h>
#include "Texture.hpp"


class Tilesheet;
class AnimatedSprite;
class Sprite;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Intialise(const Vector2i& screenSize, const std::string& windowName);

	const Vector2i& GetSize() const { return m_screenSize; };

	//Clear the screen buffer to a a colour value (4 bytes)
	void ClearScreen(const HAPISPACE::HAPI_TColour& colour);
	//Clear the screen buffer to a gray scale value (Between 0-255)
	void ClearScreen(HAPISPACE::BYTE grayScale);
	//Clear the screen buffer to a gray scale value (Between 0-255)
	void ClearScreen(const std::string& spriteName);

	//Set a specific pixel on the screen to a certain colour (Pass in position Vector2i)
	void SetPixel(const Vector2i& vec, const HAPISPACE::HAPI_TColour& colour);
	//Set a specific pixel on the screen to a certain colour (Pass in position as int)
	void SetPixel(int x, int y, const HAPISPACE::HAPI_TColour& colour);

	void LoadSprite(const std::string& spriteName, const std::string& textureName);
	void LoadAnimatedSprite(const std::string& animationName, const std::string& textureName, int rows, int columns, int startFrame, int endFrame);
	void LoadTilesheet(const std::string& tilesheetName, const std::string& textureName, int rows, int columns);

	///Sprites
	//DrawAnimation a sprite and clip it to the screen (Vector2i as pos)
	void Draw(const std::string& spriteName, const Vector2i& pos);
	//DrawAnimation a sprite and clip it to the screen (Vector2f as pos)
	void Draw(const std::string& spriteName, const Vector2f& pos);
	//DrawAnimation a sprite and clip it to the screen (Vector3f as pos)
	void Draw(const std::string& spriteName, const Vector3f& pos);
	///Animated Sprites
	//DrawAnimation a animated sprite and clip it to the screen (Vector2i as pos)
	void DrawAnimation(const std::string& animationName, const Vector2i& pos, int& currentFrame, float speed);
	//DrawAnimation a animated sprite and clip it to the screen (Vector2f as pos)
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, float speed);
	//DrawAnimation a animated sprite and clip it to the screen (Vector3f as pos)
	void DrawAnimation(const std::string& animationName, const Vector3f& pos, int& currentFrame, float speed);
	///Tiles
	//Draw a single tile from a tilesheet and clip it to the screen (Vector2i as pos)
	void DrawTile(const std::string& tilesheetName, const Vector2i& pos, int tileIndex);
	//Draw a single tile from a tilesheet and clip it to the screen (Vector2f as pos)
	void DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex);
	//Draw a single tile from a tilesheet and clip it to the screen (Vector3f as pos)
	void DrawTile(const std::string& tilesheetName, const Vector3f& pos, int tileIndex);

	//Load a sprite into the sprite map
	void LoadTexture(std::string name, const std::string& path);

	void Cleanup();

private:
	Vector2i m_screenSize;
	HAPISPACE::BYTE* m_screen{nullptr};
	float m_eyeDistance{ 100.0f };
	Vector2i m_offset{100,100};

	//texture map
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Sprite*> m_sprites;

	//Project a 3D world space to the Screen space
	Vector2i projectPosition(const Vector3f& sourcePos);

};

