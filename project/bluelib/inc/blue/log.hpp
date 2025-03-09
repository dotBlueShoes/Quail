// Created 2024.10.28 by Matthew Strumiłło (dotBlueShoes)
//  LICENSE: GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
//
#pragma once
#include "types.hpp"
#include "io_types.hpp"
#include "debug.hpp"

#include "windows/types.hpp"

#ifndef ERROR_NEW_LINE
	#define ERROR_NEW_LINE "\n"
#endif

#ifdef DEBUG_FLAG_CLOCKS

	#include "timestamp.hpp"

	TIMESTAMP::Timestamp TIMESTAMP_BEGIN;

	#ifndef LOGGER_TIME_FORMAT
		#define LOGGER_TIME_FORMAT "%2.2f"
	#endif

	#ifdef CONSOLE_COLOR_ENABLED

		#include <windows.h>

		#define CNS_CLR_INF 2 	
		#define CNS_CLR_WAR 6 	
		#define CNS_CLR_ERR 4 	
		#define CNS_CLR_DEF 7 

		#define LOGINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				fprintf (stdout, "[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_INF); \
				fprintf (stdout, " INFO"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			} \
		}

		#define LOGWINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				fwprintf (stdout, L"[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_INF); \
				fwprintf (stdout, L" INFO"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			} \
		}

		#define LOGWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				fprintf (stdout, "[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_WAR); \
				fprintf (stdout, " WARN"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			} \
		}

		#define LOGWWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				fwprintf (stdout, L"[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_WAR); \
				fwprintf (stdout, L" WARN"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			} \
		}

		#ifndef LOGERROR
			#define LOGERROR(...) { \
				fprintf (stdout, "[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
				fprintf (stdout, " ERRR"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			}
		#endif

		#ifndef LOGWERROR
			#define LOGWERROR(...) { \
				fwprintf (stdout, L"[" LOGGER_TIME_FORMAT "]", TIMESTAMP::GetElapsed (TIMESTAMP_BEGIN)); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
				fwprintf (stdout, L" ERRR"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			}
		#endif

	#else

		#define LOGINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			printf ("INFO: " __VA_ARGS__); \
		}

		#define LOGWINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			wprintf (L"INFO: " __VA_ARGS__); \
		}

		#define LOGWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			printf ("WARN: " __VA_ARGS__); \
		}

		#define LOGWWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			wprintf (L"WARN: " __VA_ARGS__); \
		}

		#ifndef LOGERROR
			#define LOGERROR(...) { \
				printf ("ERRR: " __VA_ARGS__); \
			}
		#endif

		#ifndef LOGWERROR
			#define LOGWERROR(...) { \
				wprintf (L"ERRR: " __VA_ARGS__); \
			}
		#endif

	#endif

	#define MSGINFO(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "INFO", MB_OK); \
	}

	#define MSGWARN(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "WARN", MB_OK); \
	}

	#define MSGERROR(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "ERROR", MB_OK); \
	}

#else

	#ifdef CONSOLE_COLOR_ENABLED
	
		#include <windows.h>
	
		#define CNS_CLR_INF 2 	
		#define CNS_CLR_WAR 6 	
		#define CNS_CLR_ERR 4 	
		#define CNS_CLR_DEF 7 
	
		#define LOGINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_INF); \
				fprintf (stdout, "INFO"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			} \
		}
	
		#define LOGWINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_INF); \
				fwprintf (stdout, L"INFO"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			} \
		}
	
		#define LOGWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_WAR); \
				fprintf (stdout, "WARN"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			} \
		}
	
		#define LOGWWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_WAR); \
				fwprintf (stdout, L"WARN"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			} \
		}
	
		#ifndef LOGERROR
			#define LOGERROR(...) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
				fprintf (stdout, "ERRR"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fprintf (stdout, ": " __VA_ARGS__); \
			}
		#endif
	
		#ifndef LOGWERROR
			#define LOGWERROR(...) { \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_ERR); \
				fwprintf (stdout, L"ERRR"); \
				SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), CNS_CLR_DEF); \
				fwprintf (stdout, L": " __VA_ARGS__); \
			}
		#endif
	
	#else
	
		#define LOGINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			printf ("INFO: " __VA_ARGS__); \
		}
	
		#define LOGWINFO(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			wprintf (L"INFO: " __VA_ARGS__); \
		}
	
		#define LOGWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			printf ("WARN: " __VA_ARGS__); \
		}
	
		#define LOGWWARN(...) { \
			DEBUG (DEBUG_FLAG_LOGGING) \
			wprintf (L"WARN: " __VA_ARGS__); \
		}
	
		#ifndef LOGERROR
			#define LOGERROR(...) { \
				printf ("ERRR: " __VA_ARGS__); \
			}
		#endif
	
		#ifndef LOGWERROR
			#define LOGWERROR(...) { \
				wprintf (L"ERRR: " __VA_ARGS__); \
			}
		#endif
	
	#endif
	
	#define MSGINFO(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "INFO", MB_OK); \
	}
	
	#define MSGWARN(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "WARN", MB_OK); \
	}
	
	#define MSGERROR(message) { \
		DEBUG (DEBUG_FLAG_LOGGING) \
		MessageBoxA (NULL, message, "ERROR", MB_OK); \
	}

#endif
