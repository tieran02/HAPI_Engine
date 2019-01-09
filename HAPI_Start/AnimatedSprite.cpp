#include "AnimatedSprite.hpp"
#include <algorithm>

using namespace HAPISPACE;

AnimatedSprite::AnimatedSprite()
{
}


AnimatedSprite::~AnimatedSprite()
{

}

void AnimatedSprite::Load(Texture* texture, int rows, int columns, int startFrame, int endFrame)
{
	m_texture = texture;

	m_startFrame = startFrame;
	m_endFrame = std::clamp(endFrame + 1, 0, rows*columns);

	m_frameSize.x = texture->GetWidth() / rows;
	m_frameSize.y = texture->GetHeight() / columns;


	int frameLength = m_endFrame - m_startFrame;
	m_subTextures.resize(frameLength);
	for (int i = 0; i < frameLength; ++i)
	{
		int currentRow = int((startFrame + i) / rows);
		int currentColumn = (startFrame + i) % rows;

		Rect rect((float)currentColumn*(float)m_frameSize.x, (float)(currentColumn + 1) * (float)m_frameSize.x, (float)currentRow*(float)m_frameSize.y, (float)(currentRow + 1) * (float)m_frameSize.y);
		int offset = (int)rect.Left * 4;
		int heightOffset = currentRow * texture->GetWidth() * m_frameSize.y * 4;;
		Texture subTexture;
		subTexture.Load(texture,offset + heightOffset,m_frameSize.x,m_frameSize.y);
		m_subTextures[i] = subTexture;
	}
}

void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed) const
{
	Rect rect(0, (float)m_frameSize.x, 0, (float)m_frameSize.y);
	Draw(screen, screenSize, pos, currentFrame, lastTime, speed, rect);
}


void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame, HAPISPACE::DWORD& lastTime, float speed, Rect area) const
{
	//speed to milliseconds
	float milliSpeed = speed * 1000.0f;
	HAPISPACE::DWORD currentTime = HAPI.GetTime();
	if (currentTime >= lastTime + milliSpeed) {
		currentFrame = (currentFrame + 1) % (m_endFrame - m_startFrame);
		lastTime = HAPI.GetTime();
	}

	if (currentFrame < m_subTextures.size())
	{
		if (m_subTextures[currentFrame].HasAlpha())
			m_subTextures[currentFrame].BlitAlpha(screen, screenSize, pos, area);
		else
			m_subTextures[currentFrame].BlitFast(screen, screenSize, pos, area);
	}
}

void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int currentFrame) const
{
	Rect rect(0, (float)m_frameSize.x, 0, (float)m_frameSize.y);
	Draw(screen, screenSize, pos, currentFrame, rect);
}

void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int currentFrame,
	Rect area) const
{
	if (currentFrame < m_subTextures.size())
	{
		if (m_subTextures[currentFrame].HasAlpha())
			m_subTextures[currentFrame].BlitAlpha(screen, screenSize, pos, area);
		else
			m_subTextures[currentFrame].BlitFast(screen, screenSize, pos, area);
	}
}
