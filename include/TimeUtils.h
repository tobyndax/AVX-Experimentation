#pragma once
#include <chrono>

#define timeNow() std::chrono::high_resolution_clock::now()

typedef std::chrono::high_resolution_clock::time_point Time;
typedef std::chrono::high_resolution_clock::duration duration;

long long durationNano(const duration& a) {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(a).count();
}
long long durationMicro(const duration& a) {
	return std::chrono::duration_cast<std::chrono::microseconds>(a).count();
} 

long long durationMilli(const duration& a) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(a).count();
}
long long durationSeconds(const duration& a) {
	return std::chrono::duration_cast<std::chrono::seconds>(a).count();
}
