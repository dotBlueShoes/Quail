// Created 2025.03.12 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include <blue/types.hpp>
#include <blue/io.hpp>

namespace OPEN::DISPLAY {

	//  TODO
	// Theres prob. a different default color within different console settings.
	//
	const u8 COLOR_DEFAULT = 7;
	const u8 COLOR_WHITE = 15;

	//  ABOUT
	// Writes a line to better visualize what command in queue is being 
	// executed at a certain time.
	//
	void QueueProgress (
		const HANDLE& console,
		const c8* const& key,
		const c8& symbol,
		const u8& color
	) {
		const c8 LP[] = "    ";

		fwrite (LP, 	 sizeof (c8),	strlen (LP),	stdout);
		printf ("...executing [%c", symbol);

		SetConsoleTextAttribute (console, color);
		fwrite (key, 	 sizeof (c8), 	strlen (key), 	stdout);
		SetConsoleTextAttribute (console, COLOR_DEFAULT);

		fwrite ("]\n", 	sizeof (c8), 	2, 				stdout);

	}


	//  ABOUT
	// Writes a line representing a key-value specified in a customized 
	// manner with an applied color.
	//
	template <typename keyT, typename valueT>
	void KeyValue (
		IN  const HANDLE& 			console,
		IN  const keyT* const& 		key,
		IN  const u16& 				keySize,
		IN  const valueT* const& 	value,
		IN  u16 					valueSize,
		IN  const c8& 				symbol,
		IN  const u8& 				color
	) {

		const c8 LP[] = "    "; // Padding of a Line.
		const c8 VP[] { 		// Padding of a Value. 
			' ', ' ', ' ', ' ', 
			' ', ' ', ' ', ' ', 
			' ', ' ', ' ', ' ', 
			' ', ' ', ' ', ' ', 
			' ', ' ', ' ', ' ' 
		};

		const u16 LINE_SIZE = 120 - 1;
		const u16 NAME_SIZE = 18;
		const u16 DOTS_SIZE = 3;

		//  LOGIC
		// If the given key (name) is longer than 'NAME_SIZE' make it so 
		//  no space-padding characters are displayed. Otherwise, calculate 
		//  the amount of space characters needed to properly format text.
		// 

		s16 vps = NAME_SIZE - keySize; BRANCHLESS vps *= (vps > 0);

		//  LOGIC
		// Calculate if the given key, value fits in the line. If it 
		//  doesn't, then trim the value and give space for '...' characters.
		// 

		s16 lps = LINE_SIZE - ( strlen (LP) + 1 + keySize + vps + 2 + DOTS_SIZE);
		s16 lineEndSize = lps - valueSize;

		BRANCH if (lineEndSize < 0) { 
			valueSize = lps; lineEndSize = DOTS_SIZE;
		} else { 
			lineEndSize = 0; 
		}
		
		{ // Actual WRITE
			fwrite (LP, 	sizeof (c8),		strlen (LP),	stdout);
			putc   (symbol, stdout);

			SetConsoleTextAttribute (console, color);
			fwrite (key, 	sizeof (keyT), 		keySize, 		stdout);
			SetConsoleTextAttribute (console, COLOR_WHITE);

			fwrite (VP, 	sizeof (c8), 		vps, 			stdout);
			fwrite (": ", 	sizeof (c8), 		2, 				stdout);
			fwrite (value, 	sizeof (valueT), 	valueSize, 		stdout);
			fwrite ("...", 	sizeof (c8), 		lineEndSize, 	stdout);
			putc   ('\n', stdout);
		}
	}

}
