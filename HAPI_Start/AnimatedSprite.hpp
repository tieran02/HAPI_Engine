#pragma once
#include "Sprite.hpp"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite();
	~AnimatedSprite() override;

	void Load(Texture* texture,int rows, int columns, int startFrame, int endFrame);

	//DrawAnimation animation at the correct speed and frame
	void Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame, float speed) const;
	//DrawAnimation animation at the correct speed and frame with clipping area
	void Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame, float speed, Rect area) const;

	int GetWidth() const override { return m_frameSize.x; }
	int GetHeight() const override { return m_frameSize.y; }
private:
	Vector2i m_frameSize{ 0,0 };
	int m_lastTime{ 0 };
	int m_startFrame{ 0 },  m_endFrame{ 0 };
	//A vector of pointers to the start of each frame
	std::vector<Texture> m_subTextures;
};

