#include "ConsoleUI.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

/**
	Gets the width (in columns) of the console window.
	@returns An int representing the number of colums of width of the console window.
*/
int ConsoleUI::getConsoleWidth() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

/**
	Gets the height (in rows) of the console window.
	@returns An int representing the number of rows height of the console window.
*/
int ConsoleUI::getConsoleHeight() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

/**
	Draws the UI header.
*/
void ConsoleUI::drawHeader() {
	int width = getConsoleWidth() - headerInfo.size() - 3;

	setConsoleColor(112);
	cout << "-> " << headerInfo;
	setConsoleColor(119);
	cout << setfill('*') << setw(width) << "" << endl << endl << setfill(' ');

	resetConsoleColor();
}

/**
	Draws a line of the buffer into the console.
	@param value The value of the string to be drawn into the console.
	@param line The line number of the buffer to be written into the console.
	@param isCurrentline Indicated whether the line that is being drawn is the current line.
*/
void ConsoleUI::drawBuffer(string value, int line, bool isCurrentLine) {
	system("cls");
	drawHeader();

	if (isCurrentLine) {
		setConsoleColor(11);
	}
	else {
		setConsoleColor(3);
	}

	cout << setw(3) << line << " |";
	resetConsoleColor();

	if (isCurrentLine) {
		setConsoleColor(13);
	}

	cout << " " << value << endl;

	resetConsoleColor();
	drawFooter(1);
}

/**
	Draws the complete file buffer into the console.
	@param ss The stringstream that contains the file buffer.
	@param currentLine The currently selected line in the editor.
*/
void ConsoleUI::drawBuffer(stringstream& ss, int currentLine) {
	int line = 1;
	int height = 0;
	string temp;

	this->currentLine = currentLine;

	system("cls");

	drawHeader();

	while (getline(ss, temp)) {
		if (line >= scrollPosition) {

			if (height > calcAvailableBufferRoom()) {
				break;
			}

			if (line == currentLine) {
				setConsoleColor(11);
			}
			else {
				setConsoleColor(3);
			}

			cout << setw(3) << line << " |";
			resetConsoleColor();

			if (line == currentLine) {
				setConsoleColor(13);
			}

			cout << " " << temp << endl;

			resetConsoleColor();

			height++;
		}

		line++;
	}

	drawFooter(height);
}

/**
	Draws a range of lines into the buffer.
	@param ss The stringstream that contains the file buffer.
	@param currentLine The currently selected line in the editor.
	@param start The starting position of the range.
	@param end The end position of the range.
*/
void ConsoleUI::drawBuffer(stringstream& ss, int currentLine, int start, int end) {
	int line = 1;
	int height = 0;
	string temp;

	system("cls");
	drawHeader();

	while (getline(ss, temp)) {
		if (line >= min(start, end) && line <= max(start, end)) {
			if (height > calcAvailableBufferRoom()) {
				break;
			}

			if (line == currentLine) {
				setConsoleColor(11);
			}
			else {
				setConsoleColor(3);
			}

			cout << setw(3) << line << " |";
			resetConsoleColor();

			if (line == currentLine) {
				setConsoleColor(13);
			}

			cout << " " << temp << endl;

			height++;
		}
		resetConsoleColor();
		line++;
		bufferSize = line;
	}

	drawFooter(height);
}

/**
	Draws the command prompt UI element into the console window.
*/
void ConsoleUI::drawCommandPrompt() {
	COORD pos = { 0, (short)getConsoleHeight() - 3 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	cout << " >> ";
}

/**
	Draws the footer portion of the UI into the console window.
	@param padding The amount of padding (empty lines) to be drawn after the 
	buffer.
*/
void ConsoleUI::drawFooter(int padding) {
	int width = getConsoleWidth() - footerInfo.size() - 3;

	int fillHeight = getConsoleHeight() - padding - 7;

	for (int i = 0; i < fillHeight; i++) {
		cout << endl;
	}

	cout << endl;
	setConsoleColor(112);
	cout << "<- " << footerInfo;
	setConsoleColor(119);
	cout << setfill('*') << setw(width) << "" << endl << setfill(' ');

	resetConsoleColor();
	drawStatusBar();
	drawCommandPrompt();
}

/**
	Draws the status bar UI element into the console window.
*/
void ConsoleUI::drawStatusBar() {
	stringstream ss;

	ss << " lines : " << bufferSize << " SEL : " << this->currentLine << " ";
	string lines = ss.str();
	int width = getConsoleWidth() - statusMessage.size() - lines.size() - 1;

	cout << endl << endl;
	setConsoleColor(143);
	cout << " " << statusMessage;
	setConsoleColor(136);
	cout << setfill('*') << setw(width) << "";
	setConsoleColor(143);
	cout << lines << setfill(' ');
	resetConsoleColor();

	statusMessage = "";
}

/**
	Prompts the user for input.
*/
string ConsoleUI::promptForInput() {
	string input;

	COORD pos = { 0, (short)getConsoleHeight() - 3 };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
	cout << setw(getConsoleWidth()) << "\r : ";
	getline(cin, input);

	return input;
}

/**
	Sets the color of the console window.
	@param color An integer representing the color to be used for drawing.
*/
void ConsoleUI::setConsoleColor(int color) {
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hstdout);
	SetConsoleTextAttribute(hstdout, color);
}

/**
	Resets the console window colors to their original state.
*/
void ConsoleUI::resetConsoleColor() {
	setConsoleColor(15);
}

/**
	Sets the buffer size.
	@param size The size of the current buffer.
*/
void ConsoleUI::setBufferSize(int size)
{
	bufferSize = size;
}

/**
	Caluclates the amount of space available to be used for drawing the file buffer.
	@returns An integer representing the number of lines available for drawing the buffer.
*/
int ConsoleUI::calcAvailableBufferRoom() {
	return getConsoleHeight() - 8;
}

/**
	Sets the information to be displayed in the header bar.
*/
void ConsoleUI::setHeaderInfo(string value) {
	headerInfo = value;
}

/**
	Sets the scrol position.
	@param pos The new scroll position.
*/
void ConsoleUI::setScrollPosition(int pos)
{
	scrollPosition = pos;
}

/**
	Sets the information to be displayed in the footer bar.
*/
void ConsoleUI::setFooterInfo(string value) {
	footerInfo = value;
}

/**
	Sets the message to be displayed in the status bar when the buffer has been redrawn.
	@param value The message to be displayed.
*/
void ConsoleUI::setStatusMessage(string value) {
	statusMessage = value;
}