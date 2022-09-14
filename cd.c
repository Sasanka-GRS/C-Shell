#include "headers.h"

extern int maxPathSize;
extern char home[];
extern char cwd[];
extern char prev[];
extern int errno;

void homeToTilda(int select, char home[])
{
    if (select == 0)
    {
        // change cwd
        // check if relative home is a part of it or not
        int yes = 0;
        char s[maxPathSize];
        int l = strlen(home);
        strncpy(s, cwd, l);
        s[l] = '\0';
        if (!strcmp(home, s))
        {
            yes = 1;
        }
        if (yes)
        {
            char buf[maxPathSize];
            buf[0] = '~';
            buf[1] = '\0';
            int l1 = strlen(cwd), count = 0, count1 = 0;
            for (int i = l; i < l1; i++)
            {
                buf[i - l + 1] = cwd[i];
            }
            buf[l1 - l + 1] = '\0';
            strcpy(cwd, buf);
        }
    }
    else
    {
        // change prev
        // check if relative home is a part of it or not
        int yes = 0;
        char s[maxPathSize];
        int l = strlen(home);
        strncpy(s, prev, l);
        s[l] = '\0';
        if (!strcmp(home, s))
        {
            yes = 1;
        }
        if (yes)
        {
            char buf[maxPathSize];
            buf[0] = '~';
            buf[1] = '\0';
            int l1 = strlen(prev), count = 0, count1 = 0;
            for (int i = l; i < l1; i++)
            {
                buf[i - l + 1] = prev[i];
            }
            buf[l1 - l + 1] = '\0';
            strcpy(prev, buf);
        }
    }
}

void cd(char home[])
{
    char delim[] = " \n\t";
    char *token = strtok(NULL, delim);
    if (token == NULL)
    {
        getcwd(prev, maxPathSize);
        homeToTilda(1, home);
        cwd[0] = '~';
        cwd[1] = '\0';
        chdir(home);
        return;
    }
    else
    {
        char *tok = strtok(NULL, delim);
        if (tok != NULL)
        {
            printf("\033[1;31mbash: cd: too many arguments");
            printf("\033[0m \n");
            return;
        }
        if (!strcmp(token, "."))
        {
            getcwd(prev, maxPathSize);
            homeToTilda(1, home);
            return;
        }
        else if (!strcmp(token, ".."))
        {
            char buf[maxPathSize];
            getcwd(buf, maxPathSize);
            int flag = 0, l = strlen(home), l1 = strlen(buf);
            if (l != l1)
            {
                flag = 0;
            }
            else
            {
                char s[maxPathSize];
                strncpy(s, buf, l);
                s[l] = '\0';
                if (!strcmp(home, s))
                {
                    flag = 1;
                }
            }
            //printf("buf is %s\n",buf);
            if (flag)
            {
                printf("%s\n", home);
                chdir("..");
                getcwd(cwd,maxPathSize);
                homeToTilda(0,home);
                //printf("cwd is %s\n",cwd);
                prev[0] = '~';
                prev[1] = '\0';
                return;
            }
            else
            {
                getcwd(prev, maxPathSize);
                homeToTilda(1, home);
                chdir("..");
                getcwd(cwd, maxPathSize);
                homeToTilda(0, home);
            }
            return;
        }
        else if (!strcmp(token, "-"))
        {
            if (prev[0] == '\0')
            {
                printf("\033[1;31mbash: cd: OLDPWD not set");
                printf("\033[0m\n");
                return;
            }
            else
            {
                strcpy(cwd, prev);
                getcwd(prev, maxPathSize);
                homeToTilda(1, home);
                if (cwd[0] == '~')
                {
                    char buf[maxPathSize];
                    int l = strlen(home);
                    strcpy(buf, home);
                    char *buf1 = cwd + 1;
                    strcat(buf, buf1);
                    chdir(buf);
                    return;
                }
                chdir(cwd);
            }
        }
        else if (!strcmp(token, "~"))
        {
            getcwd(prev, maxPathSize);
            homeToTilda(1, home);
            chdir(home);
            cwd[0] = '~';
            cwd[1] = '\0';
            return;
        }
        else
        {
            if (token[0] == '~')
            {
                char buf[maxPathSize];
                strcpy(buf, home);
                char *buf1 = token + 1;
                strcat(buf, buf1);
                getcwd(prev, maxPathSize);
                homeToTilda(1, home);
                int flagC = chdir(buf);
                if (flagC < 0)
                {
                    printf("\033[1;31mbash: cd: %s: ", token);
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                }
                getcwd(cwd, maxPathSize);
                homeToTilda(0, home);
                return;
            }
            else if (token[0] == '.')
            {
                getcwd(prev, maxPathSize);
                homeToTilda(1, home);
                int flagC = chdir(token);
                if (flagC < 0)
                {
                    printf("\033[1;31mbash: cd: %s: ", token);
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                }
                getcwd(cwd, maxPathSize);
                homeToTilda(0, home);
            }
            else
            {
                getcwd(prev, maxPathSize);
                homeToTilda(1, home);
                
                int flagC = chdir(token);
                if (flagC < 0)
                {
                    printf("\033[1;31mbash: cd: %s: ", token);
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                }
                getcwd(cwd, maxPathSize);
                homeToTilda(0, home);
                //printf("cwd is %s\n",cwd);
            }
        }
    }
}