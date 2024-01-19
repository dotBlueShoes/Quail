#pragma once
#include "lib/Framework.hpp"

#include "Arguments.hpp"
#include "Validate.hpp"

#include "Commands/Help.hpp"
#include "Commands/Open/Open.hpp"

namespace Arguments::Parse::Cmmds {

	block ViewMainConfig () {
		Commands::Open::GetMainConfigData ();
		Commands::Open::Parse::DisplayFiles ();
	}


	block ViewProjectConfig (
		IN 						const uint8& 				projectLength, 
		INREADS (projectLength) const charConsole* const 	projectName
	) {

		// THIS SUCKS !
		wchar filePath[512] { L'\0' };
		//uint8 filePathCount[4] { 0 };

		wchar other1[256] { LR"(D:\Storage\Projects\Quail\.quail\properties.txt)" }; other1[48] = 0;
		wchar other2[256] { LR"(D:\Storage\Projects\Quail\.quail\extra.txt)" }; other2[43] = 0;

		{ // Get PROJECT from main config.
			// LOAD DATA FROM MAIN CONFIG INTO BUFFOR
			Commands::Open::GetMainConfigData();
			// GET ASSOCIATED FILEPATH WITH PROJECT NAME
			Commands::Open::Projects::ValidateProjectArgument(filePath, projectLength, projectName);
			//printf("here1");
			// READ DATA FROM PROJECT CONFIG INTO BUFFOR
			Commands::Open::GetProjectConfigData(filePath);
		}

		{ // Get IMPORTS from Project
			// LOAD DATA FROM NESTED IMPORT
			Commands::Open::GetConfigData(other1);
			Commands::Open::GetConfigData(other2);
		}
					
		// Display commands and queues we got.
		//printf("here2");
		Commands::Open::Parse::DisplayProject();
		//printf("here3");
	}


	block RunSubcommand (
		IN 						const uint8& 				projectLength, 
		INREADS (projectLength) const charConsole* const 	projectName,
		IN 						const uint8& 				subcmmdLength, 
		INREADS (projectLength) const charConsole* const 	subcmmdName
	) {

		// THIS SUCKS !
		wchar filePath[512] { L'\0' };
		//uint8 filePathCount[4] { 0 };

		wchar other1[256] { LR"(D:\Storage\Projects\Quail\.quail\properties.txt)" }; other1[48] = 0;
		wchar other2[256] { LR"(D:\Storage\Projects\Quail\.quail\extra.txt)" }; other2[43] = 0;

		{ // Get PROJECT from main config.
			// LOAD DATA FROM MAIN CONFIG INTO BUFFOR
			Commands::Open::GetMainConfigData();
			// GET ASSOCIATED FILEPATH WITH PROJECT NAME
			Commands::Open::Projects::ValidateProjectArgument(filePath, projectLength, projectName);
			// READ DATA FROM PROJECT CONFIG INTO BUFFOR
			Commands::Open::GetProjectConfigData(filePath);
		}

		{ // Get IMPORTS from Project
			// LOAD DATA FROM NESTED IMPORT
			Commands::Open::GetConfigData(other1);
			Commands::Open::GetConfigData(other2);
		}
					
		Commands::Open::Parse::ExecuteSubcommand(subcmmdLength, subcmmdName);
	}

}

namespace Arguments::Parse {



	getter int32 CommandIncomplete(
		IN const charConsole* const commandName
	) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (Validate::Short(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::ViewMainConfig ();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					Commands::Help::Display ();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::ViewMainConfig ();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					Commands::Help::Display ();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}



	getter int32 CommandProjectOnly(
		IN const charConsole* const commandName, 
		IN const charConsole* const projectName
	) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// GET PROJECT LENGTH
		uint8 projectLength = 0; 
		for (; projectName[projectLength] != '\0'; ++projectLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (Validate::Short(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::ViewProjectConfig (projectLength, projectName);
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf ("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::ViewProjectConfig (projectLength, projectName);
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf ("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}



	getter int32 Command(
		IN const charConsole* const commandName,
		IN const charConsole* const projectName,
		IN const charConsole* const subcmmdName
	) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// GET PROJECT LENGTH
		uint8 projectLength = 0; 
		for (; projectName[projectLength] != '\0'; ++projectLength);

		// GET SUBCOMMAND LENGTH
		uint8 subcmmdLength = 0; 
		for (; subcmmdName[subcmmdLength] != '\0'; ++subcmmdLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (Validate::Short(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::RunSubcommand (projectLength, projectName, subcmmdLength, subcmmdName);
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf ("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf ("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Cmmds::RunSubcommand (projectLength, projectName, subcmmdLength, subcmmdName);
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf ("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf ("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf ("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}


}