#pragma once
#include "Vector2.hpp"
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

	InstancedSprite(Type spriteType, const std::string& sprite_name, const Vector2f& pos, const Vector2f& lastPos, float percentage = 1.0f,int frame = -1, int* endFrame = nullptr)
		: spriteType(spriteType),
		  spriteName(&sprite_name),
		  position(pos),
		  lastPosition(lastPos),
		  frame(frame),
		  percentage(percentage){
	}

	const Type spriteType;
	const std::string* spriteName;
	Vector2f position;
	Vector2f lastPosition;
	int frame;
	float percentage;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Intialise(const Vector2i& screenSize, const std::string& windowName);

	const Vector2i& GetSize() const { return m_screenSize; };
	const Vector2f& GetOffset() const { return m_offset; };
	void SetOffset(const Vector2f& offset);

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
	//Draw a sprite and clip it to the screen (Vector2f as position)
	void Draw(const std::string& spriteName, const Vector2f& pos);
	//Draw a section of a sprite and clip it to the screen (Vector2f as position)
	void Draw(const std::string& spriteName, const Vector2f& pos, Rect area);

	///Animated Sprites
	//DrawAnimation a animated sprite and clip it to the screen (Vector2f as position)
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed);
	//DrawAnimation a section of the animated sprite and clip it to the screen (Vector2f as position)
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed, const Rect& area);

	//Draw Animation at one given frame
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int currentFrame);
	//Draw Animation section at one given frame
	void DrawAnimation(const std::string& animationName, const Vector2f& pos, int currentFrame, const Rect& area);
	//Retrieve common data about the animation
	void GetAnimationFrameData(const std::string& animationName, int& StartFrame, int& EndFrame);

	///Tiles
	//Draw a single tile from a tilesheet and clip it to the screen (Vector2f as position)
	void DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex);
	//Draw a sectionb of a single tile from a tilesheet and clip it to the screen (Vector2f as position)
	void DrawTile(const std::string& tilesheetName, const Vector2f& pos, int tileIndex, const Rect& rect);

	//Add a new sprite to always be drawn on each frame.
	void InstanceDraw(int id,const std::string& spriteName, const Vector2f& pos, const Vector2f& lastPos, float percentage = 1.0f);
	//Add a new sprite to always be drawn on each frame.
	void InstanceDrawTile(int id, const std::string& spriteName, const Vector2f& pos, const Vector2f& lastPos, int index, float percentage = 1.0f);
	//Add a new animated sprite to always be drawn on each frame.
	void InstanceDrawAnimation(int id, const std::string& spriteName, const Vector2f& pos, const Vector2f& lastPos, int frame, int& startFrame, int& endFrame, float percentage = 1.0f);

	//Remove instance which will get deleted at the beginning of the next draw stage
	void RemoveInstance(int id);
	//Draw all instanced sprites
	void DrawInstancedSprites(float s);

	//Load a sprite into the sprite map
	void LoadTexture(std::string name, const std::string& path);

	Rect GetSpriteRect(const std::string& spriteName) const;
	void Cleanup();

private:
	Vector2i m_screenSize;
	HAPISPACE::BYTE* m_screen{nullptr};
	float m_deltaFrameTime{ 0.0f };
	float m_eyeDistance{ 100.0f };
	float m_lerpValue{ 0.0f };
	Vector2f m_offset{0,0};
	Vector2f m_targetOffset{ 0,0 };
	Vector2f m_lastOffset{ 0,0 };

	HAPISPACE::DWORD m_lastTime{ 0 };

	//texture map
	std::unordered_map<std::string, Texture*> m_textures;
	std::unordered_map<std::string, Sprite*> m_sprites;

	//Instance sprite a sprites that will always be drawn if they exist in the map ( used to sync the ECS with the render system)
	std::unordered_map<int, InstancedSprite> m_instancedSprites;

	Vector2f Lerp(const Vector2f& pos, const Vector2f& lastPos, float s);

};

