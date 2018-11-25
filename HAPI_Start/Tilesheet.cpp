#include "Tilesheet.hpp"



Tilesheet::Tilesheet()
{
}


Tilesheet::~Tilesheet()
{
}

void Tilesheet::Load(Texture* texture, int rows, int columns)
{
	m_texture = texture;

	m_frameSize.x = texture->GetWidth() / rows;
	m_frameSize.y = texture->GetHeight() / columns;


	m_subTextures.resize(rows * columns);
	for (int i = 0; i < m_subTextures.size(); ++i)
	{
		int currentRow = int(i / rows);
		int currentColumn = i % rows;

		Rect rect(currentColumn*m_frameSize.x, (currentColumn + 1) * m_frameSize.x, currentRow*m_frameSize.y, (currentRow + 1) * m_frameSize.y);
		int offset = rect.Left * 4;
		int heightOffset = currentRow * texture->GetWidth() * m_frameSize.y * 4;;
		Texture subTexture;
		subTexture.Load(texture, offset + heightOffset, m_frameSize.x,m_frameSize.x);
		m_subTextures[i] = subTexture;
	}
}

void Tilesheet::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int tileIndex) const
{
	Rect rect(0, m_frameSize.x, 0, m_frameSize.y);
	Draw(screen, screenSize, pos, tileIndex, rect);
}

void Tilesheet::Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, int tileIndex, Rect area) const
{
	if (m_subTextures[tileIndex].HasAlpha())
		m_subTextures[tileIndex].BlitAlpha(screen, screenSize, pos, area);
	else
		m_subTextures[tileIndex].BlitFast(screen, screenSize, pos, area);
}
