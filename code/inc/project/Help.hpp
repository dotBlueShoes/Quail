#pragma once
#include "../Framework.hpp"

#include "../Tokens.hpp"

namespace Project::Help {


	callback Action( Tokens::ActionArgs& ) {
        DEBUG std::cout << "Entered action 'Help'\n";
    }
	
	block Display() {
		
		// TODO
		//  increase buffor ...
		//  display desc.
		//  verbose option ?
		//  full command show ?
		
		char buffor[256] { 0 };
		uint8 bufforIndex = 0;
		
		const char* t1 = "\nHELP PAGE:";
		const char* t2 = "\n ";
		const char* t3 = "desc.";
		
		// Insert 1st text
		for (uint8 i = 0; t1[i] != '\0'; ++i) {
			buffor[i] = t1[i];
		}
		
		// Move the index.
		bufforIndex += 11;
		
		// Insert Commands and their pre texts and after texts.
		for (uint8 j = 0; j < Tokens::shorts.Length(); ++j) {
			
			// pre text
			buffor[bufforIndex + 0] = t2[0];
			buffor[bufforIndex + 1] = t2[1];
			bufforIndex += 2;
			
			// command
			for (uint8 i = 0; i < Tokens::shorts[j].Length(); ++i) {
				buffor[bufforIndex + i] = Tokens::shorts[j][i];
			} bufforIndex += Tokens::shorts[j].Length();
			
			// after text
			buffor[bufforIndex + 0] = ' ';
			buffor[bufforIndex + 1] = t3[0];
			buffor[bufforIndex + 2] = t3[1];
			buffor[bufforIndex + 3] = t3[2];
			buffor[bufforIndex + 4] = t3[3];
			buffor[bufforIndex + 5] = t3[4];
			bufforIndex += 6;
		}
		
		buffor[bufforIndex] = '\0';
		
		std::cout << buffor << '\n' << std::endl;
		
		//Tokens::commands
	}
	
}