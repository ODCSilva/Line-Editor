#ifndef EDITOR_H
#define EDITOR_H

#include "StringLinkedList.h"
#include "ConsoleUI.h"
#include <string>
#include <thread>

using namespace std;

const string DELETE_REGEX = "^[Dd]\\s?([0-9]*)\\s?([0-9]*)$";
const string GOTO_REGEX = "^[Gg]\\s?([0-9]*)$";
const string HELP_REGEX = "^[Hh]$";
const string INSERT_REGEX = "^[Ii]\\s?([0-9]*)$";
const string LIST_REGEX = "^[Ll]\\s?([0-9]*)\\s?([0-9]*)$";
const string POSITION_REGEX = "^[Pp]\\s?([0-9]*)$";
const string QUIT_REGEX = "^[Qq]$";
const string SAVE_EXIT_REGEX = "^[Ee]$";
const string SUB_REGEX = "^[Ss]\\s?([0-9]*)$";
const string VIEW_REGEX = "^[Vv]$";

class Editor
{
private:
	ConsoleUI console;
	StringLinkedList linkedList;
	int currentLine = 1;
	string inPath;
	string outPath;

public:
	bool shouldExit = false;
	Editor(string inPath, string outPath);

	void deleteLine(int line = -1);
	void deleteRange(int from, int to);
	void displayBuffer();
	void displayHelpInfo();
	void exit();
	void goToLine(int line = 1);
	void insertBeforeCurrentLine();
	void insertLine(int);
	void list();
	void list(int from, int to);
	void list(int line);
	void openDocument(string path);
	void parseCommand(string command);
	void saveDocument(string path);
	void scrollToCurrent();
	void scrollToPosition(int pos);
	void substituteCurrentLine();
	void substituteLine(int line = 0);
};

#endif
