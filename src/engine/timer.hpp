#pragma once

#include <chrono>

class Timer {
private:
	std::chrono::high_resolution_clock::time_point lastTime;
	float time, deltaTime;
public:
	Timer();
	void update();

	float getDeltaTime() const;
	float getTime() const;
};