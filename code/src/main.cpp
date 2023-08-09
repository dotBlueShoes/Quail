#include "Framework.hpp"

#include "project/Help.hpp"
#include "Validation.hpp"

// 1. re-create tools.npp file so it will contain all the files there are in those folders.
// 2. setup script that will
//  - ask the user for:
//    - project name
//    - build name
//    - libs
//  - will setup project path

// Error checking
// output
// If in file defined variables would be always in the same order 
//  the parse function could work as more then a 1 function that would be 
//  changed constanly in a loop for functions to look for other tokens. 
//  Also yeah that can be easier implemented... 

string parseSecret(string line, const string projectPath) {
	const string projectPathToken { "projectPath" };
	bool isToken { false };
	size_t i = 0;
	
	for (; i < projectPathToken.length(); i++) {
		if (line[i] == projectPathToken[i]) isToken = true;
		else { 
			isToken = false; 
			break; 
		}
	}
	
	if (isToken) {
		string temp;
		temp += projectPathToken;
		temp +=	" = ";
		temp += projectPath;
		temp += "\n";
		return temp;
	}
	
	line += '\n';
	return line ;
}

int setup(const string projectPath) {
	const string originPath { ".ninja\\secret.ninja" },
		resultPath { ".ninja\\_secret.ninja" };
	
	vector<string> data;
	string line;
	
	std::cout << projectPath << '\n';

	ifstream originFile(originPath);
	ofstream resultFile(resultPath);
	
	if (originFile.is_open() && resultFile.is_open()) {
		
		// READ
		while (getline(originFile, line))
			data.push_back(line);
		originFile.close();
		
		// WRITE
		for (size i = 0; i < data.size(); i++)
			resultFile << parseSecret(data[i], projectPath);
		resultFile.close();
		
		// REPLACE (Is this the safest way?)
		if (filesystem::remove(originPath)) {
			filesystem::rename(resultPath, originPath);
			return 0;
		}
		
	} else 
		std::cerr << "Error: Opening or creating the file.\n";
	
	std::cerr << "Error: Removing old file.\n";
	return 0;
}

// Specific to windows unicode main function.
// int wmain(int argumentsLength, wchar_t* arguments[])

// ??
// Am I chaining commands here or am I telling it to do 1 specific job ?
// the 2 one... so why do I care so much about the random order .......
// 
// 1 command is crusial
// other that follow are not.
// and should be random
// simply enumerate on thoose as theres no need for more.

// ASCII
// 1. 128 - allowed characters total
// 2. sorted / not sorted / half sorted
// 3. 

// Algorithm
// 1. check argumentsLength (is 1 - none) is (1 + 2 - proper use) [as theres always command followed by its set value]
// 2. 
// 1. Chceck if param's 2 sign is "-" therefore we know we're dealing with long form.
	
int32 main (
	const int32 argumentsLength, 
	const char** arguments
) {
	
	DEBUG std::cout << "Entered program in DEBUG mode!\n";
	
	//DEBUG {
	//	using Type = Project::Open::SubCommand;
	//	const array<Type, 4> elements {
	//		Type { { "lll" }, { "jjj" } },
	//		Type { { "aaa" }, { "jjj" } },
	//		Type { { "ddd" }, { "jjj" } },
	//		Type { { "ccc" }, { "jjj" } }
	//	};
	//
	//	const uint8 valuesLength = 3;
	//	const char values[] = "aaa";
	//
	//	//auto pointerToStructure = (void*)(&elements[0]);
	//	//auto pointerInside = (char*)(pointerToStructure);
	//
	//	auto& value = ((char*)((byte*)(void*)(&elements[0]) + 8))[0];
	//
	//	auto datatype1 = (byte*)(void*)(&elements[0]) + 8; /* + 16 + 8 */
	//	auto datatype2 = elements[0].name.Pointer();
	//	auto datatype3 = elements[0].context.Pointer();
	//	printf("\n%p\n", (void*)(datatype1));
	//	printf("\n%p\n", (void*)(datatype2));
	//	printf("\n%p\n", (void*)(datatype3));
	//
	//	printf("\n%c\n", ((char*)(datatype1))[1]);
	//}
	

	// 0 arg - whereami
	// 1 arg - main command
	// 2 arg - main command param
	if (argumentsLength > 2) {
		Tokens::Actions::Initialize();
		uint8 state = Tokens::Validation::Main(arguments);

		if (state == Tokens::Validation::success) {
			Tokens::ActionArgs actionArguments { argumentsLength, arguments };
			Tokens::Actions::actions[0]( actionArguments );
		}

		Tokens::Actions::Destroy();
	} else {
		Project::Help::Display();
	}
	
	// Parsing
	//  Type 1. Parse string by string. (enumarate inside)
	//  Type 2. Look for tokens in string instead. (enumerate inside)
	//  Type 3. Parsing for big files. If file data is being structuried into lets say for example code funtions.
	//   Then we can use found function declaration to later look for inner statements (tokens).
	
	//for (uint32 i = 0; arguments[1][i] != '\0'; ++i)
	//	if (arguments[1][i] != tokens::setupFull[i]) {
	//		isEqual = false;
	//		break;
	//	}
	//	
	//if (isEqual) { setup(arguments[2]); return 0;}
	//isEqual = true;
	//	
	//	
	//for (uint32 i = 0; arguments[1][i] != '\0'; ++i)
	//	if (arguments[1][i] != tokens::setupShort[i]) {
	//		isEqual = false;
	//		break;
	//	}
	//	
	//if (isEqual) { setup(arguments[2]); return 0; }
	//isEqual = true;
	//
	//
	//for (uint32 i = 0; arguments[1][i] != '\0'; ++i)
	//	if (arguments[1][i] != tokens::reprojectFull[i]) {
	//		isEqual = false;
	//		break;
	//	}
	//	
	//if (isEqual) { 
	//	const char* projects[] { arguments[3], arguments[4] }; 
	//	NppProcessor::refreshNpp(arguments[2], argumentsLength - 3, projects); 
	//	return 0;
	//} isEqual = true;
	//	
	//	
	//for (uint32 i = 0; arguments[1][i] != '\0'; ++i)
	//	if (arguments[1][i] != tokens::reprojectShort[i]) {
	//		isEqual = false;
	//		break;
	//	}
	//	
	//if (isEqual) { 
	//	const char* projects[] { arguments[3], arguments[4] }; 
	//	{ using namespace NppProcessor;
	//		refreshNpp(arguments[2], argumentsLength - 3, projects); 
	//	}
	//	return 0;
	//}
	
	
	//std::cin.get();
	return 0;
}