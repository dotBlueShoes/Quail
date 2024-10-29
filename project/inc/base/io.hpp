// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//
#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream> 

namespace IO {

	void Create (
		const c16* const& pathname
	) {
		std::ofstream outfile (pathname);
		outfile << "my text here!" << std::endl;
		outfile.close();
	}

	void Read (
		const c16* const& pathname
	) {
		//std::ofstream myfile;
		//myfile.open (pathname, std::ios::out | std::ios::app | std::ios::binary);
	}

}
