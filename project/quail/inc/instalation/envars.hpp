// Created 2024.11.17 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/log.hpp>
#include <blue/memory.hpp>
#include <blue/error.hpp>

namespace ENVARS {

	// This approach makes an environment variable that will not persist after the execution!
	// 1. use this instead: 
	// https://stackoverflow.com/questions/5246046/how-to-add-environment-variable-in-c
	// 2. also to make non persistent variables using this method would be better:
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setenvironmentvariable.

	void AddQuailToPath (
		const u16& quailLength,
		const c16* const& quail
	) {
		const c16 header[] = L"PATH=";

		c16* env; ALLOCATE (c16, env, 2048);

		memcpy (env, header, 12);
		wcscat (env, _wgetenv (L"PATH"));
		wcscat (env, L";");

		auto envLength = wcslen (env);
		auto&& envPoint = env + envLength;

		memcpy (envPoint, quail, quailLength);

		envPoint = envPoint + ((quailLength - 1) / 2);
		envPoint[0] = L'\0';

		//LOGWINFO ("%s\n", env);

		u32 error = _wputenv (env);

		if (error) {
			ERROR ("Could not add `Quail` to the environment var: `path`.\n");
		} else {
			LOGINFO ("Successfully added `Quail` to the environment variable `path`.\n");
		}

		FREE (env);

	}

}
