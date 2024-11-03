// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include "log.hpp"

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
		outfile << "my text here!" << std::endl;
		outfile.close();
	}

	void Read (
		const c16* const& pathname,
		FILE*& file
	) {
		file = _wfopen(pathname, L"rb");
		if (file == NULL) ERROR ("ERROR! File could not be opened file");
	}

	void Close (
		FILE*& file
	) {
		fclose (file);
	}

}
