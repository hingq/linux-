/*
 *File:my_ls.c
 *This example implements a simple ls function.
 *Writen by Shawn Yang
 *Create time : Aug 30 , 2023
 *Update time : Sep 13 , 2023
*/

#include<dirent.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<linux/types.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
void list(char* path);
void list1(char* path1, char* path2);

int main(int argc, char* argv[])
{
	char name[256];
	printf("type\t\tsize\tname\n");
	printf("****************************************\n");
	if (argc == 1) {
		printf("name:");
		scanf("%s", name);
		list(name);
	}
	else if (argc == 3) {
		list1(argv[1], argv[2]);
	}
	else
		list(argv[1]);
}

void list(char* path)
{
	DIR* dirname;
	struct dirent* content;
	struct stat sb;
	/*open directory*/
	if ((dirname = opendir(path)) == NULL) {
		perror("open diretory");
		return;
	}

	chdir(path); //change current dir

	/*Read dirctory*/
	while ((content = readdir(dirname)) != NULL) {
			

		if (content->d_type == 4) {
			printf("dir\t\t");
		}
		else if (content->d_type == 8) {
			printf("file\t\t");
		}
		else {
			printf("others\t\t");
		}

		stat(content->d_name, &sb);//Get file status
				
		printf("%ld\t", sb.st_size);
		printf("%s\t", content->d_name);
	}

	closedir(dirname);
	chdir("..");
}
void list1(char* path1, char* path2)
{
	int showHidden = 0;
	int longFormat = 0;
	int recursive = 0;
	DIR* dir;
	struct dirent* entry;
	struct stat file_stat;
    char file_path[256];
	switch (*path2) {
	case 'a':
		showHidden = 1;
		break;
	case 'l':
		longFormat = 1;
		break;
	case 'R':
		recursive = 1;
		break;
	}
	if ((dir = opendir(path1)) == NULL) {
		perror("open diretory");
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		 
            snprintf(file_path, sizeof(file_path)+1, "%s/%s", path1, entry->d_name);
             // 跳过当前目录和上级目录   cmd=-a output 
        	if (entry->d_name[0] == '.' && !showHidden)
           		 continue;
        	// 输出文件名
        	printf("%s", entry->d_name);
			if (longFormat) {
                if(stat(path1,&file_stat)==-1){
                    perror("stat");
                    exit(EXIT_FAILURE);
                }
                struct passwd *pwd = getpwuid(file_stat.st_uid);
                struct group *grp = getgrgid(file_stat.st_gid);
                printf("\t");
                printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
                printf("\t%d", (int)file_stat.st_nlink); //连接数
                printf("\t%s", pwd->pw_name); //所有者
                printf("\t%s", grp->gr_name); // group
                printf("\t%d", (int)file_stat.st_size); //size
                printf("\t%s", ctime(&file_stat.st_mtime)); //create-time
			}
            printf("\n");
           if (recursive && entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            list1(file_path,path2);
        }
	}
	if (closedir(dir) == -1) {
		perror("closedir");
		exit(EXIT_FAILURE);
	}
	printf("\n");
}