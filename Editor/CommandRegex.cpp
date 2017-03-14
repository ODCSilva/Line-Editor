#include "CommandRegex.h"

std::regex CommandRegex::getRegex(Command cmd) {
	std::string cmdRegex = "";

	switch (cmd) {
	case Command::DEL:
		cmdRegex = DELETE_REGEX;
		break;
	case Command::EXIT:
		cmdRegex = EXIT_REGEX;
		break;
	case Command::GOTO:
		cmdRegex = GOTO_REGEX;
		break;
	case Command::INSERT:
		cmdRegex = INSERT_REGEX;
		break;
	case Command::LIST:
		cmdRegex = LIST_REGEX;
		break;
	case Command::SAVE_EXIT:
		cmdRegex = SAVE_EXIT_REGEX;
		break;
	case Command::SUB:
		cmdRegex = SUB_REGEX;
		break;
	case Command::VIEW:
		cmdRegex = VIEW_REGEX;
		break;
	}

	return std::regex(cmdRegex);
}