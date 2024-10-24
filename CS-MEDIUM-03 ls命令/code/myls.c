#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> // ���ܲ��Ǳ���ģ�������POSIX��׼
#include <time.h>
/*
    ����ļ���Ϣ
*/
void print_file_info(const char *name, struct stat *statbuf) {
    printf("%s\t", name);  //�ļ���
    printf("%ld\t", statbuf->st_size); // �ļ���С
    printf("%o\t", statbuf->st_mode & 0777); // �ļ�Ȩ�ޣ�����ʾ�����λ��
    // ����޸�ʱ��
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", localtime(&statbuf->st_mtime));
    printf("%s\n", timebuf);
}
/*
    �ļ�·����Ϣ�Ĵ���
*/
void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if (!(dir = opendir(path))) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        print_file_info(entry->d_name, &statbuf);
    }
    closedir(dir);
}
/*
    ���������������
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        list_directory("."); // Ĭ���г���ǰĿ¼
    } else {
        for (int i = 1; i < argc; i++) {
            list_directory(argv[i]);
        }
    }

    return 0;
}
