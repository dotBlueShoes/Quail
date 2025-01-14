// Created 2024.01.14 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#include <blue/types.hpp>
#include <blue/log.hpp>

#include <global/registry.hpp>

int WinMain (
	HINSTANCE	instance,
	HINSTANCE	previousInstance,
	LPSTR		commandline,
	s32			isConsole
) {

	UNREFERENCED_PARAMETER (previousInstance);
	UNREFERENCED_PARAMETER (commandline);
    UNREFERENCED_PARAMETER (isConsole);

	LOGINFO ("Application Statred!\n");
	WINDOWS::REGISTRY::Sample();

	MessageBoxW (nullptr, L"Uninstallation was Succesfull", L"Quail Uninstallation", MB_OK);

	return 0;
}