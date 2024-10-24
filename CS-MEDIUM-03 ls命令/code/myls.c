#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> // 可能不是必需的，但符合POSIX标准
#include <time.h>
/*
    输出文件信息
*/
void print_file_info(const char *name, struct stat *statbuf) {
    printf("%s\t", name);  //文件名
    printf("%ld\t", statbuf->st_size); // 文件大小
    printf("%o\t", statbuf->st_mode & 0777); // 文件权限（仅显示最后三位）
    // 最后修改时间
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", localtime(&statbuf->st_mtime));
    printf("%s\n", timebuf);
}
/*
    文件路径信息的处理
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
    程序的主函数部分
*/
int main(int argc, char *argv[]) {
    if (argc < 2) {
        list_directory("."); // 默认列出当前目录
    } else {
        for (int i = 1; i < argc; i++) {
            list_directory(argv[i]);
        }
    }

    return 0;
}
