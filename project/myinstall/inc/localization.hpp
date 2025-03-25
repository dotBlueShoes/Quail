// Created 2024-01-15 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <global/config.hpp>
#include <blue/types.hpp>

// TODO: CANCEL
// CANCEL CONFIRM MSG BOX
// Title as Window
// Install has not completed. Are you sure you want to exit?

namespace LOCAL {

	const c16 WINDOW_TITLE				[] { QUAIL_NAME_VERSION_W L" Setup Wizard" };
	// TEXTS
	const c16 EntryWelcome				[] = L"Welcome to " QUAIL_NAME_VERSION_W L" Setup Wizard";
	const c16 EntryText					[] = L"This wizard will guide you through the installation of\n" QUAIL_NAME_VERSION_W L".\n\nClick 'Next' to continue.";
	const c16 LicenseTop				[] = L"Scroll down to see the rest of the agreement.";
	const c16 LicenseBot				[] = L"If you accept the terms of the agreement, click I Agree to continue. You must accept\nthe agreement to install `" QUAIL_NAME_VERSION_W L"`. Scrollbar handle has to reach the end.";
	const c16 BrowseTip					[] = L"To continue, click Next. If you would like to select a different folder, click Browse.";
	const c16 RegistryTop				[] = L"Select the components you want to install clear the components you do not want to\ninstall. Click Next when you are ready to continue.";
	const c16 ConfirmationBot			[] = L"Click 'Start' to install '" QUAIL_NAME_VERSION_W L"'";
	const c16 ExitTag					[] = L"Completing the " QUAIL_NAME_VERSION_W L" Setup Wizard";
	const c16 ExitText					[] = L"Setup has finished installing " QUAIL_NAME_VERSION_W L" on your computer.";

	const c16 LVRegistry				[] = L"Create necessary " QUAIL_NAME_W L" Registry Keys (Personal configuration settings)";
	const c16 LVPath					[] = L"Add " QUAIL_NAME_W L" to 'Path' variable (Will make " QUAIL_NAME_W L" accessible from anywhere)";
	const c16 LVBatch					[] = L"Create an additional 'o.bat' file which is a short for 'Quail -o ...'";

	// TEXTS TAGS
	const c16 TagLicense				[] = L"License Agreement";
	const c16 TagDirectory				[] = L"Directory Selection";
	const c16 TagRegistry				[] = L"Select Components";
	const c16 TagConfirmation			[] = L"Confirmation";
	const c16 TagDownload				[] = L"Download";

	// TEXTS DESCRIPTIONS
	const c16 DescriptionLicense		[] = L"Please review the license terms before installing " QUAIL_NAME_VERSION L".";
	const c16 DescriptionDirectory		[] = L"Where should " QUAIL_NAME_VERSION L" software be installed?";
	const c16 DescriptionRegistry		[] = L"Which components should be installed?";
	const c16 DescriptionConfirmation	[] = L"You are now ready to install " QUAIL_NAME_VERSION L" software.";
	const c16 DescriptionDownload		[] = L"Please wait while Wizard installs " QUAIL_NAME_VERSION L" on your computer.";

	// TEXTS INSTALLER TAGS
	const c16 InstallerTagDownloading	[] = L"Downloading files . . .";
	const c16 InstallerTagExtracting	[] = L"Extracting files . . .";
	const c16 InstallerTagRegistry		[] = L"Creating registry keys . . .";
	const c16 InstallerTagFiles			[] = L"Creating files . . .";

	// TEXTS BUTTONS
	const c16 BUTTON_BROWSE				[] = L"Browse...";
	const c16 BUTTON_AGREE				[] = L"I Agree";
	const c16 BUTTON_FINISH				[] = L"Finish";
	const c16 BUTTON_CANCEL				[] = L"Cancel";
	const c16 BUTTON_LAST				[] = L"< Last";
	const c16 BUTTON_NEXT				[] = L"Next >";
	const c16 BUTTON_START				[] = L"Start";

	const c16 CONFIRMATION_REGISTRY		[] = L"- Registry keys will be added.";
	const c16 CONFIRMATION_PATH			[] = L"- New entry in Environment Variable 'Path' will be added.";
	const c16 CONFIRMATION_BATCH		[] = L"- Batch short will be created.";

	const c8 INSTALL_TASK_REGISTRY		[] = "Adding registry keys";
	const c8 INSTALL_TASK_PATH			[] = "Adding " QUAIL_NAME_VERSION " to path environment variable";
	const c8 INSTALL_TASK_FILES			[] = "Creating necessary " QUAIL_NAME_VERSION " files";

	const u8 MAX_EXE_SIZE = 20;
	
	#define CONFIRMATION_TOP_1 	L"This program will install " // 27
	#define CONFIRMATION_TOP_2 	L" into: \"" // 9
	#define DISK_SPACE_1		L"Disk space needed : " // 21

	const u8 CONFIRMATION_TOP_1_SIZE 	= sizeof (CONFIRMATION_TOP_1) / sizeof (c16) - 1; // minus EOS sign.
	const u8 CONFIRMATION_TOP_2_SIZE 	= sizeof (CONFIRMATION_TOP_2) / sizeof (c16) - 1; // minus EOS sign.
	const u8 DISK_SPACE_1_SIZE 			= sizeof (DISK_SPACE_1) / sizeof (c16) - 1; // minus EOS sign.

	// DYNAMIC TEXTS
	c16 confirmationTop	[CONFIRMATION_TOP_1_SIZE + CONFIG::QUAIL_NAME_VERSION_SIZE + CONFIRMATION_TOP_2_SIZE + MAX_PATH] = CONFIRMATION_TOP_1 QUAIL_NAME_VERSION_W CONFIRMATION_TOP_2;
	c16 diskSpace		[DISK_SPACE_1_SIZE + LOCAL::MAX_EXE_SIZE] = DISK_SPACE_1;
}
