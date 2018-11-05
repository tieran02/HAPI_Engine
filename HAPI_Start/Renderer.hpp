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

	//Set a specific pixel on the screen to a certain colour (Pass in position Vector2i)
	void SetPixel(const Vector2i& vec, const HAPISPACE::HAPI_TColour& colour);
	//Set a specific pixel on the screen to a certain colour (Pass in position as int)
	void SetPixel(int x, int y, const HAPISPACE::HAPI_TColour& colour);

	void LoadSprite(std::string name, Sprite& sprite);
	void LoadAnimatedSprite(std::string name, AnimatedSprite& animatedSprite, int rows, int columns, int startFrame, int endFrame);

	///Sprites
	//Draw a sprite and clip it to the screen (Vector2i as pos)
	void Draw(const Sprite& sprite, const Vector2i& pos);
	//Draw a sprite and clip it to the screen (Vector2f as pos)
	void Draw(const Sprite& sprite, const Vector2f& pos);
	//Draw a sprite and clip it to the screen (Vector3f as pos)
	void Draw(const Sprite& sprite, const Vector3f& pos);
	///Animated Sprites
	//Draw a animated sprite and clip it to the screen (Vector2i as pos)
	void Draw(const AnimatedSprite& sprite, const Vector2i& pos, int& currentFrame, float speed);
	//Draw a animated sprite and clip it to the screen (Vector2f as pos)
	void Draw(const AnimatedSprite& sprite, const Vector2f& pos, int& currentFrame, float speed);
	//Draw a animated sprite and clip it to the screen (Vector3f as pos)
	void Draw(const AnimatedSprite& sprite, const Vector3f& pos, int& currentFrame, float speed);

	//Load a sprite into the sprite map
	void LoadTexture(std::string name, const std::string& path);

	void Cleanup();

private:
	Vector2i m_screenSize;
	HAPISPACE::BYTE* m_screen{nullptr};
	float m_eyeDistance{ 100.0f };

	//texture map
	std::unordered_map<std::string, Texture*> m_textures;

	//Project a 3D world space to the Screen space
	Vector2i projectPosition(const Vector3f& sourcePos);

};

