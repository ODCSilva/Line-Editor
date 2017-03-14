#include "Editor.h"
#include "ConsoleUI.h"
#include <fstream>
#include <regex>
#include <Windows.h>
#include <sstream>
#include <chrono>
#include <cstdio>

using namespace std;

/**
	Main constructor.
	@param inPath The path of the file to be loaded.
	@param outPath The path of the file to output the changes to.
*/
Editor::Editor(std::string inPath, std::string outPath) {
	this->inPath = inPath;
	this->outPath = outPath;

	console.setHeaderInfo(inPath);
	console.setFooterInfo(outPath);
	openDocument(inPath);
}

/**
	Parses incoming commands. If the command is valid, the associated method is called.
	@param command The command string to parse.
*/
void Editor::parseCommand(const std::string command) {
	smatch match;

	// DELETE command (D)
	if (regex_search(command, match, regex(DELETE_REGEX))) {
		string paramA = match[1].str();
		string paramB = match[2].str();

		if (paramA.empty() && paramB.empty()) {
			deleteLine();
		}
		else if (paramB.empty()) {
			deleteLine(stoi(paramA));
		}
		else {
			deleteRange(stoi(paramA), stoi(paramB));
		}
	}

	// VIEW command (V)
	else if (regex_match(command, regex(VIEW_REGEX))) {
		displayBuffer();
	}

	// INSERT command (I)
	else if (regex_search(command, match, regex(INSERT_REGEX))) {
		string param = match[1].str();
		if (!param.empty()) {
			insertLine(std::stoi(param));
		}
		else {
			insertBeforeCurrentLine();
		}
	}

	// GOTO command (G)
	else if (regex_search(command, match, regex(GOTO_REGEX))) {
		string param = match[1].str();
		if (!param.empty()) {
			goToLine(std::stoi(param));
		}
		else {
			goToLine();
		}
	}

	// LIST command (L)
	else if (regex_search(command, match, regex(LIST_REGEX))) {
		string paramA = match[1].str();
		string paramB = match[2].str();

		if (paramA.empty() && paramB.empty()) {
			list();
		}
		else if (paramB.empty()) {
			list(stoi(paramA));
		}
		else {
			list(stoi(paramA), stoi(paramB));
		}
	}

	// SUBSTITUTE command (S)
	else if (regex_search(command, match, regex(SUB_REGEX))) {
		string param = match[1].str();
		if (!param.empty()) {
			substituteLine(std::stoi(param));
		}
		else {
			substituteCurrentLine();
		}
	}

	// POSITION command (P)
	else if (regex_search(command, match, regex(POSITION_REGEX))) {
		string param = match[1].str();
		if (!param.empty()) {
			scrollToPosition(stoi(param));
		}
		else {
			scrollToCurrent();
		}
	}

	// EXIT command
	else if (regex_search(command, match, regex(QUIT_REGEX))) {
		exit();
	}

	// HELP command
	else if (regex_search(command, match, regex(HELP_REGEX))) {
		displayHelpInfo();
	}

	else if (regex_search(command, match, regex(SAVE_EXIT_REGEX))) {
		saveDocument(outPath);
		exit();
	}

	else {
		stringstream ss;
		ss << "Unrecognized command : \'" << command << "\'";
		console.setStatusMessage(ss.str());
		displayBuffer();
	}
}

/**
	Loads a text-based document, saving each line into a StringLinkedList buffer
	@param path The path of the file to open.
*/
void Editor::openDocument(std::string path) {
	ifstream in;
	string temp;

	in.open(path);
	if (in.fail()) {
		//throw FileIOException();
	}

	if (in.is_open()) {
		while (getline(in, temp)) {
			linkedList.add(temp);
		}
		in.close();
	}
}

