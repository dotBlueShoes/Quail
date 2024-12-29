// Created 2024.12.29 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "types.hpp"
#include "../types.hpp"

namespace WINDOWS::CONTROLS {

	const c16 BUTTON_CLASS[] { L"BUTTON" };

	void CreateButton (
		HWND& window, 
		const HWND& parentWindow, 
		const HINSTANCE& instance, 
		const pair<u16>& position,
		const pair<u16>& size,
		const c16* const& text = L"Click Me!"
	) {
		window = CreateWindowExW (
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