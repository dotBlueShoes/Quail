// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "error.hpp"

namespace IO {

	bool IsExisting (const c16* const& name) {
		struct _stat64i32 buffer;   
  		return (_wstat (name, &buffer) == 0); 
	}

	bool IsExisting (const c8* const& name) {
		struct stat buffer;   
  		return (stat (name, &buffer) == 0); 
	}

	void Create (
		const c16* const& pathname
	) {
		std::ofstream outfile (pathname);
		//outfile << "hello world" << std::endl;
		outfile.close ();
	}

	void CreateAdd (
		const c16* const& pathname,
		c16*& context
	) {
		std::wofstream outfile (pathname);
		//std::ofstream outfile (pathname);
		outfile << context << std::endl;
		outfile.close ();
	}

	void Read (
		const c16* const& pathname,
		FILE*& file
	) {
		file = _wfopen (pathname, L"rb");
		if (file == NULL) ERROR ("File: '%ls' could not be opened." ERROR_NEW_LINE, pathname);
	}

	void Close (
		FILE*& file
	) {
		fclose (file);
	}

}
