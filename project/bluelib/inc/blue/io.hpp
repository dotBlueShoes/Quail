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

	void CreateEmpty (
		IN		const c16* const& pathname
	) {
		std::wofstream outfile (pathname);
		outfile.close ();
	}

	void CreateEmpty (
		IN		const c8* const& pathname
	) {
		std::ofstream outfile (pathname);
		outfile.close ();
	}

	void CreateAdd (
		IN		const c16* const& pathname,
		IN		const c16* const& context
	) {
		std::wofstream outfile (pathname);
		outfile << context << std::endl;
		outfile.close ();
	}

	void CreateAdd (
		IN		const c8* const& pathname,
		IN		const c8* const& context
	) {
		std::ofstream outfile (pathname);
		outfile << context << std::endl;
		outfile.close ();
	}

	void Read (
		IN		const c16* const& 	pathname,
		OUT		FILE*& 				file
	) {
		file = _wfopen (pathname, L"rb");
		if (file == nullptr) ERROR ("File could not be opened - '%ls'." ERROR_NEW_LINE, pathname);
	}

	void Close (
		INOUT	FILE*& file
	) {
		fclose (file);
	}

}
