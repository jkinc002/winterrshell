#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>

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
}

void print_2d_arr(char **arr,int size){
	for(int i=0;i<size;++i){
		print_arr(arr[i]);
		std::cout << "  ";
	}
}

void print_vec(std::vector<std::string> &v){
	for(unsigned i=0;i<v.size();++i){
		std::cout << v.at(i) << "  ";
	}
}

int main(int argc, char **argv)
{
	int fix1;
	if(argc == 1){
		fix1 = 0;
		char dir[] = ".";
		argv[0] = dir;
	}
	else{
		for(int i=0;i<argc-1;++i){
			argv[i]=argv[i+1];
		}
		argc -= 1;
		fix1 = 1;
	}
	bool aflag = false;
	bool lflag = false;
//	bool rflag = false;
	bool is_flag = false;
	bool is_dir = false;
	int dir_count = 0;
	for(int i=0;i<argc;++i){
		is_flag = false;
		if(argv[i][0]=='-') is_flag = true;
		else{
			is_dir = true;
			argv[dir_count] = argv[i];
			++dir_count;
		}
		for(int j=1;argv[i][j]!='\0';++j){
			if(is_flag == true && is_dir == false){
				if(argv[i][j] == 'a') aflag = true;
				if(argv[i][j] == 'l') lflag = true;
				if(argv[i][j] == 'R');// rflag = true;
			}
		}
	}
	std::vector<std::string> sorted;
	if(fix1 == 0 || dir_count == 0){
		char arg_fix[] = ".";
		argv[0] = arg_fix;
		if(dir_count == 0){
			++dir_count;
		}
	}
	for(int k=0;k<dir_count;++k){
		DIR *dirp1 = opendir(argv[k]);
		if(dirp1 == NULL){
			if(!lflag){
				print_arr(argv[k]);
				std::cout << "  ";
			}
			else{
				struct stat s;
				int ret = stat(argv[k],&s);
				ret = s.st_mode;
				if(S_ISDIR(ret)!=0)std::cout << "d";
				else std::cout << "-";
				std::cout << ((ret & S_IRUSR)?"r":"-")
						  << ((ret & S_IWUSR)?"w":"-")
						  << ((ret & S_IXUSR)?"x":"-")
						  << ((ret & S_IRGRP)?"r":"-")
						  << ((ret & S_IWGRP)?"w":"-")
						  << ((ret & S_IXGRP)?"x":"-")
						  << ((ret & S_IROTH)?"r":"-")
						  << ((ret & S_IWOTH)?"w":"-")
						  << ((ret & S_IXOTH)?"x":"-")
						  << ' ' << s.st_nlink
						  << ' ' << s.st_uid
						  << ' ' << s.st_gid
						  << ' ' << s.st_size
						  << ' ' << s.st_mtime << ' ';
						  print_arr(argv[k]);
						  std::cout << '\n';
			}
		}
		else{
			if(dir_count > 1){
				std::cout << '\n';
				print_arr(argv[k]);
				std::cout << ":\n";
			}

			dirent *direntp;
			while((direntp = readdir(dirp1))){
				if(direntp == NULL)perror("readdir");
				else{
					if(aflag != true){
						if(direntp->d_name[0]!='.'){
							std::string curr_str(direntp->d_name);
							sorted.push_back(curr_str);
						}
					}
					else{
						std::string curr_str(direntp->d_name);
						sorted.push_back(curr_str);
					}
				}
			}
			std::sort(sorted.begin(), sorted.begin() + sorted.size());
			if(!lflag){
				print_vec(sorted);
			}
			else{
				for(unsigned l=0;l<sorted.size();++l){
					struct stat s;
					int ret = stat(sorted.at(l).c_str(),&s);
					ret = s.st_mode;
					if(S_ISDIR(ret)!=0)std::cout << "d";
					else std::cout << "-";
					std::cout << ((ret & S_IRUSR)?"r":"-")
							  << ((ret & S_IWUSR)?"w":"-")
							  << ((ret & S_IXUSR)?"x":"-")
							  << ((ret & S_IRGRP)?"r":"-")
							  << ((ret & S_IWGRP)?"w":"-")
							  << ((ret & S_IXGRP)?"x":"-")
							  << ((ret & S_IROTH)?"r":"-")
							  << ((ret & S_IWOTH)?"w":"-")
							  << ((ret & S_IXOTH)?"x":"-")
							  << ' ' << s.st_nlink
							  << ' ' << s.st_uid
							  << ' ' << s.st_gid
							  << ' ' << s.st_size
							  << ' ' << s.st_mtime
							  << ' ' << sorted.at(l) << '\n';
				}
			}
			int close = closedir(dirp1);
			if(close == -1)perror("closedir");
		}
	sorted.clear();
	}
	std::cout << '\n';
	return 0;
}
