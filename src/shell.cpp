#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>

std::vector<int> conn_order;													//vector to track order and type of connectors
int words = 0;																	//int to keep track of argument #'s throughout user input
																				//I'll keep this if I might need it in the future
void read_order(char *cmds){
	int flag = 0;
	for(int i=0; cmds[i]!='\0'; ++i){
		if(cmds[i] == ';'){
			conn_order.push_back(1);
			flag = 0;
		}
		else if(cmds[i] == '&' && cmds[i+1] == '&'){
			conn_order.push_back(2);
			++i;
			flag = 0;
		}
		else if(cmds[i] == '|' && cmds[i+1] == '|'){
			conn_order.push_back(3);
			++i;
			flag = 0;
		}
		else if(cmds[i] != ' ' && flag == 0){
		//	conn_order.push_back(-1);
			flag = 1;
		}
		else if(cmds[i] == ' ') flag = 0;
	}
}

void disp_v(std::vector<int> &v){
	for(unsigned i=0; i<v.size(); ++i){
		std::cout << v.at(i);
	}
	std::cout << '\n';
}
//Counts the number of "words" in the User's input between white-space and connectors. I'm afraid to remove it ):
void count_words(char *cmds){
	for(unsigned i=0; i<conn_order.size(); ++i){
		if(conn_order.at(i) == -1) ++words;
	}
}

//This everything in the user's input found behind the first '#' character.
void tok_comment(char *cmd){
	cmd = strtok(cmd, "#");
}

/*
NAME: tok_conn //why do I even include the name??? It's right THERE!
INPUT: char** //is the array of pointers that will be modified to carry each argument tokenized in this function
		char* //is the c_string that holds the not yet tokenized string of arguments
OUTPUT: Returns an integer whose value is the calculated number of "argument-lines" that existed between the
		now tokenized connectors. This outpu becomes 'int size' for 'tok_space()'
*/
int tok_conn(char **cmdlist, char *cmd){
	int argc = 0;
	cmdlist[argc] = strtok(cmd, ";&|");
	while(cmdlist[argc] != NULL){
		++argc;
		cmdlist[argc] = strtok(NULL, ";&|");
	}
	return argc;
}

//Function to recognize the 'exit' command. This took most of my time.
bool is_exit(char *c){
	if(c[0] == 'e' && c[1] == 'x' && c[2] == 'i' && c[3] == 't' && c[4] == '\0') return true;
	return false;
}

/*
NAME: tok_space() //though it's actually where commands are executed as well
INPUT: char** //holds commands whose connectors were tokenized, but not the white space between them
		int //integer whose value is the number of "argument lines" calculated to exist between all connectors
FUNCT: Loops inside the function. Tokenizing an entire argument line's white space and organizes those tokens to be
		executed with 'execvp()'
*/

void tok_space(char **cmdlist, int size){
	int curr = 0;
	unsigned index = 0;
	char **temp;
	temp = new char *[words + 1];
	while(curr != size){
		int argc = 0;
		temp[argc] = strtok(cmdlist[curr], " ");
		while(temp[argc] != NULL){
			if(is_exit(temp[argc])){
				exit(0);
			}
			++argc;
			temp[argc] = strtok(NULL, " ");							//temp[argc] holds the "space-tokenized" commands
		}
		argc = 0;


	
		int pid = fork();
		if(pid == -1){
			perror("fork");
		}
		else if(pid == 0){
			int ret = execvp(temp[argc], temp);						//There is some black magic happening here
			if(ret == -1){
				perror("execvp");
			}
			exit(1);
		}
		else{
			int ret;
			waitpid(pid,&ret,0);
			if(conn_order.size() > 0 && index < conn_order.size()){
				if(ret != 0){
					if(conn_order.at(index) == 2){
						return;
					}
					++index;
				}
				else{
					if(conn_order.at(index) == 3){
						return;
					}
					++index;
				}
			}
			++curr;
		}
	}
}
/*
NAME: is_comment()
INPUT: string
OUTPUT: boolean
FUNCT: Returns true if entire string was a comment. False otherwise.
*/
bool is_comment(std::string &s){
	if(s.at(0) == '#') return true;
	return false;
}


int main()
{
	while(1){
		std::cout << "$ ";
		std::string cmds;															//string for user input;
		getline(std::cin, cmds);
		if(cmds == "exit") exit(0);													//if input is "exit", exit process
		if(cmds.size() != 0 && !(is_comment(cmds))){								//brach if there was input
			char *cmd_c_str;
			cmd_c_str = new char [cmds.size() + 1];
			strcpy(cmd_c_str, cmds.c_str());										//convert cmd string to c_string.
			char ** cmdlist;
			cmdlist = new char *[cmds.size() + 1];
			tok_comment(cmd_c_str);													//'cmdlist' holds the first command
			read_order(cmd_c_str);
			int list_size = tok_conn(cmdlist,cmd_c_str);
			tok_space(cmdlist, list_size);											//move to function where everything is executed
			

		}
		words = 0;
		conn_order.clear();
	}
	return 0;
}

