#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>

int is_a = 0;
int is_l = 0;
int is_r = 0;

void print_perms(mode_t st) {
    char perms[11];
    if (st && S_ISREG(st)) perms[0] = '-';
    else if (st && S_ISDIR(st)) perms[0] = 'd';
    else if (st && S_ISFIFO(st)) perms[0] = '|';
    else if (st && S_ISLNK(st)) perms[0] = 's';
    else if (st && S_ISCHR(st)) perms[0] = 'c';
    else if (st && S_ISBLK(st)) perms[0] = 'b';
    else perms[0] = 'l';  // S_ISLNK
    perms[1] = (st && S_IRUSR) ? 'r' : '-';
    perms[2] = (st && S_IWUSR) ? 'w' : '-';
    perms[3] = (st && S_IXUSR) ? 'x' : '-';
    perms[4] = (st && S_IRGRP) ? 'r' : '-';
    perms[5] = (st && S_IWGRP) ? 'w' : '-';
    perms[6] = (st && S_IXGRP) ? 'x' : '-';
    perms[7] = (st && S_IROTH) ? 'r' : '-';
    perms[8] = (st && S_IWOTH) ? 'w' : '-';
    perms[9] = (st && S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
    printf("%s", perms);
}

void init_flag(const char flag) {
    if (flag == 'a') {
        is_a = 1;
    } else if (flag == 'l') {
        is_l = 1;
    } else if (flag == 'R') {
        is_r = 1;
    } else {
        printf("Unknown flag: %c. Will ignore it.\n", flag);
    }
}

void init_flags(const char *flags) {
    char p;
    int i = 0;
    p = flags[i];
    while (p != 0) {
        p = flags[++i];
        if (p != 0) {
            init_flag(p);
        }
    }
}

int is_flags(const char arg[]) {
    if (arg[0] == '-') {
        return 1;
    } else {
        return 0;
    }
}

void print_l(struct dirent *file) {
    struct stat sbuf;
    char buf[128];
    struct passwd pwent, *pwentp;
    struct group grp, *grpt;
    char datestring[256];
    struct tm time;

    if (stat(file->d_name, &sbuf) < 0) {
        puts(file->d_name);
        printf("stat error: %s\n", file->d_name);
        exit(1);
    }
    print_perms(sbuf.st_mode);
    printf(" %d", (int) sbuf.st_nlink);
    if (!getpwuid_r(sbuf.st_uid, &pwent, buf, sizeof(buf), &pwentp))
        printf(" %s", pwent.pw_name);
    else
        printf(" %d", sbuf.st_uid);

    if (!getgrgid_r(sbuf.st_gid, &grp, buf, sizeof(buf), &grpt))
        printf(" %s", grp.gr_name);
    else
        printf(" %d", sbuf.st_gid);
    printf(" %5d", (int) sbuf.st_size);

    localtime_r(&sbuf.st_mtime, &time);
    /* Get localized date string. */
    strftime(datestring, sizeof(datestring), "%F %T", &time);

    printf(" %s %s\n", datestring, file->d_name);
}

void ls(char *path) {
    DIR *dir;
    struct dirent *file;
    dir = opendir(path);
    while (file = readdir(dir)) {
        if (!is_a && file->d_name[0] == '.') {
            continue;
        }
        if (is_l) {
            print_l(file);
        } else {
            printf("%s  ", file->d_name);
        }
    }
    free(file);
    free(dir);
}

void ls_r(char path[]) {
    DIR *dir;
    struct dirent *file;
    struct stat sbuf;
    char tmp[128];
    dir = opendir(path);
    if (dir == NULL) {
        puts(path);
        printf("opendir error: %s\n", path);
        perror("opendir error");
        exit(1);
    }
    while (file = readdir(dir)) {
        if (file->d_name[0] == '.') continue;
        strcpy(tmp, path);
        strcat(tmp, "/");
        strcat(tmp, file->d_name);
        //printf("%s\n", tmp);
        if (stat(tmp, &sbuf) < 0) {
            puts(tmp);
            printf("stat error: %s\n", tmp);
            perror("stat error");
            exit(1);
        }
        if (sbuf.st_mode && S_ISDIR(sbuf.st_mode)) {
            ls(path);
            printf("\n%s:\n", tmp);
            ls_r(tmp);
        }
    }
    free(file);
    free(dir);
}

int main(int argc, char *argv[]) {
    char pathname[128];
    getcwd(pathname, 128);
    if (argc == 1) { // ls
        ls(pathname);
    } else if (argc == 2) { // ls some
        if (is_flags(argv[1])) {
            init_flags(argv[1]);
            if (is_r) {
                ls_r(pathname);
            } else {
                ls(pathname);
            }
        } else {
            if (is_r) {
                ls_r(argv[1]);
            } else {
                ls(argv[1]);
            }
        }
    } else { // ls some some...
        int i = 1;
        if (is_flags(argv[1])) {
            init_flags(argv[1]);
            i = 2;
        }
        for (; i < argc; i++) {
            if (is_r) {
                ls_r(argv[i]);
            } else {
                printf("\n%s:\n", argv[i]);
                ls(argv[i]);
            }
        }
    }
    printf("\n");
    return 0;
}
