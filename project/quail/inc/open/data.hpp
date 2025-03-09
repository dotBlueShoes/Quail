// Created 2024.11.05 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/config.hpp>
#include <blue/types.hpp>
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

	enum OPEN_TYPE : u8 {
		OPEN_TYPE_LISTING = 0,
		OPEN_TYPE_COMMAND = 1,
	};

	struct ProjectsCape {

		struct Special {			// 0 - 255
			u8 type : 1;			// 0 v 128
			u8 includesCount: 7;	// 0 - 127
		} special;

		u32 configLength;
		u32 pathLength;
		
	};

	template <typename T>
	struct Projects {
		std::vector<T> 				keys;
		std::vector<T> 				paths;
		std::vector<T> 				configs;
		std::vector<ProjectsCape> 	capes;
	};

	struct Constants {
		std::vector<u8*> keys;
		std::vector<u8*> values;
		std::vector<u32> valueLengths;
	};

	struct Commands {
		std::vector<u8*> keys;
		std::vector<u8*> values;
	};

	struct Queues {
		std::vector<u8*> keys;
		std::vector<u8*> values;
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
		TYPE_SEPARATOR 			= ',',
	};

	// global.txt

	// Have on top information about last read config filePath.
	u32 currentConfigFolderLength;
	c16* currentConfigFolder;

	// All the commands information.
	std::vector<FILE*> files;
	std::vector<u8*> includes;
	Projects<u8*> projects;
	Constants constants;
	Commands commands;
	Queues queues;

	void Destroy (void*) {

		//for (s32 i = 0; i < projects.keys.size(); ++i) {
		//	FREE (projects.configs[i]);
		//	FREE (projects.paths[i]);
		//	FREE (projects.keys[i]);
		//}

		for (s32 i = 0; i < projects.configs.size(); ++i) {
			FREE (projects.configs[i]);
		}

		for (s32 i = 0; i < projects.paths.size(); ++i) {
			FREE (projects.paths[i]);
		}

		for (s32 i = 0; i < projects.keys.size(); ++i) {
			FREE (projects.keys[i]);
		}

		for (s32 i = 0; i < includes.size(); ++i) {
			FREE (includes[i]);
		}

		//for (s32 i = 0; i < constants.keys.size(); ++i) {
		//	FREE (constants.values[i]);
		//	FREE (constants.keys[i]);
		//}
		//
		//for (s32 i = 0; i < commands.keys.size(); ++i) {
		//	FREE (commands.values[i]);
		//	FREE (commands.keys[i]);
		//}
		//
		//for (s32 i = 0; i < queues.keys.size(); ++i) {
		//	FREE (queues.values[i]);
		//	FREE (queues.keys[i]);
		//}

		for (s32 i = 0; i < constants.values.size(); ++i) {
			FREE (constants.values[i]);
		}

		for (s32 i = 0; i < commands.values.size(); ++i) {
			FREE (commands.values[i]);
		}

		for (s32 i = 0; i < queues.values.size(); ++i) {
			FREE (queues.values[i]);
		}

		for (s32 i = 0; i < constants.keys.size(); ++i) {
			FREE (constants.keys[i]);
		}

		for (s32 i = 0; i < commands.keys.size(); ++i) {
			FREE (commands.keys[i]);
		}

		for (s32 i = 0; i < queues.keys.size(); ++i) {
			FREE (queues.keys[i]);
		}

		for (u32 iFile = 0; iFile < files.size(); ++iFile) {
			IO::Close (files[iFile]);
		}

	}

	// We're using those 2 to form a string that later 
	// copied and pushed to a specific vector.
	u32 temporaryLength;
	u8 temporary[2048];

	// During 1st read when we possess all the files we need to offset
	//  projects by a number that represents previously added projects at lower depth
	//  so that we always check with newly added projects when looking for subprojects.
	u32 projectsOffset = 0;
	u32 projectId = -1;

	void SetFirstTemp (const c8& value) {
		temporary[0] = value;
		temporaryLength = 1;
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

	//// Cascading values require storing data
	u32 cascadingLength;
	//u8 cascading[256];

}