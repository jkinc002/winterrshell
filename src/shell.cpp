#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <vector>


/*
PLANE:
	get input
	token to get everything infront of any '#' character
	token to get string in front of ';'
		  to get '||'
		  to get '&&'
	do things...


//void execute(char ** commands, char *cmd)
void disp(char **cmdlist){
}
*/

std::vector<int> conn_order;													//vector to track order and type of connectors
int words = 0;																	//int to keep track of argument #'s throughout user input

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
			conn_order.push_back(-1);
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

void count_words(char *cmds){
	for(unsigned i=0; i<conn_order.size(); ++i){
		if(conn_order.at(i) == -1) ++words;
	}
}

int c_size(char *c){
	int i = 1;
	for( ; c[i] != '\0'; ++i){}
	return i;
}

void tok_comment(char *cmd){
	cmd = strtok(cmd, "#");
}

void disp(char **cmdlist){
	std::cout << cmdlist[0][0] << '\n';
}

void disp_cmds(char **cmdlist, int size){
	for(int i = 0;i<size ; ++i){
		std::cout << cmdlist[i][0] << '\n';
	}
}

int tok_conn(char **cmdlist, char *cmd){
	int argc = 0;
	cmdlist[argc] = strtok(cmd, ";&|");
	while(cmdlist[argc] != NULL){
		++argc;
		cmdlist[argc] = strtok(NULL, ";&|");
	}
	return argc;
}

void tok_space(char **cmdlist, int size){
	int curr = 0;
	char **temp;
	temp = new char *[words + 1];
	while(curr != size){
	int argc = 0;
	temp[argc] = strtok(cmdlist[curr], " ");
	while(temp[argc] != NULL){
		++argc;
		temp[argc] = strtok(NULL, " ");							//temp[argc] holds the "space-tokenized" commands
	}
	argc = 0;


	
	int pid = fork();
	if(pid == -1){
		perror("fork");
	}
	else if(pid == 0){
		int ret = execvp(temp[argc], temp);
		if(ret == -1){
			perror("execvp");
		}
		exit(1);
	}
	else{
		wait(NULL);
		++curr;
	}
	}
}


int main()
{
	while(1){
		std::cout << "$ ";
		std::string cmds;															//string for user input;
		getline(std::cin, cmds);
		if(cmds == "exit") exit(0);													//if input is "exit", exit process
		if(cmds.size() != 0){														//brach if there was input
			char *cmd_c_str;
			cmd_c_str = new char [cmds.size() + 1];
			strcpy(cmd_c_str, cmds.c_str());										//convert cmd string to c_string.
			char ** cmdlist;
			cmdlist = new char *[cmds.size() + 1];
			tok_comment(cmd_c_str);													//'cmdlist' holds the first command
			read_order(cmd_c_str);
			count_words(cmd_c_str);
			int list_size = tok_conn(cmdlist, cmd_c_str);											//token out connectors and white spaces
			tok_space(cmdlist, list_size);
			

		}
		words = 0;
		conn_order.clear();
	}
	return 0;
}

