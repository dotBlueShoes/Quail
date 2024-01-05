#pragma once
#include "lib/Framework.hpp"

#include "Arguments.hpp"
#include "Validate.hpp"

#include "Commands/Help.hpp"
#include "Commands/Open/Open.hpp"


namespace Arguments::Parse {

	getter int32 CommandIncomplete(const charConsole* const commandName) {

		// GET COMMAND LENGTH
		uint8 commandLength = 0; 
		for (; commandName[commandLength] != '\0'; ++commandLength);

		// POSSIBLE SHORT COMMAND
		if (commandLength == COMMAND_SHORT_LENGTH) {

			switch (Validate::Short(commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Commands::Open::GetMainConfigData();
					Commands::Open::Parse::DisplayFiles();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					Commands::Help::Display();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					Commands::Open::GetMainConfigData();
					Commands::Open::Parse::DisplayFiles();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					Commands::Help::Display();
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}


	getter int32 CommandProjectOnly(
		const charConsole* const commandName, 
		const charConsole* const projectName
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
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {

					// THIS SUCKS !
					wchar filePath[256] { L'\0' };

					// LOAD DATA FROM MAIN CONFIG INTO BUFFOR
					Commands::Open::GetMainConfigData();
					// GET ASSOCIATED FILEPATH WITH PROJECT NAME
					Commands::Open::Projects::Validate(filePath, projectLength, projectName);

					//printf("\npath: %s\n", filePath);
					
					// LOAD DATA FROM PROJECT CONFIG INTO BUFFOR
					Commands::Open::GetConfigData(filePath);

				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		// POSIIBLE FULL COMMAND
		} else if (commandLength > COMMAND_SHORT_LENGTH && commandLength <= COMMAND_MAX_LENGTH) {

			switch (Validate::Full(commandLength, commandName)) {

				case MATCH::IMPOSIBLE: {
					printf("FAILRE: IMPOSIBLE_CASE?");
				} return ExitCode::FAILURE_UNKNOWN;

				case MATCH::OPEN: {
					//const wchar* const filePath = L"0";
					//Commands::Open::GetConfigData(filePath);
				} return ExitCode::SUCCESSFULL_COMMAND_EXECUTION;

				case MATCH::HELP: {
					printf("FAILURE: PASSED_UNNEEDED_ARGUMENT");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				case MATCH::INVALID: {
					printf("FAILURE: INVALID_COMMAND_SIGN");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

				default: {
					printf("FAILURE: INVALID_NO_DASH");
				} return ExitCode::FAILURE_INVALID_ARGUMENT;

			}

		} else {
			// INVALID COMMAND LENGTH.
			return ExitCode::FAILURE_ARGUMENT_COUNT;
		}

	}

}