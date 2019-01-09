#include <HAPI_lib.h>
#include "Input.hpp"

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
	for (int i = 0; i < 4; i++)
	{
		const auto controller = HAPI.GetControllerData(i);
		m_controllers[i] = controller.isAttached;
	}
}

int Input::GetPlayerControllerID(int player)
{
	//get the controller that is attached corresponding to the number of the player
	int currentPlayer = 1;
	for (int i = 0; i < 4; ++i)
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
	for (int i = 0; i < 4; ++i)
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

float Input::JoystickTrigger(int controllerID, int trigger)
{
	auto const controller = HAPI.GetControllerData(controllerID);
	int deadzone = HK_GAMEPAD_TRIGGER_THRESHOLD;

	int t = 0;
	
	if (trigger == 1) {
		t = std::abs(controller.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER]) > deadzone
			? controller.analogueButtons[HK_ANALOGUE_LEFT_TRIGGER]
			: 0;
	}
	else 
	{
		t = std::abs(controller.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]) > deadzone
			? controller.analogueButtons[HK_ANALOGUE_RIGHT_TRIGGER]
			: 0;
	}

	float value = (float)t / 32767.0f;

	return value;
}

const float& Input::GetAxis(std::string axisName)
{
	if (axisName == "Horizontal")
		return m_horizontal;
	return m_vertical;

}
