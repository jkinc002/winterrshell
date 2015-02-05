#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <stdio.h>
#include <vector>

std::vector<int> arg_order;

/*
DIR *opendir(const char *name); returns NULL for errors
struct dirent *readdir(DIR *dirp); returns NULL for errors
int closedir(DIR *dirp); returns 0 normally, -1 for errors
*/

void print_arr(char * c){
	for(int i=0;c[i]!='\0';++i){
		std::cout << c[i];
	}
	std::cout << '\n';
}

void print_2d_arr(char **arr,int size){
	for(int i=0;i<size;++i){
		print_arr(arr[i]);
	}
}

/*NOTES:
hidden files have a '.' in front
*/
/*
void exec_ls(char **argv,int argc){
	bool aflag = false;
	bool lflag = false;
	bool rflag = false;
	bool is_flag = false;
	bool is_dir = false;
	char **dir_arr = new char *[argc];
	int dir_count = 0;
	for(int i=0;i<argc;++i){
		is_flag = false;
		if(argv[i][0]=='-') is_flag = true;
		else{
			is_dir == true;
			dir_arr[dir_count] = argv[i];
			++dir_count;
		}
		for(int j=1;argv[i][j]!='\0';++j){
			if(is_flag == true && is_dir == false){
				if(argv[i][j] == 'a') aflag = true;
				if(argv[i][j] == 'l') lflag = true;
				if(argv[i][j] == 'R') rflag = true;
			}
		}
	}
	for(int k=0;k<dir_count;++k){
		DIR *dirp1 = opendir(dir_arr[k]);
		if(dirp1 == NULL) perror("opendir");

		dirent *direntp;
		while(direntp = readdir(dirp1)){
			if(direntp == NULL)perror("readdir");
			else{
				if(aflag == true){
					if(direntp->d_name[0]!='.'){
						print_arr(direntp->d_name);
					}
				}
				else{
					print_arr(direntp->d_name);
				}
			}
			int close = closedir(dirp1);
			if(close == -1)perror("closedir");
		}
	}
		
}
*/

int main(int argc, char **argv)
{
	if(argc == 1){
		char dir[] = ".";
		argv[0] = dir;
	}
	else{
		for(int i=0;i<argc-1;++i){
			argv[i]=argv[i+1];
		}
		argc -= 1;
	}
	bool aflag = false;
	bool lflag = false;
	bool rflag = false;
	bool is_flag = false;
	bool is_dir = false;
	int dir_count = 0;
	for(int i=0;i<argc;++i){
		is_flag = false;
		if(argv[i][0]=='-') is_flag = true;
		else{
			is_dir == true;
			argv[dir_count] = argv[i];
			++dir_count;
		}
		for(int j=1;argv[i][j]!='\0';++j){
			if(is_flag == true && is_dir == false){
				if(argv[i][j] == 'a') aflag = true;
				if(argv[i][j] == 'l') lflag = true;
				if(argv[i][j] == 'R') rflag = true;
			}
		}
	}
	for(int k=0;k<dir_count;++k){
		DIR *dirp1 = opendir(argv[k]);
		if(dirp1 == NULL) perror("opendir");

		dirent *direntp;
		while(direntp = readdir(dirp1)){
			if(direntp == NULL)perror("readdir");
			else{
				if(aflag != true){
					if(direntp->d_name[0]!='.'){
						print_arr(direntp->d_name);
					}
				}
				else{
					print_arr(direntp->d_name);
				}
			}
		}
		int close = closedir(dirp1);
		if(close == -1)perror("closedir");
	}
	
		/*

		DIR *dirp1 = opendir(in_cstr);
		if(dirp1 == NULL) perror("opendir");

		dirent *direntp;
		while(direntp = readdir(dirp1)){
			if(direntp == NULL)perror("readdir");
			print_arr(direntp->d_name);
		}
	}

		int close = closedir(dirp1);
		if(close == -1)perror("closedir");
		std::cout << "close = " << close << '\n';

		*/

	return 0;
}
