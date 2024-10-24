#include<stdio.h>
#include<unistd.h>
char buf[260];
int main()
{
    getcwd(buf,sizeof(buf));
    printf("%s\n",buf);
    return 0;
}
