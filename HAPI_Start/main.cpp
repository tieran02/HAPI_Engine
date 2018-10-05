/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file
	
	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI:
	https://scm-intranet.tees.ac.uk/users/u0018197/hapi.html
*/

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>
//Include mathmatical vector struct
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Texture.hpp"

// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

// ---Function prototypes---

//Clear the screen buffer to a a colour value (4 bytes, Pass in screen size as int)
void ClearScreen(int screenWidth, int screenHeight, const HAPI_TColour& colour);
//Clear the screen buffer to a a colour value (4 bytes, Pass in screen size as Vector2i)
void ClearScreen(const Vector2i& screenSize, const HAPI_TColour& colour);

//Clear the screen buffer to a gray scale value (Between 0-255, Pass in screen size as int)
void ClearScreen(int screenWidth, int screenHeight ,BYTE grayScale);
//Clear the screen buffer to a gray scale value (Between 0-255, Pass in screen size as Vector2i)
void ClearScreen(const Vector2i& screenSize, BYTE grayScale);

//Set a specific pixel on the screen to a certain colour (Pass in position and screen size as Vector2i)
void SetPixel(const Vector2i& vec, const Vector2i& screenSize, const HAPI_TColour& colour);
//Set a specific pixel on the screen to a certain colour (Pass in position and screen size as int)
void SetPixel(int x, int y, int screenWidth, int screenHeight, const HAPI_TColour& colour);

//Project a 3D world space to the Screen space
Vector2i ProjectPixel(const float& eyeDistance, const Vector2i& screenSize, const Vector3f& sourcePos);

//Star struct to hold all the data for a single star
struct Star
{
	Vector3f position; //Star position is a Vector3 float
	float speed{ 1.0f }; //The speed the star moves at
	HAPI_TColour colour{ HAPI_TColour::CYAN }; //The base color of a star
};

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{
	Vector2i screen_size{ 1280,720 };

	if (!HAPI.Initialise(screen_size.x, screen_size.y, "HAPI - T7042361"))
		return;

	//Star vars
	const unsigned int star_amount{ 1000000 };
	const int star_distance{ 2000 };
	const auto stars{ new Star[star_amount] };

	//init stars pos
	for(size_t i = 0; i < star_amount;i++)
	{
		stars[i].position.x = static_cast<float>(rand() % screen_size.x);
		stars[i].position.y = static_cast<float>(rand() % screen_size.y);
		stars[i].position.z = static_cast<float>(rand() % star_distance);

		//Calculate a random colour
		const HAPI_TColour random_color = { static_cast<BYTE>(rand() % 255),
											static_cast<BYTE>(rand() % 255),
											static_cast<BYTE>(rand() % 255) };
		stars[i].colour = random_color;
	}

	//eye distance used for projection
	float eye_distance = 100.0f;
	//keyboard data
	const auto& key_data = HAPI.GetKeyboardData();

	//load texture
	Texture tex("Data\\star.jpg");

	//Get Screen pointer (Top left pixel)
	BYTE* screen = HAPI.GetScreenPointer();

	//Test variables
	bool clear_screen_to_color{ false };
	bool random_star_colors{ true };

	//Start the game loop
	while (HAPI.Update())
	{
		if (key_data.scanCode['3'])
		{
			//Calculate a random colour
			const HAPI_TColour random_color = { static_cast<BYTE>(rand() % 255),
												static_cast<BYTE>(rand() % 255),
												static_cast<BYTE>(rand() % 255) };

			ClearScreen(screen_size, random_color);
		}
		else if(clear_screen_to_color)
			ClearScreen(screen_size, HAPI_TColour::YELLOW);
		else //Clear screen to a grayscale value (Fastest)
			ClearScreen(screen_size, 50);

		//Debug text
		HAPI.RenderText(10, 10, HAPI_TColour::WHITE, HAPI_TColour::BLACK, 1.0f, "Press '1' to clear screen to Yellow");
		HAPI.RenderText(10, 20, HAPI_TColour::WHITE, HAPI_TColour::BLACK, 1.0f, "Press '2' to clear screen to grayscale");
		HAPI.RenderText(10, 30, HAPI_TColour::WHITE, HAPI_TColour::BLACK, 1.0f, "Press '3' to clear screen to random colours");
		HAPI.RenderText(10, 40, HAPI_TColour::WHITE, HAPI_TColour::BLACK, 1.0f, "Press '4' to set star colour to random colours");
		HAPI.RenderText(10, 50, HAPI_TColour::WHITE, HAPI_TColour::BLACK, 1.0f, "Press '5' to set star colour to cyan");

		//Keyboard inputs
		//Increase/Decrease eye distance with W and S
		if (key_data.scanCode['W']) {
			eye_distance += 10;
		}
		if (key_data.scanCode['S']) {
			eye_distance -= 10;
		}
		//Set the clear screen to color to True if 1 or false if 2
		if(key_data.scanCode['1'])
		{
			clear_screen_to_color = true;
		}
		else if (key_data.scanCode['2'])
		{
			clear_screen_to_color = false;
		}

		//Set the stars to random colours to True if 3 or false if 4
		if (key_data.scanCode['4'])
		{
			random_star_colors = true;
		}
		else if (key_data.scanCode['5'])
		{
			random_star_colors = false;
		}

		//loop through all the stars
		for(size_t i = 0; i < star_amount; i++)
		{
			//check if the z position and set it to a random distance if <= 0
			if (stars[i].position.z <= 0)
				stars[i].position.z = static_cast<float>(rand() % star_distance);
			else //else decrement the star Z by the stars speed
				stars[i].position.z -= stars[i].speed;

			//Calulate the projected pixel from 3D space to screen Space
			auto projectedPixel = ProjectPixel(eye_distance, screen_size, Vector3f(stars[i].position.x,
				stars[i].position.y, stars[i].position.z));

			//Set the pixel either to a random or set colour at the projected pixel position
			if(random_star_colors)
				SetPixel(projectedPixel, screen_size, stars[i].colour);
			else
				SetPixel(projectedPixel, screen_size, HAPI_TColour::CYAN);

			//Stars as textures WIP
			//tex.Blit(screen, screen_size, projectedPixel);
			//tex.Blit(screen, screen_size, stars[i].position,eye_distance);
		}
	}

	//delete star positions
	delete[] stars;
}

