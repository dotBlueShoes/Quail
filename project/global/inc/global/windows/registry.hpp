// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>

namespace WINDOWS::REGISTRY {

	const c16 KEY_PATH_W							[] = L"SOFTWARE\\dotBlueShoes\\Quail";
	const c16 PROPERTY_QUAIL_FILEPATH_W				[] = L"filepath";
	const c16 VALUE_DEFAULT_QUAIL_FOLDER_W			[] = L"C:\\Program Files\\dotBlueShoes\\Quail";

	const c16 CONFIG_MAIN_W							[] = L"config.txt";
	const u32 CONFIG_MAIN_LENGTH					= sizeof (CONFIG_MAIN_W);

	const c16 CONFIG_GLOBAL_W						[] = L"global.txt";
	const u32 CONFIG_GLOBAL_LENGTH					= sizeof (CONFIG_GLOBAL_W);

}
