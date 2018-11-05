#include "Sprite.hpp"

using namespace HAPISPACE;

Sprite::Sprite()
{

}

Sprite::~Sprite()
{
}

void Sprite::Load(Texture* texture)
{
	m_texture = texture;
}

void Sprite::Draw(BYTE* screen, Vector2i screenSize, const Vector2i& pos) const
{
	Draw(screen, screenSize, pos, Rect(0, m_texture->GetWidth(), 0, m_texture->GetHeight()));
}

void Sprite::Draw(BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const
{
	if (m_texture->HasAlpha())
		m_texture->BlitAlpha(screen, screenSize, pos, area);
	else
		m_texture->BlitFast(screen, screenSize, pos, area);
}