/**
	Saves the buffer to a file specified by the path parameter.
	@param path The path of the file to save the buffer to.
*/
void Editor::saveDocument(string path) {
	ofstream out;
	stringstream ss;

	out.open(path);

	if (out.is_open()) {
		out << linkedList;

		out.close();
	}

	ss << "File saved to: \"" << outPath << "\". Press ENTER to quit.";
	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Scrolls to the position of the currently selected line.
*/
void Editor::scrollToCurrent()
{
	if (currentLine > 0 && currentLine <= linkedList.size()) {
		console.setScrollPosition(currentLine);

		stringstream ss;
		ss << "Scrolled to position : " << currentLine;
		console.setStatusMessage(ss.str());
		displayBuffer();
	}
}

/**
	Scrolls to the position specified by the pos parameter.
	@param pos The position to scroll to.
*/
void Editor::scrollToPosition(int pos)
{
	if (pos > 0 && pos <= linkedList.size()) {
		console.setScrollPosition(pos);

		stringstream ss;
		ss << "Scrolled to position : " << pos;
		console.setStatusMessage(ss.str());
		displayBuffer();
	}
}

/**
	Inserts a line to the buffer a the specified location.
	@param at The location in which to insert the new line.
*/
void Editor::insertLine(int at) {
	if (at > 0 && at <= linkedList.size()) {
		linkedList.insertAt(at - 1, console.promptForInput());

		stringstream ss;
		ss << "Line inserted at position : " << at;

		console.setStatusMessage(ss.str());
		displayBuffer();
	}
}

/**
	Inserts a line to the buffer at the location of the currently selected line.
*/
void Editor::insertBeforeCurrentLine()
{
	if (currentLine > 0 && currentLine <= linkedList.size()) {
		linkedList.insertAt(currentLine - 1, console.promptForInput());

		stringstream ss;

		ss << "Line inserted at : " << currentLine;
		console.setStatusMessage(ss.str());
		displayBuffer();
	}
}

/**
	Displays the current buffer.
*/
void Editor::displayBuffer() {
	stringstream ss;
	ss << linkedList;
	console.setBufferSize(linkedList.size());
	console.drawBuffer(ss, currentLine);
}

/**
	Display command help information.
*/
void Editor::displayHelpInfo()
{
	stringstream ss;

	ss << "  CMD   PARAMETERS                  DESCRIPTION                                                             |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| D   | none, <pos>, <start, end> | Delete the line at <pos>, or a range of lines from <start> to <end>, or |" << endl;
	ss << "|     |                           | the currently selected line.                                            |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| E   | none                      | Saves the buffer and exits the program.                                 |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| G   | none, <pos>               | Sets the currently selected <pos>, or selects the first line.           |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| H   | none                      | Displays this help screen.                                              |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| I   | none, <pos>               | Inserts new line at <pos>, or inserts it at the selected line.          |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| L   | none, <pos>, <start, end> | Display the line at <pos> or a range of line from <start> to <end> or   |" << endl;
	ss << "|     |                           | the currently selected line.                                            |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| P   | none, <pos>               | Scrolls to the line at <pos>, or to the currently selected line.        |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| Q   | none                      | Quits the program without saving the buffer.                            |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| S   | none, <pos>               | Substitutes the line at <pos> or the current line.                      |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;
	ss << "| V   | none                      | Displays the entire buffer.                                             |" << endl;
	ss << "-------------------------------------------------------------------------------------------------------------" << endl;

	console.drawBuffer(ss, 0);
}

/**
	List a range of lines in the buffer.
	@param a The first integer on the range.
	@param b The second integer on the range.
*/
void Editor::list(int a, int b) {
	int height = max(a, b) - min(a, b);

	stringstream ss;
	ss << linkedList;

	stringstream msg;
	msg << "Viewing lines : " << min(a,b) << " through " << max(a,b);

	console.setStatusMessage(msg.str());
	console.drawBuffer(ss, currentLine, a, b);
}

/**
	Lists a specific line in the buffer.
	@param line The position of the line to list.
*/
void Editor::list(int line) {
	if (line > 0 && line <= linkedList.size()) {
		string value = linkedList.get(line - 1);

		stringstream msg;
		msg << "Viewing line : " << line;
		console.setStatusMessage(msg.str());
		console.drawBuffer(value, line, (line == currentLine));
	}
}

/**
	Lists the currently selected line on the buffer.
*/
void Editor::list() {
	if (currentLine > 0 && currentLine <= linkedList.size()) {
		string value = linkedList.get(currentLine - 1);

		stringstream msg;
		msg << "Viewing selected line : " << currentLine;
		console.setStatusMessage(msg.str());
		console.drawBuffer(value, currentLine, true);
	}
}

/**
	Deletes a range of lines from the buffer.
	@param from The start position of the range to be deleted.
	@param to The end position of the range to be deleted.
*/
void Editor::deleteRange(int from, int to) {
	int start = min(from, to);
	int numItems = max(from, to) - min(from, to);
	linkedList.deleteRange(start - 1, numItems + 1);

	stringstream ss;
	ss << "Deleted lines " << min(from, to) << " through " << max(from, to);

	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Deletes a specific line from the buffer.
	@param line The position of the line to be deleted.
*/
void Editor::deleteLine(int line) {
	stringstream ss;

	console.setStatusMessage(ss.str());
	displayBuffer();
	if (line == -1) {
		if (currentLine > 0 && currentLine <= linkedList.size()) {
			linkedList.deleteNode(currentLine - 1);
			ss << "Deleted line at position : " << currentLine;
		}
	}
	else if (line > 0 && line <= linkedList.size()) {
		linkedList.deleteNode(line - 1);
		ss << "Deleted line at position : " << line;
	}

	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Substitutes the value of the currently selected line in the buffer.
*/
void Editor::substituteCurrentLine()
{
	stringstream ss;

	if (currentLine > 0 && currentLine <= linkedList.size()) {
		linkedList.updateValue(currentLine - 1, console.promptForInput());
		ss << "Line " << currentLine << " updated";
	}

	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Substitutes the value of the line specified by the line parameter.
	@param line The position of the line to be updated.
*/
void Editor::substituteLine(int line) {
	stringstream ss;

	if (line > 0 && line <= linkedList.size()) {
		linkedList.updateValue(line - 1, console.promptForInput());
		ss << "Line " << line << " updated";
	}

	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Set the currently selected line in the buffer.
	@param line The position of the line to be selected.
*/
void Editor::goToLine(int line) {
	int maxSize = linkedList.size();

	if (line > maxSize) {
		currentLine = maxSize;
	}
	else {
		currentLine = line;
	}

	stringstream ss;
	ss << "Current line set to " << line;

	console.setStatusMessage(ss.str());
	displayBuffer();
}

/**
	Exits the program.
*/
void Editor::exit() {
	shouldExit = true;
}