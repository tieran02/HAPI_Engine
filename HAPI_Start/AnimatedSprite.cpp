#include "AnimatedSprite.hpp"

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
	m_endFrame = endFrame;

	m_frameSize.x = texture->GetWidth() / rows;
	m_frameSize.y = texture->GetHeight() / columns;


	int frameLength = endFrame - startFrame;
	m_subTextures.resize(frameLength);
	for (int i = 0; i < frameLength; ++i)
	{
		int currentRow = int((startFrame + i) / rows);
		int currentColumn = (startFrame + i) % rows;

		Rect rect(currentColumn*m_frameSize.x, (currentColumn + 1) * m_frameSize.x, currentRow*m_frameSize.y, (currentRow + 1) * m_frameSize.y);
		int offset = (rect.Left + rect.Top)* 4;
		Texture subTexture;
		subTexture.Load(texture,offset);
		m_subTextures[i] = subTexture;
	}
}

void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame,
	float speed) const
{
	Rect rect(0, m_frameSize.x, 0, m_frameSize.y);
	Draw(screen, screenSize, pos, currentFrame, speed, rect);
}

int lastTime = 0;
void AnimatedSprite::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int& currentFrame,
	float speed, Rect area) const
{
	//speed to milliseconds
	int milliSpeed = speed * 1000.0f;
	int currentTime = HAPI.GetTime();
	if (currentTime >= lastTime + milliSpeed) {
		currentFrame = (currentFrame + 1) % (m_endFrame - m_startFrame);
		lastTime = HAPI.GetTime();
	}

	if (m_subTextures[currentFrame].HasAlpha())
		m_subTextures[currentFrame].BlitAlpha(screen, screenSize, pos, area);
	else
		m_subTextures[currentFrame].BlitFast(screen, screenSize, pos, area);
}