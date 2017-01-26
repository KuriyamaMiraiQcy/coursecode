#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void copyfile(char *source, char *dest);
void fixdir(char *dir, char *filename, char *result);
void isexist(char *dir);
void cpr(char *sourcedir, char *destdir);

//对单个文件进行复制
void copyfile(char *source, char *dest)
{
	FILE *readf = fopen(source, "r");
	FILE *writef = fopen(dest, "w");
	char temp;
	while (1) {
		temp = fgetc(readf);
		if (feof(readf))
			break;
		fputc(temp, writef);
	}
	fclose(readf);
	fclose(writef);
	return;
}

//生成需要的文件目录字符串
void fixdir(char *dir, char *filename, char *result)
{
	strcpy(result, dir);
	strcat(result, "/");
	strcat(result, filename);
}

//检查dir目录是否存在，如果不存在就新建一个
void isexist(char *dir)
{
	if (!access(dir,R_OK))
		return;
	else {
		mkdir(dir, S_IRWXU);
	}
}

//递归复制
void cpr(char *sourcedir, char *destdir)
{
	struct stat filestat;
	DIR* dir;
	struct dirent* currentdp;
	int chdirgood;

	chdirgood = chdir(sourcedir);
	if (chdirgood == -1) {//如sourcedir是一个文件
		copyfile(sourcedir, destdir);//直接复制以后结束程序
	}
	else {//如果sourcedir是一个目录
		isexist(destdir);//检查目的文件目录是否存在
		dir = opendir(sourcedir);
		while ((currentdp = readdir(dir)) != NULL) {
			lstat(currentdp->d_name, &filestat);
			if (S_ISDIR(filestat.st_mode)) {//如果是目录
				if (strcmp(".", currentdp->d_name) == 0 || 
					strcmp("..", currentdp->d_name) == 0) {
					continue;
				}//不复制. 和 .. 目录
				char tempsdir[999], tempddir[999];
				fixdir(sourcedir, currentdp->d_name, tempsdir);
				fixdir(destdir, currentdp->d_name, tempddir);
				cpr(tempsdir, tempddir);//递归复制目录
				chdir(sourcedir);//重新返回原文件目录
			}
			else {//如果是单一文件
				char tempsdir[999], tempddir[999];
				fixdir(sourcedir, currentdp->d_name, tempsdir);
				fixdir(destdir, currentdp->d_name, tempddir);
				copyfile(tempsdir, tempddir);
			}
		}
		closedir(dir);
		free(currentdp);
	}
}

int main(int argc, char const *argv[])
{
	char sdir[99], ddir[99];
	strcpy(sdir, argv[1]);//原文件地址
	strcpy(ddir, argv[2]);//目的文件地址
	cpr(sdir, ddir);//执行复制操作
	return 0;
}