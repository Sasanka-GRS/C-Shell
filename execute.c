#include "headers.h"

extern time_t begin;
extern pid_t fore;
extern char foregroundName[];
extern char *foregroundArgs[];
extern void sigtHandler();

void execute(char *token)
{
    char delim1[] = " \t\n";
    int l = strlen(token);
    char *comm[100];
    comm[0] = token;
    int c = 0;
    char *argExec[100][100];
    // printf("argExec[0][0] is %s\n",argExec[0][0]);
    char *argPipe[100];
    char *pathIn[100], *pathOut[100];
    for (int i = 0; i < 100; i++)
    {
        pathIn[i] = NULL;
        pathOut[i] = NULL;
    }
    int flagg[100], flaggg[100], flagl[100];
    for (int i = 0; i < 100; i++)
    {
        flagg[i] = 0;
        flaggg[i] = 0;
        flagl[i] = 0;
    }
    // char pathIn[10000], pathOut[10000];
    pathIn[0] = '\0';
    pathOut[0] = '\0';
    int pip = 0;
    // char first[1000];
    // strcpy(first,comm[0]);
    // printf("comm[0] is %s\n",comm[0]);
    while (token != NULL)
    {
        if (flagg[pip] == 0 && !strcmp(token, ">"))
        {
            flagg[pip] = 1;
            token = strtok(NULL, delim1);
            if (token == NULL)
            {
                printf("\033[1;31mno output path detected");
                printf("\033[0m\n");
                return;
            }
            pathOut[pip] = token;
            token = strtok(NULL,delim1);
            // strcpy(pathOut, token);
            continue;
        }
        if (flaggg[pip] == 0 && !strcmp(token, ">>"))
        {
            flaggg[pip] = 1;
            token = strtok(NULL, delim1);
            if (token == NULL)
            {
                printf("\033[1;31mno output path detected");
                printf("\033[0m\n");
                return;
            }
            pathOut[pip] = token;
            token = strtok(NULL,delim1);
            continue;
        }
        if (flagl[pip] == 0 && !strcmp(token, "<"))
        {
            flagl[pip] = 1;
            token = strtok(NULL, delim1);
            if (token == NULL)
            {
                printf("\033[1;31mno input path detected");
                printf("\033[0m\n");
                return;
            }
            pathIn[pip] = token;
            continue;
        }
        if (!strcmp(token, "|"))
        {
            token = strtok(NULL, delim1);
            if (token == NULL)
            {
                printf("\033[1;31mno output pipe end detected");
                printf("\033[0m\n");
                return;
            }
            // argPipe[pip] = token;
            argExec[pip][c] = NULL;
            pip++;
            c = 0;
            // printf("token is %s\n", token);
            comm[pip] = token;
            // printf("comm[pip] is %s\n",comm[pip]);
            //  token = strtok(NULL, delim1);
            continue;
        }
        // printf("token is %s\n",token);
        argExec[pip][c] = token;
        c++;
        token = strtok(NULL, delim1);
    }
    // printf("pathIn and pathOut are %s, %s\n", pathIn, pathOut);
    //  /flagl = 1 - flagl;
    argExec[c][pip] = NULL;
    // strcpy(comm[0],first);
    /*
    for(int i=0; i<=pip; i++)
    {
        printf("comm now is %s\n",comm[i]);
    }*/
    /*
    for(int i=0; i<=pip; i++)
    {
        int j=0;
        printf("arguments are\n\n");
        while(argExec[i][j])
        {
            printf("%s ", argExec[i][j]);
            j++;
        }
        //printf("j is %d\n",j);
        printf("\n");
    }*/
    // --------------------------------------- start from here --------------------------------------------
    if (pip == 0)
    {
        pid_t shell, child;
        int statusChild = 0;
        shell = getpid();
        child = fork();
        begin = time(NULL);
        int stdout1 = dup(1), stdin1 = dup(0);
        if (child < 0)
        {
            printf("\033[1;31mfork() failure: ");
            int err = errno;
            printf("%s", strerror(err));
            printf("\033[0m\n");
            return;
        }
        if (child == 0)
        {
            int fdO, fdI;
            // printf("pathIn and pathOut are %s, %s\n", pathIn, pathOut);
            if (flagg[0])
            {
                fdO = open(pathOut[0], O_RDWR | O_CREAT | O_TRUNC, 0644);
                dup2(fdO, 1);
                // printf("Yaay");
            }
            if (flaggg[0])
            {
                fdO = open(pathOut[0], O_RDWR | O_CREAT | O_APPEND, 0644);
                dup2(fdO, 1);
            }
            if (flagl[0])
            {
                // printf("Yaay");
                fdI = open(pathIn[0], O_RDONLY);
                if (fdI < 0)
                {
                    dup2(stdout1, 1);
                    // printf("here\n");
                    printf("\033[1;31m%s: %s: ", comm[0], pathIn[0]);
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                    return;
                }
                dup2(fdI, 0);
            }
            // write(stdout1,argExec[0][1],strlen(argExec[0][1]));
            // write(stdout1,"\n",1);
            // argExec[0][1] = NULL;
            int iter = 0;
            while (argExec[0][iter])
            {
                foregroundArgs[iter] = argExec[0][iter];
                iter++;
            }
            foregroundArgs[iter] = NULL;
            strcpy(foregroundName, comm[0]);
            int h = execvp(comm[0], argExec[0]);
            if (h < 0)
            {
                dup2(stdout1, 1);
                printf("\033[1;31m%s: ", comm[0]);
                printf("invalid command");
                printf("\033[0m\n");
                exit(0);
            }
            exit(1);
        }
        else
        {
            fore = child;
            while (waitpid(child, &statusChild, 0) == 0)
            {
            }
        }
        fore = -1;
        dup2(stdout1, 1);
        dup2(stdin1, 0);
    }
    else
    {
        int pipe1[2], pipe2[2];
        if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
        {
            printf("\033[1;31mpipe() failure: ");
            int err = errno;
            printf("%s", strerror(err));
            printf("\033[0m\n");
        }
        //int in = pipe1[0], out = pipe1[1];
        // printf("initial pipe stdin = %d and pipe stdout = %d\n",pipe1[0],pipe1[1]);
        int stdout1 = dup(1), stdin1 = dup(0);
        // printf("initial stdin = %d and stdout = %d\n",dup(0),dup(1));
        int t1 = open(".tmp1.txt", O_RDWR | O_CREAT, 0777);
        // int t2 = open("tmp2.txt", O_RDWR | O_CREAT, 0777);
        // printf("t1 is %d\nt2 is %d\n", t1, t1);
        for (int i = 0; i <= pip; i++)
        {
            //printf("i is %d\n",i);
            //printf("was here\n");
            /*
            if (!flagl[i] && i != 0)
            {
                // printf("was in first when i = %d\n",i);
                dup2(pipe1[0], 0);
                dup2(pipe1[1], 1);
            }
            if (!(flagg[i] || flaggg[i]) && i != pip)
            {
                // printf("was in second when i = %d\n",i);
                dup2(pipe1[0], 0);
                dup2(pipe1[1], 1);
            }*/
            /*
         if (!strcmp(comm[i], "wc"))
         {
             char buf[1000];
             read(0, buf, 1000);
             write(t1, buf, strlen(buf));
             // write(stdout1,"here\n",5);
             char *e = ".tmp1.txt";
             argExec[i][1] = e;
             // write(stdout1,"here\n",5);
             argExec[i][2] = NULL;
         }*/
            /*
            if(i)
            {
                char buf[1000];
                read(0,buf,1000);
                dup2(stdout1,1);
                printf("read is - %s\n",buf);
                dup2(out, 1);
            }*/
            /*
            char buff[1000];
            sprintf(buff,"current stdin = %d and stdout = %d\n",dup(0),dup(1));
            write(stdout1,buff,strlen(buff));*/
            pid_t shell, child;
            int statusChild = 0;
            shell = getpid();
            child = fork();
            begin = time(NULL);

            // printf("here\n");
            if (child < 0)
            {
                printf("\033[1;31mfork() failure: ");
                int err = errno;
                printf("%s", strerror(err));
                printf("\033[0m\n");
                return;
            }
            if (child == 0)
            {
                if (i == 0)
                {
                    dup2(pipe2[1], 1);
                }
                else
                {
                    if (i == pip)
                    {
                        if (!i % 2)
                        {
                            dup2(pipe1[0], 0);
                        }
                        else
                        {
                            dup2(pipe2[0], 0);
                        }
                    }
                    else
                    {
                        if (!i % 2)
                        {
                            dup2(pipe1[0], 0);
                            dup2(pipe2[1], 1);
                        }
                        else
                        {
                            dup2(pipe2[0], 0);
                            dup2(pipe1[1], 1);
                        }
                    }
                }
                int fdO, fdI;
                // printf("pathIn and pathOut are %s, %s\n", pathIn, pathOut);
                if (flagg[i])
                {
                    fdO = open(pathOut[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
                    dup2(fdO, 1);
                    // printf("Yaay");
                }
                if (flaggg[i])
                {
                    fdO = open(pathOut[i], O_RDWR | O_CREAT | O_APPEND, 0644);
                    dup2(fdO, 1);
                }
                if (flagl[i])
                {
                    // printf("Yaay");
                    fdI = open(pathIn[i], O_RDONLY);
                    if (fdI < 0)
                    {
                        dup2(stdout1, 1);
                        // printf("here\n");
                        printf("\033[1;31m%s: %s: ", comm[i], pathIn[i]);
                        int err = errno;
                        printf("%s", strerror(err));
                        printf("\033[0m\n");
                        return;
                    }
                    dup2(fdI, 0);
                }
                // write(stdout1,argExec[i][3],strlen(argExec[i][3]));
                // write(stdout1,"\n",1);
                int h = execvp(comm[i], argExec[i]);
                if (h < 0)
                {
                    dup2(stdout1, 1);
                    printf("\033[1;31m%s: ", comm[i]);
                    printf("invalid command");
                    printf("\033[0m\n");
                    exit(0);
                }
                exit(1);
            }
            else
            {
                //printf("here\n");
                while (waitpid(child, &statusChild, 0) == 0)
                {
                }
                if (i == 0)
                {
                    close(pipe2[1]);
                }
                else
                {
                    if (i == pip)
                    {
                        if (!i % 2)
                        {
                            close(pipe1[0]);
                        }
                        else
                        {
                            close(pipe2[0]);
                        }
                    }
                    else
                    {
                        if (!i % 2)
                        {
                            close(pipe1[0]);
                            close(pipe2[1]);
                        }
                        else
                        {
                            close(pipe2[0]);
                            close(pipe1[1]);
                        }
                    }
                }
            }
            //dup2(stdout1, 1);
            //dup2(stdin1, 0);
            // printf("here\n");
            /*
            char bffer[1000];
            printf("t1 is %d\nt2 is %d\n", t1, t2);
            int flagRead = read(t2, bffer, 1000);
            printf("flagRead is %d\n", flagRead);
            if (flagRead < 0)
            {
                printf("Error occured\n");
                return;
            }
            printf("buffer is \n%s\n", bffer);
            write(t1, bffer, strlen(bffer));*/
        }
        // remove(".tmp1.txt");
        //  remove(".tmp2.txt");
    }
}