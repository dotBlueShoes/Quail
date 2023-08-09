#pragma once
#include "Framework.hpp"

namespace NppProcessor {
	
	namespace tokens {
		const string 
			include { R"(inc)" }, 
			source { R"(src)" },
			build { R"(bld)" },
			ninja { R"(.ninja)" };
	}
	
	void recursiveFolderParse (
		ofstream& resultFile, 
		string buffor, 
		string indentation
	) {
		const filesystem::path path { buffor };
		for (const auto& entry : filesystem::directory_iterator(path)) {
			if (entry.is_directory()) {
				resultFile << indentation << "<Folder name=\"" << entry.path().filename().string() << "\">\n"; 
				recursiveFolderParse(resultFile, entry.path().string(), indentation + '\t');
				resultFile << indentation << "</Folder>\n";
			} else 
				resultFile << indentation << "<File name=\"" << entry.path().string() << "\" />\n";
		}
	}
	
	int refreshNpp (
		string projectFilePath, 
		const size_t length, 
		const char** projects
	) {

		const string originPath = projectFilePath,
			resultPath = projectFilePath + ".temp";
	
		//vector<string> data;
		string buffor;

		//ifstream originFile(originPath);
		ofstream resultFile(resultPath);
			
		buffor.reserve(_MAX_PATH);
	
		// READ
		//if (originFile.is_open() && resultFile.is_open()) {
		//	while (getline(originFile, line))
		//		data.push_back(line);
		//	originFile.close();
		//} else std::cerr << "Error: Opening or creating the file.\n";
		// Get the first line in.
		//resultFile << data[0] << '\n';
		
		resultFile << "<NotepadPlus>\n";
			
		// WRITE
		for (size_t i = 0; i < length; i++) {
				
			// Get projectName.
			const string project = projects[i];
			const size_t projectNamePosition = project.rfind('\\') + 1;
			const string projectName = project.substr(projectNamePosition);
				
			// Get the project line in.
			resultFile << "\t<Project name=\"" << projectName << "\">\n";
			
			// BUILD
			resultFile << "\t\t<Folder name=\"" << tokens::build << "\">\n";
			
			{
				const filesystem::path path { projects[i] };
				string indentation = "\t\t\t";
				
				buffor.append(path.parent_path().string());
				
				resultFile << indentation << "<File name=\"" << buffor << "\\build.ninja" << "\" />\n";
				
				buffor.append("\\");
				buffor.append(tokens::ninja);
			
				recursiveFolderParse(resultFile, buffor, indentation);
			}
			
			buffor.clear();
			
			// INCLUDE
			resultFile << "\t\t</Folder>\n\t\t<Folder name=\"" << tokens::include << "\">\n";
			buffor.append(projects[i]);
			buffor.append("\\");
			buffor.append(tokens::include);
				
			{
				string indentation = "\t\t\t";
				recursiveFolderParse(resultFile, buffor, indentation);
			}
				
			buffor.clear();
			
			// SOURCE
			resultFile << "\t\t</Folder>\n\t\t<Folder name=\"" << tokens::source << "\">\n";
			buffor.append(projects[i]);
			buffor.append("\\");
			buffor.append(tokens::source);
				
			{
				string indentation = "\t\t\t";
				recursiveFolderParse(resultFile, buffor, indentation);
			}
				
			buffor.clear();
			
			resultFile << "\t\t</Folder>\n\t</Project>\n";
				
		}
			
		resultFile << "</NotepadPlus>\n";
		resultFile.close();	
			
		// REPLACE
		// Should do a check here.
		filesystem::remove(originPath);
		filesystem::rename(resultPath, originPath);
	
		return 0;
	}
	
}


// Look for ?? - nie ma co szukać idziemy liniowo.
		
// ZAKŁADAMY ŻE DO TEJ FUNKCJI ARGUMENTEM MOŻE BYĆ CAŁA ŚCIEŻKA
// A JEŻELI SAMA NAZWA. eg. "C://sample//sampleLib"
// WTEDY TRZEBA WYODRĘBNIĆ CIĄG PO OSTATNIM //.
		
// 1 linijka pomijamy
// 2 linijka "Project name=" "x"
//  sprawdzamy czy x jest jedną z projects
//  jeśli tak to 
//  nie wywalamy błędu
//  kolejne liniki aż do </Folder>
//   na nowo tworzymy
//   "<File name=" + "projectPath" + "src" + "kolejny_plik"
//   "<File name=" + "projectPath" + "inc" + "kolejny_plik"

//std::cout << projectFilePath << '\n';
//string sample = "abc\\ab";
//string processed(std::find(sample.begin(), sample.end(), '\\') + 1, sample.end());
		
//auto iterator = std::find(sample.begin(), sample.end(), '\\') + 1;
//iterator = std::find(iterator, sample.end(), '\\') + 1;
//if (iterator != sample.end() + 1) {
//	std::cout << "lol" << '\n';
//	string processed(iterator, sample.end());
//	std::cout << processed << '\n';
//}
		
// Get the relativePath of the .npp file.
//size_t fileSeparatorPosition = projects[0].rfind('\\') + 1;
//string projectPath = projects[0].substr(0, fileSeparatorPosition);
//string projectName = projects[0].substr(fileSeparatorPosition);