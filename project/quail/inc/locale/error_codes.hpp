// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>

namespace LOCALE {

	#define LOCALE_ERROR_NO_ACTIVITY 			"run `quail --help` to view the manual."
	#define LOCALE_ERROR_INTERPRETER_SYNTAX 	"Invalid syntax '%s' ['%c'-%d]" ERROR_NEW_LINE
	#define LOCALE_ERROR_RECURENT_PROJECT 		"Said project file is already being loaded: `%ls`" ERROR_NEW_LINE
	#define LOCALE_ERROR_CASC_CONSTANT			"Invalid cascaded constant: %s" ERROR_NEW_LINE
	#define LOCALE_ERROR_INVALID_PRE_ALIAS		"Could not match string with any project." ERROR_NEW_LINE
	#define LOCALE_ERROR_INVALID_ALIAS 			"Could not match string with a command nor a queue." ERROR_NEW_LINE
	#define LOCALE_ERROR_INVALID_ALIAS_IN_QUEUE	"Command '%s' inside Queue '%s' does not exist." ERROR_NEW_LINE

	#define LOCALE_ERROR_INVALID_SETTING_VALUE 	"Incorrect setting value: \"%s\" for key: \"%s\"." ERROR_NEW_LINE
	#define LOCALE_ERROR_INVALID_SETTING_NAME	"Incorrect setting name: \"%s\"." ERROR_NEW_LINE
	#define LOCALE_ERROR_INVALID_SETTING_QUEUE	"Missing string. Could be representing a setting key or a value." ERROR_NEW_LINE
	#define LOCALE_ERROR_NO_SETTING 			"Provide a valid setting name. See https://github.com/dotBlueShoes/Quail/wiki/Settings." ERROR_NEW_LINE


}
