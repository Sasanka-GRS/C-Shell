#include "headers.h"

extern int maxPathSize;
char *data[10000];
char disPath[10000][100000];
char *file = NULL, *folder = NULL;

void stackIt(char in[], int flagd, int flagf, char home[])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(in);
    int iter = 0;
    char *data[1000];
    if (d)
    {
        if(flagd)
        {
            printf("%s\n",in);
        }
        while ((dir = readdir(d)) != NULL)
        {
            data[iter] = dir->d_name;
            iter++;
        }
        int isdir[iter];
        memset(isdir, 0, sizeof(isdir));
        for (int i = 0; i < iter; i++)
        {
            //printf("data items in the folder are %s\n",data[i]);
            int flagDirIn = 0;
            int lsOpenIn = chdir(data[i]);
            if (lsOpenIn == 0)
            {
                flagDirIn = 1;
            }
            chdir(in);
            isdir[i] = flagDirIn;
        }
        //printf("iter is %d\n", iter);
        for (int i = 0; i < iter; i++)
        {
            if (!isdir[i])
            {
                if (flagf)
                {
                    int yes = 0;
                    char s[maxPathSize];
                    int l = strlen(home);
                    char buf[maxPathSize];
                    strcpy(buf, in);
                    strncpy(s, in, l);
                    s[l] = '\0';
                    if (!strcmp(home, s))
                    {
                        yes = 1;
                    }
                    if (yes)
                    {
                        buf[0] = '~';
                        buf[1] = '\0';
                        int l1 = strlen(in), count = 0, count1 = 0;
                        for (int j = l; j < l1; j++)
                        {
                            buf[j - l + 1] = in[j];
                        }
                        buf[l1 - l + 1] = '\0';
                    }
                    printf("%s/%s\n", buf, data[i]);
                }
            }

            else
            {
                if (flagd)
                {
                    //printf("data[i] is %s\n",data[i]);
                    if (!strcmp(data[i], ".") || !strcmp(data[i], ".."))
                    {
                        continue;
                    }
                    int yes = 0;
                    char s[maxPathSize];
                    int l = strlen(home);
                    char buf[maxPathSize];
                    strcpy(buf, in);
                    strncpy(s, in, l);
                    s[l] = '\0';
                    if (!strcmp(home, s))
                    {
                        yes = 1;
                    }
                    if (yes)
                    {
                        buf[0] = '~';
                        buf[1] = '\0';
                        int l1 = strlen(in), count = 0, count1 = 0;
                        for (int j = l; j < l1; j++)
                        {
                            buf[j - l + 1] = in[j];
                        }
                        buf[l1 - l + 1] = '\0';
                    }
                    printf("%s/%s\n", buf, data[i]);
                }
                if (!strcmp(data[i], ".") || !strcmp(data[i], ".."))
                {
                    continue;
                }
                stackIt(data[i], flagd, flagf, home);
            }
        }
    }
}

