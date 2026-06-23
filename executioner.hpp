#include "parser.hpp"
#include <cstdlib>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
//std::vector<std::string> into a std::vector<char*>

void foo();
// this should all be in a class executioner.

class Executioner {
public:

	std::vector<char*> commandToArgv(const Command& command) {
		std::vector<char*> argv;
		for (const std::string& arg : command.args) {
			argv.push_back(const_cast<char*>(arg.c_str()));
		}
		argv.push_back(nullptr);  // required sentinel — execvp scans until it sees this
		return argv;
	}
	
	void run(const Command& command, const Pipeline& pipeline) {
		//TODO make this elif chain a switch. Also, check for built ins before commandToArgv.
		std::vector<char*>argv = commandToArgv(command);
		size_t x = 0;
		while (x <= pipeline.commands.size()) {

			if (fork() == 0) {


				if ((std::string)argv[0] == "cd" ) {
					int err = 0;
					err = chdir(argv[1]);
					if (err == 1 ) {
						std::cerr<<"There was an error.\n";
						break;
					} 
				}
				else if ((std::string)argv[0] == "exit") {
					//TODO need to think of a better way to do this.
					std::cout<<std::exit;
				}
				else {
					execvp(argv[0], argv.data());
				}

			}
			else {
				std::cerr<<"There was an error.\n";
			}



			x++;
		}
		

		/*
		ls -la | grep ".cpp" > output.txt
		turns into:
		ls -la   and   grep ".cpp"
	
		
		need the && and || logic. if both left and right are true, run.
		if left is false but right is true, run.
		
		but how? There must be two arguments. The OS probably checks for us, then I can do accordingly.

		*/









	}
};
//So, i need to build arguments using the commands vector.
//i then loop that to execvp.
//open() is then used to open a file which is also in the commands vector.
