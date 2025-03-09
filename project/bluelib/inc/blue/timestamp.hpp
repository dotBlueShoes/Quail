// Mabe by Matthew Strumillo 2024.11.30
//
#pragma once
//
#include <ctime>
#include <chrono>
//
#include "types.hpp"

namespace TIMESTAMP {

	using Duration = std::chrono::duration<r32>;
	using Clock = std::chrono::system_clock;
	using Timestamp = Clock::time_point;

	Timestamp GetCurrent() {
		return Clock::now ();
	}

    auto GetElapsed (const Timestamp& previous) {
		auto current = Clock::now ();
		Duration elapsed = current - previous;
		return elapsed.count ();
	}

}

//#define TIMESTAMP_START() TIMESTAMP::Timestamp timestampBegin = TIMESTAMP::GetCurrent ();
//{ // Get Time
//	r32 elapsedTime = TIME::GetElapsed (timeBegin);
//	LOGINFO ("Setup finished at: %f\n", elapsedTime);
//}
