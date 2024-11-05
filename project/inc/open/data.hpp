// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"

#include <vector>

namespace OPEN { 

	enum SPECIAL_CHARACTERS : c8 {
		TYPE_PROJECT			= '^',
		TYPE_INCLUDE 			= '&',
		TYPE_COMMAND 			= '!',
		TYPE_QUEUE				= '|',
		TYPE_CONSTANT 			= '%',
		TYPE_VARIABLE 			= '@',
		TYPE_SECRET 			= '?',
		TYPE_COMMENT			= '#',
		TYPE_ASSIGN				= '=',
		//
		TYPE_TAB 				= '\t',
		TYPE_NEW_LINE 			= '\n',
		TYPE_EOF				= '\0',
		TYPE_EOS				= '\0',
		TYPE_CARRIAGE_RETURN	= 13,
		TYPE_SPACE 				= ' ',
	};

	u32 mainConfigFolderPathLength;
	u32 mainConfigFilePathLength;
	c16* mainConfigFilePath;

	u32 temporaryLength;
	u8 temporary[2048];

	void AddTemp (const c8& value) {
		temporary[temporaryLength] = value;
		++temporaryLength;
	}

	void AddTempW (const c8& value) {
		temporary[temporaryLength] = (u8)(value);
		++temporaryLength;
		temporary[temporaryLength] = 0;
		++temporaryLength;
	}

	void AddTemp (const c16& value) {
		temporary[temporaryLength] = (u8)(value >> 8);
		++temporaryLength;
		temporary[temporaryLength] = (u8)(value >> 0);
		++temporaryLength;
	}

	std::vector<u8*> includes;

}