#ifndef COMMANDREGEX_H
#define COMMANDREGEX_H
#include <string>
#include <regex>

const std::string DELETE_REGEX = "^[Dd]\\s?([0-9]*)\\s?([0-9]*)$";
const std::string EXIT_REGEX = "^[Qq]$";
const std::string GOTO_REGEX = "^[Gg]\\s?([0-9]*)$";
const std::string INSERT_REGEX = "^[Ii]\\s?([0-9]*)$";
const std::string LIST_REGEX = "^[Ll]\\s?([0-9]*)\\s?([0-9]*)$";
const std::string SAVE_EXIT_REGEX = "^[Ee]$";
const std::string SUB_REGEX = "^[Ss]\\s?([0-9]*)$";
const std::string VIEW_REGEX = "^[Vv]$";

enum Command { DEL, EXIT, GOTO, INSERT, LIST, SAVE_EXIT, SUB, VIEW };

class CommandRegex {
public:
	static std::regex getRegex(Command cmd);
};

#endif COMMANDREGEX_H