void stackItVariant(char in[], int flagd, int flagf, char file[], char home[])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(in);
    int iter = 0;
    char *data[10000];
    if (d)
    {
        
        while ((dir = readdir(d)) != NULL)
        {
            data[iter] = dir->d_name;
            iter++;
        }
        int isdir[iter];
        memset(isdir, 0, sizeof(isdir));
        for (int i = 0; i < iter; i++)
        {
            //printf("data items in the folder are %s\n",data[i]);
            int flagDirIn = 0;
            int lsOpenIn = chdir(data[i]);
            if (lsOpenIn == 0)
            {
                flagDirIn = 1;
            }
            chdir(in);
            isdir[i] = flagDirIn;
        }
        for (int i = 0; i < iter; i++)
        {
            if (!isdir[i])
            {
                if (flagf && !strcmp(data[i], file))
                {
                    int yes = 0;
                    char s[maxPathSize];
                    int l = strlen(home);
                    char buf[maxPathSize];
                    strcpy(buf, in);
                    strncpy(s, in, l);
                    s[l] = '\0';
                    if (!strcmp(home, s))
                    {
                        yes = 1;
                    }
                    if (yes)
                    {
                        buf[0] = '~';
                        buf[1] = '\0';
                        int l1 = strlen(in), count = 0, count1 = 0;
                        for (int j = l; j < l1; j++)
                        {
                            buf[j - l + 1] = in[j];
                        }
                        buf[l1 - l + 1] = '\0';
                    }
                    printf("%s/%s\n", buf, data[i]);
                }
            }
            else
            { // Wont work for searching "."
                if (flagd && !strcmp(data[i], file))
                {
                    if(!strcmp(data[i],".") || !strcmp(data[i],".."))
                    {
                        continue;
                    }
                    int yes = 0;
                    char s[maxPathSize];
                    int l = strlen(home);
                    char buf[maxPathSize];
                    strcpy(buf, in);
                    strncpy(s, in, l);
                    s[l] = '\0';
                    if (!strcmp(home, s))
                    {
                        yes = 1;
                    }
                    if (yes)
                    {
                        buf[0] = '~';
                        buf[1] = '\0';
                        int l1 = strlen(in), count = 0, count1 = 0;
                        for (int j = l; j < l1; j++)
                        {
                            buf[j - l + 1] = in[j];
                        }
                        buf[l1 - l + 1] = '\0';
                    }
                    printf("%s/%s\n", buf, data[i]);
                }
                if (!strcmp(data[i], ".") || !strcmp(data[i], ".."))
                {
                    continue;
                }
                stackItVariant(data[i], flagd, flagf, file, home);
            }
        }
    }
}

void discover(char home[])
{
    char delim[] = " \n\t";
    char *token = strtok(NULL, delim);
    int flagf = 0, flagd = 0, flagAll = 0, flagAllF = 0, flagAllD = 0;
    int count = 0;
    char cur[maxPathSize];
    int stdout1 = dup(1), stdin1 = dup(0);
    //file[0] = '\0';

    getcwd(cur, maxPathSize);
    folder = ".";
    if (token == NULL)
    {
        flagd = 1;
        flagf = 1;
    }
    while (token != NULL)
    {
        if (!strcmp(token, "-d"))
        {
            flagd = 1;
        }
        else if (!strcmp(token, "-f"))
        {
            flagf = 1;
        }
        else if (token[0] == '"')
        {
            file = token;
        }
        else if(!strcmp(token,">"))
        {
            token = strtok(NULL,delim);
            int fdO = open(token, O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fdO, 1);
            break;
        }
        else if(!strcmp(token,">>"))
        {
            token = strtok(NULL,delim);
            int fdO = open(token, O_RDWR | O_CREAT | O_APPEND, 0644);
            dup2(fdO, 1);
            break;
        }
        else
        {
            folder = token;
            if (folder[0] == '~')
            {
                int l1 = strlen(folder), l2 = strlen(home);
                //char* temp = lsPath[i];
                //printf("l1 is %d l2 is %d\n",l1,l2);
                char temp1[l2 + l1];
                strcpy(temp1, home);
                for (int j = l2; j < l2 + l1 - 1; j++)
                {
                    temp1[j] = folder[j - l2 + 1];
                }
                temp1[l2 + l1 - 1] = '\0';
                strcpy(folder, temp1);
            }
        }
        token = strtok(NULL, delim);
    }
    if(!flagd^flagf)
    {
        flagd = 1;
        flagf = 1;
    }
    //printf("folder is %s\n",folder);
    if (file == NULL)
    {
        stackIt(folder, flagd, flagf, home);
    }
    else
    {
        char* file1 = strtok(file,"\"");
        stackItVariant(folder, flagd, flagf, file1, home);
    }
    chdir(cur);
    dup2(stdout1, 1);
    dup2(stdin1, 0);
}