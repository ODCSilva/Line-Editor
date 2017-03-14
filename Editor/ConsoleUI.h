#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <sstream>
using namespace std;

class ConsoleUI
{
private:
	string headerInfo;
	string footerInfo;
	string statusMessage = "";
	
protected:
	int scrollPosition;
	int bufferSize;
	int currentLine;
public:
	ConsoleUI() : scrollPosition(1), bufferSize(0), currentLine(1) {};
	int calcAvailableBufferRoom();
	int getConsoleHeight();
	int getConsoleWidth();
	string promptForInput();
	void drawBuffer(string, int, bool);
	void drawBuffer(stringstream& ss, int);
	void drawBuffer(stringstream& ss, int, int, int);
	void drawCommandPrompt();
	void drawFooter(int);
	void drawHeader();
	void drawStatusBar();
	void resetConsoleColor();
	void setBufferSize(int);
	void setConsoleColor(int);
	void setFooterInfo(string);
	void setHeaderInfo(string);
	void setScrollPosition(int);
	void setStatusMessage(string);
};

#endif CONSOLEUI_H
