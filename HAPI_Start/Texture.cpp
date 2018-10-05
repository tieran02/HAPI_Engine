#include "Texture.hpp"



Texture::Texture(const std::string& path)
{
	if (!loadTexture(path)) {
		std::cerr << "TEXTURE::Failed to load texture from: " << path << std::endl;
		return;
	}
}


Texture::~Texture()
{
	delete[] m_texture;
}

void Texture::Blit(BYTE* screen, const Vector2i& screenSize, const Vector2i& pos)
{
	if (pos.x >= 0 && pos.x + m_width < screenSize.x && pos.y >= 0 && pos.y + m_height < screenSize.y)
	{
		BYTE* screenPnter = screen + (pos.x + pos.y * screenSize.x) * 4;
		BYTE* texturePnter = m_texture;

		for (size_t y = 0; y < m_height; y++)
		{
			memcpy(screenPnter, texturePnter, m_width * 4);
			//move texture pointer to next line
			texturePnter += m_width * 4;
			//move screen to pointer
			screenPnter += screenSize.x * 4;
		}
	}
}

void Texture::Blit(BYTE* screen, const Vector2i& screenSize, const Vector3f& pos, const float& eyeDistance)
{
	BYTE* currentTexturePixel = m_texture;
	BYTE* currentScreenPixel = screen;
	const Vector2i center_screen = screenSize / 2;
	

	//for each pixel
	int currentPixelOnRow = 0;
	int currentHeight = 0;
	for (int i = 0; i < m_width*m_height * 4; i+=4)
	{
		//project
		Vector2i projection;
		projection.x = static_cast<int>(eyeDistance * (pos.x + currentPixelOnRow - center_screen.x) / (pos.z + eyeDistance) + center_screen.x);
		projection.y = static_cast<int>(eyeDistance * (pos.y + currentHeight - center_screen.y) / (pos.z + eyeDistance) + center_screen.y);


		if (currentPixelOnRow == m_width)
		{
			currentScreenPixel += screenSize.x * 4;
			currentHeight++;
			currentPixelOnRow = 0;
		}else
		{
			currentScreenPixel = screen + (projection.x + projection.y * screenSize.x) * 4;;
		}

		//Get screen and texture pointer
		if (projection.x >= 0 && projection.x + m_width < screenSize.x && projection.y >= 0 && projection.y + m_height < screenSize.y) {
			memcpy(currentScreenPixel, currentTexturePixel, 4);
		}

		currentPixelOnRow++;
		currentTexturePixel += 4;
	}
}


bool Texture::loadTexture(const std::string & path)
{
	return HAPI.LoadTexture(path,&m_texture,m_width,m_height);
}
