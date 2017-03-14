#include "Editor.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

/**
	Checks if the file path specified is a valid Windows file path.
	@param filePath The path to be verified.
	@returns True if path is valid, false otherwise.
*/
bool isValidFileName(string filePath) {
	char fullDir[_MAX_DIR];
	char fileName[_MAX_FNAME];
	char fileExt[_MAX_EXT];

	_splitpath_s(filePath.c_str(), NULL, 0, fullDir, _MAX_DIR, fileName, _MAX_FNAME, fileExt, _MAX_EXT);
	stringstream ss;

	ss << fileName;
	string fileNameWithExt = ss.str();

	try {
		regex reservedNames("^(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:.[^.]*)?$");
		regex invalidChars("^.*?(?:[<>:\"/\\\\|?*]\\B|[<>:\"/\\\\|?*]\\b).*?$");
		regex invalidEnd("^.*[\\ \\.]$");

		if (regex_match(fileNameWithExt, reservedNames)
			|| regex_match(fileNameWithExt, invalidChars)
			|| regex_match(fileNameWithExt, invalidEnd)
			|| filePath.length() > _MAX_PATH
			|| filePath.length() == 0)
		{
			return false;
		}
	}
	catch (const regex_error &e) {
		cerr << e.what() << endl;
	}

	return true;
}

int main(int argc, char* argv[]) {

	// Check we're getting three parameters (counting the executable name)
	if (argc == 3) {
		if (!isValidFileName(argv[1]) 
			|| !isValidFileName(argv[2])) {
			cout << endl << "Invalid input or output file paths. Try again." << endl;
			return 0;
		}
	}
	else {
		cout << endl << " Insufficient parameters." << endl << endl;
		cout << " USAGE: " << endl << endl;
		cout << " \tEditor.exe [input file path] [output file path]" << endl;

		return 0;
	}

	Editor editor(argv[1], argv[2]);
	editor.displayBuffer();

	string cmd;

	do {
		getline(cin, cmd);
		editor.parseCommand(cmd);
	} while (!editor.shouldExit);

	_getch();

	return 0;
}