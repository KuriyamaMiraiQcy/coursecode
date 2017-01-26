#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

char* DestinyDir = "/home/kuriyama/qian";

void deal(char *filename);
void fixdir(char *dir, char *filename, char *result);

//依次遍历文件并复制
void deal(char *filename)
{
	pid_t childpid;
	char sdir[999], ddir[999];
	if (strcmp(".", filename) == 0 || strcmp("..", filename) == 0)
		return; //不复制. 和 .. 目录
	fixdir(getcwd(NULL, 0), filename, sdir);
	fixdir(DestinyDir, filename, ddir);
	childpid = fork();//创建新进程
	if (childpid == 0) {
		printf("now in child!\n");
		execl("/home/kuriyama/Videos/cpr", 
			"cpr", sdir, ddir, NULL);//调用cpr程序
	}
	if (wait(NULL) == childpid)//等待子进程结束
		printf("this child is end : %d\n", childpid);

}

void fixdir(char *dir, char *filename, char *result)
{
	strcpy(result, dir);
	strcat(result, "/");
	strcat(result, filename);
}

int main(void)
{
	char* buf;
	DIR* dir;
	struct dirent* currentdp;
	
	mkdir(DestinyDir, S_IRWXU); //新建一个目录
	buf = getcwd(NULL, 0);
	chdir(buf);
	dir = opendir(buf);
	while ((currentdp = readdir(dir)) != NULL) {
		deal(currentdp->d_name);
	}
	closedir(dir);
	return 0;
}