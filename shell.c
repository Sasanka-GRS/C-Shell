#include "headers.h"
#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "history.h"
#include "execute.h"
#include "background.h"
#include "pinfo.h"
#include "discover.h"
#include "next.h"

// Not storing pids

int maxHostSize = 1000;
int maxHomeSize = 10000;
int maxPathSize = 20000;
int maxInputSize = 10000;

pid_t bgpCount[100000];
//char bgpName[100000][1000];
pid_t shell;
char *bgpComm[100000];
int bgp = 0;
char home[10000];
int totalTime = 0;
time_t begin = 0;
char display[20000];
char cwd[20000];
char prev[20000];
char *arguments[100];
char *args[100];
extern int errno;

int main(int argc, char *argv[])
{
    /*shell = getpid();
    pid_t main = fork();
    if (main == 0)
    {
        while (1)
        {
            for (int i = 0; i < bgp; i++)
            {
                int status;
                while (waitpid(bgpCount[i], &status, 0)==0)
                {
                }
                printf("%s with pid = %d exited %s\n", bgpComm[i], bgpCount[i], WEXITSTATUS(status) ? "abnormally" : "normally");
            }
            sleep(1);
        }
    }*/
    char *user = getlogin();
    char host[maxHostSize];
    gethostname(host, maxHostSize);

    int userLen = strlen(user), hostLen = strlen(host);
    strcpy(display, user);
    strcat(display, "@");
    strcat(display, host);

    char hist[] = ".history.txt";
    int fd = open(hist, O_RDWR | O_CREAT, 0600);
    close(fd);

    char home[maxHomeSize];
    getcwd(home, maxHomeSize);
    cwd[0] = '~';
    cwd[1] = '\0';
    prev[0] = '\0';

    // At startup
    begin = time(NULL);
    next("~",home,display,begin);
    int iter = 0;
    while (1)
    {
        //signal(SIGCHLD,backgroundFun);
        iter = 0;
        char delim[] = ";";
        char *arg1 = strtok(inputArgument, delim);
        if (arg1[0] == '\n')
        {
            next(cwd,home,display,begin);
            continue;
        }
        while (arg1 != NULL)
        {
            if (arg1[0] == '\n')
            {
                arg1 = strtok(NULL, delim);
                continue;
            }
            arguments[iter] = arg1;
            arg1 = strtok(NULL, delim);
            iter++;
        }
        char delim1[] = " \t\n";
        char delim2[] = "&";
        for (int i = 0; i < iter; i++)
        {
            begin = time(NULL);
            int count = 0;
            char temp[maxPathSize];
            strcpy(temp, arguments[i]);
            //printf("argument is %s\n",arguments[i]);
            char *tok = strtok(temp, delim2);
            while (tok != NULL)
            {
                args[count] = tok;
                count++;
                tok = strtok(NULL, delim2);
            }
            int flagAmp = 0;
            if (count == 1)
            {
                int l = strlen(arguments[i]);
                //printf("argument is %s\n",arguments[i]);
                for (int j = 0; j < l; j++)
                {
                    if (arguments[i][j] == '&')
                    {
                        flagAmp = 1;
                        break;
                    }
                }
            }
            else
            {
                flagAmp = 1;
            }
            //printf("flag& is %d\n",flagAmp);
            if (!flagAmp)
            {
                char *token = strtok(arguments[i], delim1);
                if (!strcmp(token, "cd"))
                {
                    cd(home);
                }
                else if (!strcmp(token, "pwd"))
                {
                    pwd();
                }
                else if (!strcmp(token, "echo"))
                {
                    echo();
                }
                else if (!strcmp(token, "ls"))
                {
                    ls(home);
                }
                else if (!strcmp(token, "history"))
                {
                    history();
                }
                else if(!strcmp(token,"pinfo"))
                {
                    pinfo(shell,home);
                }
                else if(!strcmp(token,"discover"))
                {
                    discover(home);
                }
                else
                {
                    if (token != NULL)
                    {
                        execute(token);
                    }
                }
            }
            else
            {
                // run background process here
                // 1. If it ends with &
                // 2. If it has foreground also
                background(arguments[i], count, home, cwd, display, shell);
            }
        }
        next(cwd,home,display,begin);
    }

    return 0;
}
