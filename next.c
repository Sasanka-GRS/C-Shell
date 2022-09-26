#include "headers.h"
#include "background.h"
#include "history.h"

void fill(char in[])
{
    int len = strlen(in);
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    char files[1000][1000];
    int yes[1000];
    int c = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            strcpy(files[c],dir->d_name);
            if(strlen(files[c])<len)
            {
                yes[c] = 0;
                c++;
                continue;
            }
            int i=0;
            for(i=0; i<len; i++)
            {
                if(in[i]!=files[c][i])
                {
                    yes[c] = i;
                    break;
                }
            }
            if(i==len)
            {
                yes[c] = i;
            }
            c++;
            //printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    for(int i=0; i<c; i++)
    {
        if(yes[c]<strlen(files[c]))
        {
            for(int j=yes[c]; j<strlen(files[c]); j++)
            {
                write(1,&files[c][j],1);
            }
            strcpy(inputArgument,files[c]);
        }
        printf("\n");
    }
}

void next(char currPath[], char home[], char display[], time_t begin)
{
    time_t end = time(NULL);
    int totalTime = end - begin;
    /*
    printf("\r<");
    printf("\033[0;32m\x1B[1m%s", display);
    printf("\033[0m:");
    printf("\033[0;34m\x1B[1m%s", currPath);
    printf("\033[0m\x1B[0m");
    if (totalTime > 1)
    {
        printf("took %ds", totalTime);
    }
    printf("> ");*/
    write(1, "\r<", 2);
    write(1, "\033[0;32m\x1B[1m", strlen("\033[0;32m\x1B[1m"));
    write(1, display, strlen(display));
    write(1, "\033[0m:", strlen("\033[0m:"));
    write(1, "\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
    write(1, currPath, strlen(currPath));
    write(1, "\033[0m\x1B[0m", strlen("\033[0m\x1B[0m"));
    if (totalTime > 1)
    {
        char temp[100];
        sprintf(temp, "took%ds", totalTime);
        write(1, temp, strlen(temp));
    }
    write(1, "> ", 2);
    for (int i = 0; i < 20000; i++)
    {
        inputArgument[i] = '\0';
    }
    /*if(flagGlobal)
    {
        flagGlobal = 0;
        return;
    }*/
    
    size_t b = 10000;
    char* buf = NULL;
    buf = (char*) malloc(b*sizeof(char));
    int len = getline(&buf,&b, stdin);
    strcpy(inputArgument,buf);
    /*
    char buf[20000];
    int flagTab = 0;
    for (int i = 0; i < 20000; i++)
    {
        buf[i] = '\0';
    }
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (c == '\0')
    {
        write(1, "\n", 1);
        exit(0);
    }
    while (c != '\n')
    {
        char c1[2];
        c1[0] = c;
        c1[1] = '\0';
        strcat(buf, c1);
        read(STDIN_FILENO, &c, 1);
        write(1,"it is",5);
        write(1,c1,2);  
        if (c == '\t')
        {
            printf("was here\n");
            fill(buf);
        }
    }
    //printf("here\n");
    strcpy(inputArgument, buf);
    strcat(inputArgument,"\n");
    int len = strlen(inputArgument);*/
    //printf("here\n");
    // printf("in is %s\n", inputArgument);
    // printf("%s", inputArgument);
    // scanf("%[^\n]%*c", inputArgument);
    if (!len)
    {
        exit(0);
    }
    while (!len)
    {
        printf("\n");
        // exit(0);
        write(1, "\r<", strlen("\r<"));
        write(1, "\033[0;32m\x1B[1m", strlen("\033[0;32m\x1B[1m"));
        write(1, display, strlen(display));
        write(1, "\033[0m:", strlen("\033[0m:"));
        write(1, "\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
        write(1, currPath, strlen(currPath));
        write(1, "\033[0m\x1B[0m", strlen("\033[0m\x1B[0m"));
        write(1, "> ", strlen("> "));
        scanf("%[^\n]%*c", inputArgument);
    }
    //printf("here\n");
    historyPush(inputArgument, home);
    //printf("here\n");
    signal(SIGCHLD, backgroundFun);
}