#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

char sdir[999], ddir[999];
//用全局变量给cpr函数传送参数

void deal(char *filename);
void fixdir(char *dir, char *filename, char *result);
void copyfile(char *source, char *dest);
void isexist(char *dir);
void cpr(char *sourcedir, char *destdir);
void list(void);
void *thr_ls(void *arg);
void *thr_cpr(void *arg);

//在list函数中用来调用cpr线程
void deal(char *filename)
{
	pid_t childpid;

	if (strcmp(".", filename) == 0 || strcmp("..", filename) == 0)
		return;
	fixdir(getcwd(NULL, 0), filename, sdir);
	fixdir("/home/kuriyama/Qian", filename, ddir);
	pthread_t ntid;
	pthread_create(&ntid, NULL, thr_cpr, NULL);//创建线程
	pthread_join(ntid, NULL);//等待线程结束
	puts(filename);
}

//生成需要的文件目录字符串
void fixdir(char *dir, char *filename, char *result)
{
	strcpy(result, dir);
	strcat(result, "/");
	strcat(result, filename);
}

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
		copyfile(sourcedir, destdir);//直接复制以后结束cpr
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

//依次遍历文件
void list(void)
{
	char* buf;
	DIR* dir;
	struct dirent* currentdp;
	
	buf = getcwd(NULL, 0);
	chdir(buf);
	dir = opendir(buf);
	while ((currentdp = readdir(dir)) != NULL) {
		deal(currentdp->d_name);
	}
	closedir(dir);
}

//list函数的线程
void *thr_ls(void *arg)
{
	puts("In thr_ls !!!");
	list();
	puts("Get out of thr_ls !!!");
	return NULL;
}

//cpr函数的线程
void *thr_cpr(void *arg)
{
	puts("In thr_cpr !!!");
	cpr(sdir, ddir);
	puts("Get out of thr_cpr !!!");
	return NULL;
}

int main(void)
{
	pthread_t ntid;
	mkdir("/home/kuriyama/Qian", S_IRWXU);
	pthread_create(&ntid, NULL, thr_ls, NULL);//新建thr_ls线程
	pthread_join(ntid, NULL);//等待线程结束
	return 0;
}