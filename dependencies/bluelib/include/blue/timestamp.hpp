// Created 2024.11.30 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
//
#include <ctime>
#include <chrono>
//
#include "types.hpp"

namespace TIMESTAMP {

	using Duration 	= std::chrono::duration<r32>;
	using Clock 	= std::chrono::system_clock;
	using Timestamp = Clock::time_point;

	Timestamp GetCurrent() {
		return Clock::now ();
	}

    auto GetElapsed (IN const Timestamp& previous) {
		auto current = Clock::now ();
		Duration elapsed = current - previous;
		return elapsed.count ();
	}

}
