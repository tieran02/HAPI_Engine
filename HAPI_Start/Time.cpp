#include "Time.hpp"
#include <HAPI_lib.h>


Time& Time::Instance()
{
	static Time instance; 
	return instance;
}

void Time::startUpdateTime()
{
	m_deltaTime = (HAPI.GetTime() / 1000.0f) - (m_lastTime / 1000.0f);
}

void Time::endUpdateTime()
{
	m_lastTime = HAPI.GetTime();
}