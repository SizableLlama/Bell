#include "parser.hpp"
#include "structs.hpp"
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fstream>
#include <fcntl.h>
//std::vector<std::string> into a std::vector<char*>


// this should all be in a class executioner.

class Executioner {
private:
	std::vector<char*> commandToArgv(const Command& command) {
		std::vector<char*> argv;
		for (const std::string& arg : command.args) {
			argv.push_back( const_cast<char*>(arg.c_str()) );
		}
		argv.push_back(nullptr);
		return argv;
	}

	bool builtIn(const Command& command) {
		if (command.args[0] == "cd") {
			if (command.args.size() < 2) {
				char* home = getenv("HOME");
				chdir(home);
				return true;
			}
			else {
				chdir(command.args[1].c_str());
				return true;
			}
		}
		else if (command.args[0] == "exit") {
			std::cout<<std::exit;
			return true;
		}
		return false;
	}

	void redirection(const Command& command) {
		if (command.redir_type == RedirType::NONE){
			return;
		}
		else if (command.redir_file.size() == 0) {
			std::cerr<<"expected file name after redirect.\n";
		}

		else if (command.redir_type == RedirType::APPEND) { // >>
			int file_desc = open(command.redir_file.c_str(), O_RDWR | O_CREAT | O_APPEND);

			int copy_desc = dup(file_desc);
		}
		else if (command.redir_type == RedirType::INPUT) { // <
			int file_desc = open(command.redir_file.c_str(), O_RDONLY);

			int copy_desc = dup(file_desc);
		}
		else if (command.redir_type == RedirType::OUTPUT) { // >
			int file_desc = open(command.redir_file.c_str(), O_CREAT | O_RDWR);
		}
	}









/*
 TODO: empty string, eg "hi  with no closing indicator results in a crash.

 TODO: benchmark

 TODO: figure out the wait function

 TODO stop ctrl + c
 
 TODO make a nice main
 
 TODO: fix the double output

 TODO add functionality to &&, ||, |, >, <, >>

 TODO stop crashes. This is caused by the above tokens and insufficient commands following them.
*/

public:
	Executioner() = default;
	void run(const Pipeline& pipeline) {
		//TODO make this elif chain a switch. Also, check for built ins before commandToArgv.
		const Command& com = pipeline.commands[0];
		
		std::vector<char*> argv = commandToArgv(com);
		if (builtIn(com)) return;
		redirection(com);

		int status;
		status = fork();		

		if (status == 0) {
			execvp(argv[0], argv.data());			
		}
		else if (status == -1) {
			std::cerr<<"There was an error forking.\n";
		}
		else {
			waitpid(status, nullptr, 0);
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
//open() is then used to open a file which is also in the commands vector.
