#include "parser.hpp"
#include <sstream>
#include <vector>
#include <iostream>

//std::vector<std::string> into a std::vector<char*>

void foo();
// this should all be in a class executioner.


class Executioner{
public:
	
	std::vector<char*> commandToArgv(const Command& command) {
		std::vector<char*> argv;
		for (const std::string& arg : command.args) {
			argv.push_back(const_cast<char*>(arg.c_str()));
		}
		argv.push_back(nullptr);  // required sentinel — execvp scans until it sees this
		return argv;
	}
};
//So i need to change the commands vector to char, the commands are made up of strings though.
//who knows what that actually means.
