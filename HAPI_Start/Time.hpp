#pragma once
#include <HAPI_Types.h>
class Game;

class Time
{
	friend Game;
public:
	static Time& Instance();
	Time(Time const&) = delete;
	void operator=(Time const&) = delete;

	float DeltaTime() { return m_deltaTime; }
	HAPISPACE::DWORD LastTime() { return m_lastTime; }

private:
	Time() {}
	HAPISPACE::DWORD m_lastTime{ 0 };
	float m_deltaTime;

	void startUpdateTime();
	void endUpdateTime();
};

