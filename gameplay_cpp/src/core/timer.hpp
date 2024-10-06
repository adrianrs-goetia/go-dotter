#pragma once

#include <chrono>

class Timestamp {
	std::chrono::system_clock::time_point timestamp;

public:
	Timestamp() : timestamp(std::chrono::system_clock::now()) {}
	bool timestamp_within_timeframe(float timeframe_seconds) {
		float duration_since_timestamp =
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timestamp)
						.count();
		float sec = (duration_since_timestamp / 1e9);
		return (duration_since_timestamp / 1e9) < timeframe_seconds;
	}
};

// /**
//  * Helper class to hold data. Can be used to enter the same values into several TimerManager's
//  */
// class TimerCallback {
// 	std::function<void()> m_callback;
// 	float m_time;
// 	uint8_t m_active : 1;
// 	uint8_t m_loop : 1;

// public:
// 	void setValues(std::function<void()>&& callback, float time, bool looping) {
// 		m_callback = callback;
// 		m_time = time;
// 		m_active = true;
// 		m_loop = looping;
// 	}
// };

// auto constexpr s = sizeof(TimerCallback);

// /**
//  * Collection of timers with lambda callbacks
//  */
// class TimerManager {
// 	std::vector<bool> m_active;
// 	std::vector<bool> m_looping;
// 	std::vector<float> m_times;
// 	std::vector<std::function<void()>> m_callbacks;
// };