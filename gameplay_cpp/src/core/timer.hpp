#pragma once

#include <chrono>

class Timestamp {
	std::chrono::system_clock::time_point timestamp;

public:
	Timestamp() { set_timestamp(); }
	void set_timestamp() { timestamp = std::chrono::system_clock::now(); }
	bool timestamp_within_timeframe(float timeframe_seconds) const {
		float duration_since_timestamp =
				std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timestamp)
						.count();
		float sec = (duration_since_timestamp / 1e9);
		return (duration_since_timestamp / 1e9) < timeframe_seconds;
	}
};
