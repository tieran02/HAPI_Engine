#pragma once
#include "Vector2.hpp"

class Input
{
public:
	Input();
	~Input();

	static void CheckControllers();
	static int GetPlayerControllerID(int player);
	static void ResetControllers();
	static Vector2f JoystickDirection(int controllerID, int joystick);
	static const float& GetAxis(std::string axisName);

private:
	static bool m_controllers[4];
	static float m_horizontal;
	static float m_vertical;
};

