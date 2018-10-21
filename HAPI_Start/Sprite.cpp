#include "Sprite.hpp"

using namespace HAPISPACE;

Sprite::Sprite()
{

}

Sprite::~Sprite()
{
	delete[] m_texture;
}

void Sprite::Load(const std::string& path)
{
	if (!loadTexture(path)) {
		std::cerr << "TEXTURE::Failed to load texture from: " << path << std::endl;
		HAPI.UserMessage("Texture at path '" + path + "' couldn't open!", "ERROR");
		return Load("Data\\NoTexture.jpg");
	}
	//check whether sprite has an alpha channel
	m_hasAlpha = checkAlpha();
}

bool Sprite::loadTexture(const std::string & path)
{
	return HAPI.LoadTexture(path,&m_texture,m_width,m_height);
}

bool Sprite::checkAlpha()
{
	//check each pixel for alpha
	if (m_texture == nullptr)
		return false;

	const BYTE* currentTexturePixel = m_texture;
	for (int i = 0; i < (m_width*m_height * 4); i += 4)
	{
		BYTE alpha = currentTexturePixel[3];

		if (alpha < 255)
			return true;

		currentTexturePixel += 4;
	}
	return false;
}
