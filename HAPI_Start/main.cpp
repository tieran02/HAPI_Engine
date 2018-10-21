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

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK,__FILE__, __LINE__)
#endif

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>
#include "Game.hpp"
#include "GameScene.hpp"


// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

Game game("HAPI - T7042361");

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{
	game.Initialise(Vector2i(1280,720));

	//scenes
	game.AddScene<GameScene>();
	game.LoadScene("GameScene");

	game.Start();
}
