#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

void mode(int mode, char *str);
void show(char *filename);

void show(char *filename)
{
	struct stat currentstat;
	char modestr[11];
	lstat(filename, &currentstat);//获取文件属性结构体
	mode(currentstat.st_mode, modestr);//处理文件类型和权限
	
	printf("%s\t", modestr);//输出文件的其它属性
	printf("%d\t", currentstat.st_nlink);
	printf("%d\t", currentstat.st_uid);
	printf("%d\t", currentstat.st_gid);
	printf("%lld\t", currentstat.st_size);
	printf("%.5s\t", 11 + ctime(&currentstat.st_mtime));
	printf("%s\n",filename); 
}

void mode(int mode, char *str)
{
    strcpy(str,"----------");
 
    if(S_ISDIR(mode)) str[0] = 'd';
    if(S_ISCHR(mode)) str[0] = 'c';
    if(S_ISBLK(mode)) str[0] = 'b';
 
    if(mode & S_IRUSR)  str[1] = 'r';
    if(mode & S_IWUSR)  str[2] = 'w';
    if(mode & S_IXUSR)  str[3] = 'x';
     
    if(mode & S_IRGRP)  str[4] = 'r';
    if(mode & S_IWGRP)  str[5] = 'w';
    if(mode & S_IXGRP)  str[6] = 'x';
 
    if(mode & S_IXOTH)  str[7] = 'r';
    if(mode & S_IXOTH)  str[8] = 'w';
    if(mode & S_IXOTH)  str[9] = 'x';
}

int main(void)
{
	char* buf;
	DIR* dir;
	struct dirent* currentdp;
	
	buf = getcwd(NULL, 0);
	chdir(buf);
	dir = opendir(buf);
	while ((currentdp = readdir(dir)) != NULL) {//对当前目录所有文件进行迭代
		show(currentdp->d_name);
	}
	closedir(dir);
	return 0;
}