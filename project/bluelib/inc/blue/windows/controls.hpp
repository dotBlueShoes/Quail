// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"
#include "../types.hpp"

namespace WINDOWS::CONTROLS {

	const c16 BUTTON_CLASS[] { L"BUTTON" };

	void CreateProgressBarRange (
		HWND& progressBar, 
		const HWND& parentWindow, 
		const HINSTANCE& instance,
		const DWORD& style,
		const pair<u16>& position,
		const pair<u16>& size,
		const pair<u16>& range
	) {
		progressBar = CreateWindowExW (
			0, PROGRESS_CLASSW, 
			nullptr, 
			WS_CHILD | WS_VISIBLE, // PBS_MARQUEE, PBS_SMOOTH, PBS_SMOOTHREVERSE, PBS_VERTICAL
			position.x, position.y, size.x, size.y, 
			parentWindow, 
			(HMENU) 200, 
			instance, 
			nullptr
		);

		// Setting up the numeric range of progress bar.
		SendMessageW (progressBar, PBM_SETRANGE, 0, (LPARAM) MAKELONG (range.x, range.y));
	}

	void CreateProgressBarRangeStep (
		HWND& progressBar, 
		const HWND& parentWindow, 
		const HINSTANCE& instance,
		const DWORD& style,
		const pair<u16>& position,
		const pair<u16>& size,
		const pair<u16>& range,
		const u16 stepSize
	) {
		progressBar = CreateWindowExW (
			0, PROGRESS_CLASSW, 
			nullptr, 
			WS_CHILD | WS_VISIBLE, // PBS_MARQUEE, PBS_SMOOTH, PBS_SMOOTHREVERSE, PBS_VERTICAL
			position.x, position.y, size.x, size.y, 
			parentWindow, 
			(HMENU) 200, 
			instance, 
			nullptr
		);

		// Setting up the numeric range of progress bar.
		SendMessageW (progressBar, PBM_SETRANGE, 0, (LPARAM) MAKELONG (range.x, range.y));
		// Sets the step size.
		SendMessageW (progressBar, PBM_SETSTEP, (WPARAM) stepSize, 0); 
	}

	void CreateButton (
		HWND& button, 
		const HWND& parentWindow, 
		const HINSTANCE& instance, 
		const pair<u16>& position,
		const pair<u16>& size,
		const c16* const& text = L"Click Me!"
	) {
		button = CreateWindowExW (
			0, BUTTON_CLASS,
			text,
			WS_CHILD | WS_VISIBLE,
			position.x, position.y,
			size.x, size.y,
			parentWindow,
			nullptr,
			instance,
			nullptr
		);
	}

}