void SetPixel(const Vector2i& vec, const Vector2i& screenSize, const HAPI_TColour& colour)
{
	//Call the set pixel function and pass the vector values as separate values
	SetPixel(vec.x, vec.y, screenSize.x, screenSize.y, colour);
}

void SetPixel(int x, int y, int screenWidth, int screenHeight, const HAPI_TColour& colour)
{
	//check if the position is within the screen buffer
	if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
		//Get the screen pointer
		BYTE* screen = HAPI.GetScreenPointer();

		//calculate the offset
		const unsigned int offset = (x + y * screenWidth) * 4;
		//Add the offset to the screen pixel
		BYTE* offset_ptr = screen + offset;
		//Cast the pixel to a colour and set it to the new colour value
		*(HAPI_TColour*)offset_ptr = colour;
	}
}

Vector2i ProjectPixel(const float& eyeDistance, const Vector2i& screenSize, const Vector3f& sourcePos)
{
	//Check if the Z is less than or equal to 0, If true then return the source position without doing any calculation
	if (sourcePos.z <= 0)
		return Vector2i(static_cast<int>(sourcePos.x), static_cast<int>(sourcePos.y));

	//Get the center of the screen as a vector2 int
	const Vector2i center_screen = screenSize/2;

	//Calculate the projected screen position from a 3D world position
	Vector2i projection;
	projection.x = static_cast<int>((eyeDistance * (sourcePos.x - center_screen.x)) / (sourcePos.z + eyeDistance)) + center_screen.x;
	projection.y = static_cast<int>((eyeDistance * (sourcePos.y - center_screen.y)) / (sourcePos.z + eyeDistance)) + center_screen.y;

	return projection;
}

void ClearScreen(int screenWidth, int screenHeight, const HAPI_TColour& colour)
{
	// Get the screen pointer
	BYTE* screen = HAPI.GetScreenPointer();

	//Loop through all the pixels within the screen buffer
	for (size_t x = 0; x < static_cast<size_t>(screenWidth) * static_cast<size_t>(screenHeight) * 4; x+=4)
	{	
		//calculate the offset pointer of the pixel
		BYTE* offset_ptr = screen + x;
		//Cast the pixel to a colour and set it to the new colour value
		*(HAPI_TColour*)offset_ptr = colour;
	}
}

void ClearScreen(const Vector2i& screenSize, const HAPI_TColour& colour)
{
	//Call the clear function and pass the vector values as separate values
	ClearScreen(screenSize.x, screenSize.y, colour);
}

void ClearScreen(int screenWidth, int screenHeight, BYTE grayScale)
{
	//Get the screen pointer
	BYTE* screen = HAPI.GetScreenPointer();
	//set the full screen buffer with the grayscale value
	memset(screen, grayScale, screenWidth * screenHeight * 4);
}

void ClearScreen(const Vector2i& screenSize, BYTE grayScale)
{
	//Call the clear function and pass the vector values as separate values
	ClearScreen(screenSize.x, screenSize.y, grayScale);
}
