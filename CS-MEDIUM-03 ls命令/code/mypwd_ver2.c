#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_DEPTH 256

char *filename_q[MAX_DEPTH];
int depth=0;

ino_t GetIno(char *filename)
{
    struct stat statbuf;
    if(stat(filename,&statbuf)==-1)
    {
        perror("stat\n");
        exit(-1);
    }
    return statbuf.st_ino;
}

char* GetFileName(ino_t ino)
{
    DIR* dir;
    struct dirent *entry;
    char *filename=NULL;
    if((dir=opendir("."))==NULL)
    {
        perror("opendir\n");
        exit(-1);
    }
    else
    {
        while((entry=readdir(dir))!=NULL)
        {
            if(entry->d_ino==ino)
            {
                filename=strdup(entry->d_name);
                break;
            }
        }
        closedir(dir);
    }
    return filename;
}

void check()
{
    if(depth>=MAX_DEPTH)
    {
        perror("path overdepth\n");
        exit(-1);
    }
}

int main(int argc,char *argv[])
{
    while(1)
    {
        ino_t cur_ino=GetIno(".");
        ino_t par_ino=GetIno("..");
        if(cur_ino==par_ino)break; // Reach the root directory
        chdir(".."); // Back to parent directory
        filename_q[depth++]=GetFileName(cur_ino);
        check();
    }
    for(int i=depth-1;i>=0;--i)
    {
        printf("/%s",filename_q[i]);
    }
    printf("\n");
    return 0;
}