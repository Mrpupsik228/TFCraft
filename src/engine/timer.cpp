#include "timer.hpp"

Timer::Timer() {
	this->lastTime = std::chrono::high_resolution_clock::now();
	this->time = 0.0f;
	this->deltaTime = 0.0f;
}

void Timer::update() {
	std::chrono::_V2::system_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = currentTime - this->lastTime;

	this->lastTime = currentTime;
	this->deltaTime = deltaTime.count();
	this->time += this->deltaTime;
}

float Timer::getDeltaTime() const {
	return this->deltaTime;
}
float Timer::getTime() const {
	return this->time;
}