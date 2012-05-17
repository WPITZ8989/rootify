#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>

/* I haven't found any documentation regarding the standard way of
   getting the maximum length of user names so I can only hope that
   the following value is correct. */
#define USER_NAME_MAX 32
#define CONFIG_FILE "/etc/rootify.conf"
#define VERSION_NUMBER "0.2.3"

static char *program_name;

static void die(int error, char *message);
static void print_help();


static void die(int error, char *message)
{
    fprintf(stderr, "%s: %s\n", program_name, message);
    exit(error);
}

static void print_help()
{
    printf("rootify gives root privileges for the users who are listed"
           " in /etc/rootify.conf (one username per line)\n");
    printf("Usage: %s [options | program]\n", program_name);
    printf("Options: -h, --help : Print this help text.\n");
    printf("If program is specified then it will be executed with root privileges.\n");
    printf("If no program is specified then a root shell will be opened.\n");
    printf("rootify %s is released under the GPLv3, hosted at https://github.com/mondalaci/rootify\n",
           VERSION_NUMBER);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int authorized = 0;
    int i;
    char *args;
    uid_t my_uid;
    struct passwd *my_passwd;
    char *my_name;
    char name[USER_NAME_MAX + 1];

    program_name = *argv;
    long arg_max = sysconf(_SC_ARG_MAX);
    args = malloc(arg_max+1);
    strcpy(args, "");

    if (argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
            print_help();
            exit(0);
        }
    }

    /* authorize the user */

    fp = fopen(CONFIG_FILE, "r");

    if (!fp) {
        char message[128];
        sprintf(message, "Could not open the configuration file `%s'.", CONFIG_FILE);
        die(3, message);
    }

    my_uid = getuid();

    if (my_uid == 0) {
        authorized = 1;
    }

    my_passwd = getpwuid(my_uid);

    if (!my_passwd) {
        die(4, "Could not query your user name from the"
            " password database.");
    }

    my_name = my_passwd->pw_name;

    while (fscanf(fp, "%s", name)!=EOF && !authorized) {
        if (!strcmp(name, my_name)) {
            authorized = 1;
        }
    }

    fclose(fp);

    /* gain privilige */

    if (!authorized) {
        die(1, "You are not authorized to use this program.");
    }

    if (setuid(0)) {
        die(2, "Program binary is not SUID root.");
    }

    if (setgid(0)) {
        die(3, "Program binary is not SGID root.");
    }

    /* execute the desired program */

    if (argc == 1) {
        execl("/bin/bash", "bash", NULL);
    } else {
        execvp(argv[1], argv+1);
    }

    /* if we're here, exec didn't do well for some reason */
    perror(program_name);

    return 0;
}
