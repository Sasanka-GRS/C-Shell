#include "headers.h"

int maxHistorySize = 100000;
extern int historyNum;
extern int maxPathSize;
int maxHistNum = 20;
char hist[] = ".history.txt";
char histNum[] = ".histNum.txt";

// write abt deleting both files in readme - not just one

void historyPush(char in[], char home[])
{
    char delim[] = "\n";
    char *p = strtok(in, delim);
    if (p == NULL)
    {
        return;
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char current[maxPathSize];
    getcwd(current, maxPathSize);
    chdir(home);

    fp = fopen(hist, "rw");
    if (fp == NULL)
    {
        printf("\033[1;31mbash: history: ");
        printf("unable to fetch history");
        printf("\033[0m\n");
        return;
    }
    char local[21][maxPathSize];
    int count = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        strcpy(local[count], line);
        int l1 = strlen(local[count]);
        local[count][l1 - 1] = '\0';
        count++;
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }

    if (count == 0)
    {
        char temp[maxHistorySize];
        strcpy(temp, p);
        strcat(temp, "\n");
        int fd = open(hist, O_RDWR);
        write(fd, temp, strlen(temp));
        close(fd);
        return;
    }

    if (count < 20)
    {
        if (!strcmp(local[count - 1], p))
        {
            return;
        }
        int fg = open(hist, O_RDWR | O_APPEND);
        write(fg, p, strlen(p));
        write(fg, "\n", 1);
        return;
    }

    if (!strcmp(local[count - 1], p))
    {
        return;
    }
    int fgr = open(hist, O_RDWR | O_TRUNC);
    for (int i = 1; i < count; i++)
    {
        write(fgr, local[i], strlen(local[i]));
        write(fgr, "\n", 1);
    }
    write(fgr, p, strlen(p));
    write(fgr, "\n", 1);
    chdir(current);
}

void history()
{
    char delim[] = " \n\t";
    char *token = strtok(NULL, delim);
    int stdout1 = dup(1), stdin1 = dup(0), fd1;
    if (token != NULL)
    {
        if (!strcmp(token, ">"))
        {
            token = strtok(NULL, delim);
            fd1 = open(token, O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd1, 1);
        }
        else if (!strcmp(token, ">>"))
        {
            token = strtok(NULL, delim);
            fd1 = open(token, O_RDWR | O_CREAT | O_APPEND, 0644);
            dup2(fd1, 1);
        }
        else
        {
            dup2(stdout1, 1);
            printf("\033[1;31mpwd: ");
            printf("invalid command");
            printf("\033[0m\n");
        }
    }
    char buffer[maxHistorySize];
    int fd = open(hist, O_RDONLY);
    read(fd, buffer, maxHistorySize);
    close(fd);
    int l = strlen(buffer);
    char buf[l];
    strcpy(buf, buffer);

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(hist, "r");
    if (fp == NULL)
    {
        printf("\033[1;31mbash: history: ");
        printf("unable to fetch history");
        printf("\033[0m\n");
        return;
    }
    char local[21][maxPathSize];
    int count = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        strcpy(local[count], line);
        count++;
    }

    fclose(fp);
    if (line)
    {
        free(line);
    }

    if (count < 10)
    {
        printf("%s", buf);
    }
    else
    {
        for (int i = count - 10; i < count; i++)
        {
            printf("%s", local[i]);
        }
    }
    dup2(stdout1,1);
    dup2(stdin1,0);
}