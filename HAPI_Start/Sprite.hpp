#pragma once
#include <HAPI_lib.h>
#include "Vector2.hpp"
#include "Rect.hpp"
#include "Texture.hpp"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void Load(Texture* texture);

	virtual int GetWidth() const { return m_texture->GetWidth(); }
	virtual int GetHeight() const { return  m_texture->GetHeight(); }

	//Draw texture (detects whether to use Alpha or not automatically)
	void Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos) const;
	//Draw specific area of texture (detects whether to use Alpha or not automatically)
	void Draw(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const;

protected:
	Texture* m_texture{nullptr};
};

