// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "base/types.hpp"

#include <vector>

namespace OPEN { 

	//template <typename T>
	//struct Map {
	//	std::vector<T> key;
	//	std::vector<T> value;
	//};

	enum PATH_TYPE : u8 {
		PATH_ABSOLUTE = 0,
		PATH_RELATIVE = 1,
	};

	template <typename T>
	struct Projects {
		std::vector<T> 		keys;
		std::vector<T> 		paths;
		std::vector<T> 		configs;
		
		std::vector<u32>	configLengths;
		std::vector<u32>	pathLengths;
		std::vector<u8>		types;
	};

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
		TYPE_ABSOLUTE			= '+',
		TYPE_RELATIVE			= '-',
		//
		TYPE_TAB 				= '\t',
		TYPE_NEW_LINE 			= '\n',
		TYPE_EOF				= -1,
		TYPE_EOS				= '\0',
		TYPE_CARRIAGE_RETURN	= 13,
		TYPE_SPACE 				= ' ',
	};

	// Have on top information about main config filePath.
	u32 mainConfigFolderPathLength;
	u32 mainConfigFilePathLength;
	c16* mainConfigFilePath;

	// Have on top information about last read config filePath.
	u32 currentConfigFolderLength;
	c16* currentConfigFolder;

	// All the commands information.
	std::vector<u8*> includes;
	Projects<u8*> projects;

	// We're using those 2 to form a string that later 
	// copied and pushed to a specific vector.
	u32 temporaryLength;
	u8 temporary[2048];

	// During 1st read when we possess all the files we need to offset
	//  projects by a number that represents previously added projects at lower depth
	//  so that we always check with newly added projects when looking for subprojects.
	u32 projectsOffset = 0;

	void SetFirstTemp (const c8& value) {
		temporary[0] = value;
		temporaryLength = 0;
	}

	void SetFirstTempW (const c8& value) {
		temporaryLength = 2;
		temporary[0] = value;
		temporary[1] = 0;
	}

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

}