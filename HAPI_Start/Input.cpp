#include "Input.h"
#include <HAPI_lib.h>

using namespace HAPISPACE;

bool Input::m_controllers[4];
float Input::m_horizontal{};
float Input::m_vertical{};

Input::Input()
{
}


Input::~Input()
{
}

void Input::CheckControllers()
{
	for (size_t i = 0; i < 4; i++)
	{
		const auto controller = HAPI.GetControllerData(i);
		m_controllers[i] = controller.isAttached;
	}
}

int Input::GetPlayerControllerID(int player)
{
	//get the controller that is attached corresponding to the number of the player
	int currentPlayer = 1;
	for (size_t i = 0; i < 4; ++i)
	{
		if (m_controllers[i] && currentPlayer == player)
			return i;
		else if (m_controllers[i] && currentPlayer != player)
			currentPlayer++;
	}
	return -1;
}

void Input::ResetControllers()
{
	for (size_t i = 0; i < 4; ++i)
	{
		if (m_controllers[i])
		{
			HAPI.SetControllerRumble(i, 0, 0);
		}
	}
}

Vector2f Input::JoystickDirection(int controllerID, int joystick)
{
	if (m_controllers[controllerID])
	{
		auto const controller = HAPI.GetControllerData(controllerID);
		int x, y, deadzone;
		if (joystick == 1) {
			deadzone = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
			x = std::abs(controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X]) > deadzone
						? controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X]
						: 0;
			y = std::abs(controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y]) > deadzone
				        ? controller.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y]
				        : 0;
		}else
		{
			deadzone = HK_GAMEPAD_RIGHT_THUMB_DEADZONE;
			x = std::abs(controller.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X]) > deadzone
				? controller.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_X]
				: 0;
			y = std::abs(controller.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y]) > deadzone
				? controller.analogueButtons[HK_ANALOGUE_RIGHT_THUMB_Y]
				: 0;
		}

		float nx =  (float)x  / 32767.0f;
		float ny = (float)y  / 32767.0f;
		Vector2f direction = Vector2f(nx, -ny);
	return direction;
	}
	return Vector2f(0.0f, 0.0f);
}

const float& Input::GetAxis(std::string axisName)
{
	if (axisName == "Horizontal")
		return m_horizontal;
	else if(axisName == "Vertical")
		return m_vertical;
}
