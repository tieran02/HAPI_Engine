#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include <unordered_map>

#include <HAPI_lib.h>
#include "Texture.hpp"


class Tilesheet;
class AnimatedSprite;
class Sprite;

struct InstancedSprite
{
	enum Type
	{
		Sprite,
		Animated,
		Tile
	};

	InstancedSprite(Type spriteType, const std::string& sprite_name, const Vector2i& pos, int* frame = nullptr, HAPISPACE::DWORD* last_time = nullptr,
		float* speed = nullptr, bool loop = true, int* endFrame = nullptr)
		: spriteType(spriteType),
		  spriteName(&sprite_name),
		  position(pos),
		  frame(frame), loop(loop), endFrame(endFrame),
		  lastTime(last_time),
		  speed(speed)
	{
	}

	const Type spriteType;
	const std::string* spriteName;
	Vector2i position;
	int* frame;

	bool loop;
	int* endFrame;
	HAPISPACE::DWORD* lastTime;
	float* speed;
};

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
	//DrawAnimation a sprite and clip it to the screen (Vector2i as position)
	void Draw(const std::string& spriteName, const Vector2i& pos);
	//DrawAnimation a sprite and clip it to the screen (Vector2f as position)
	void Draw(const std::string& spriteName, const Vector2f& pos);
	//DrawAnimation a sprite and clip it to the screen (Vector3f as position)
	void Draw(const std::string& spriteName, const Vector3f& pos);
	///Animated Sprites
	//DrawAnimation a animated sprite and clip it to the screen (Vector2i as position)
	void DrawAnimation(const std::string& animationName, const Vector2i& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed);
	//DrawAnimation a animated sprite and clip it to the screen (Vector2f as position)
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed);
	//DrawAnimation a animated sprite and clip it to the screen (Vector3f as position)
	void DrawAnimation(const std::string& animationName, const Vector3f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed);
	///Tiles
	//Draw a single tile from a tilesheet and clip it to the screen (Vector2i as position)
	void DrawTile(const std::string& tilesheetName, const Vector2i& pos, int tileIndex);
	//Draw a single tile from a tilesheet and clip it to the screen (Vector2f as position)
	void DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex);
	//Draw a single tile from a tilesheet and clip it to the screen (Vector3f as position)
	void DrawTile(const std::string& tilesheetName, const Vector3f& pos, int tileIndex);

	//Add a new sprite to always be drawn on each frame.
	void InstanceDraw(int id,const std::string& spriteName, Vector2i& pos);
	//Add a new animated sprite to always be drawn on each frame.
	void InstanceDrawAnimation(int id, const std::string& spriteName, Vector2i& pos, int& frame, HAPISPACE::DWORD& lastTime, float& speed, bool loop, int& endFrame);

	//Remove instance which will get deleted at the beginning of the next draw stage
	void RemoveInstance(int id);
	//Draw all instanced sprites
	void DrawInstancedSprites();

	//Load a sprite into the sprite map
	void LoadTexture(std::string name, const std::string& path);

	void Cleanup();

private:
	Vector2i m_screenSize;
	HAPISPACE::BYTE* m_screen{nullptr};
	float m_eyeDistance{ 100.0f };
	Vector2i m_offset{0,0};

	HAPISPACE::DWORD m_lastTime{ 0 };

	//texture map
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Sprite*> m_sprites;

	//Instance sprite a sprites that will always be drawn if they exist in the map ( used to sync the ECS with the render system)
	std::unordered_map<int, InstancedSprite> m_instancedSprites;
	//Flagged instances will get deleted at the beginning of the next draw stage
	std::vector<int> flaggedInstances;

	//Project a 3D world space to the Screen space
	Vector2i projectPosition(const Vector3f& sourcePos);

};

