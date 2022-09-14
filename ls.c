#include "headers.h"

extern int maxPathSize;
int maxfileNameSize = 10000;
char *data[10000];
char lsPath[10000][100000];

void tildaToHome(int l, char home[])
{
    //printf("home is %s\n",home);
    for (int i = 0; i < l; i++)
    {
        if (lsPath[i][0] == '~')
        {
            int l1 = strlen(lsPath[i]), l2 = strlen(home);
            //char* temp = lsPath[i];
            //printf("l1 is %d l2 is %d\n",l1,l2);
            char temp1[l2 + l1];
            strcpy(temp1, home);
            for (int j = l2; j < l2 + l1 - 1; j++)
            {
                temp1[j] = lsPath[i][j - l2 + 1];
            }
            temp1[l2 + l1 - 1] = '\0';
            strcpy(lsPath[i], temp1);
        }
    }
}

void lexi(int n)
{
    int i, j;
    char temp[maxfileNameSize];

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (strcmp(data[i], data[j]) > 0)
            {
                strcpy(temp, data[i]);
                strcpy(data[i], data[j]);
                strcpy(data[j], temp);
            }
        }
    }
}

void lexiPath(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (strcmp(lsPath[i], lsPath[j]) > 0)
            {

                int l = strlen(lsPath[i]), l1 = strlen(lsPath[j]);
                char temp[l];
                strcpy(temp, lsPath[i]);
                char temp1[l1];
                strcpy(temp1, lsPath[j]);
                strcpy(lsPath[i], temp1);
                strcpy(lsPath[j], temp);
            }
        }
    }
}

void minusL(int select, char in[], int flag)
{
    if (select == 0)
    {
        // It is a file
        int len = strlen(in);
        char temp[len];
        strcpy(temp, in);
        int fdf = open(temp, O_RDONLY, 0600);
        if (fdf < 0)
        {
            printf("\033[1;31mls: cannot access '%s': No such file or directory", in);
            printf("\033[0m\n");
            return;
        }
        struct stat statNew;
        int flagStatNew = stat(in, &statNew);

        if (flagStatNew < 0)
        {
            printf("\033[1;31mE: unable to obtain permissions of new file");
            printf("\033[0m\n");
            return;
        }

        // Permissions of file
        if (!flagStatNew)
        {
            printf("-");
            if (statNew.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            // -------------------------------------------------------------
            //
            printf("%lu ", statNew.st_nlink);
            // -------------------------------------------------------------
            struct passwd *pwd;
            pwd = getpwuid(statNew.st_gid);
            printf("%s ", pwd->pw_name);
            // -------------------------------------------------------------
            struct group *grp;
            grp = getgrgid(statNew.st_gid);
            printf("%s ", grp->gr_name);
            // -------------------------------------------------------------
            int size = lseek(fdf, 0, SEEK_END);
            printf("%d ", size);
            // -------------------------------------------------------------
            char *s = ctime(&statNew.st_mtime);
            for (int i = 4; i <= 15; i++)
            {
                printf("%c", s[i]);
            }
            printf(" ");
            // -------------------------------------------------------------
            printf("%s\n", in);
        }
    }
    else if (select == 1)
    {
        // It is a folder
        //printf("In is %s\n",in);
        int len = strlen(in);
        char temp[len];
        strcpy(temp, in);
        struct stat statNew;
        int flagStatNew = stat(temp, &statNew);

        if (flagStatNew < 0)
        {
            printf("\033[1;31mE: unable to obtain permissions of new file");
            printf("\033[0m\n");
            return;
        }

        // Permissions of file
        if (!flagStatNew)
        {
            printf("-");
            if (statNew.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            // -------------------------------------------------------------
            // Do this separately -> links
            printf("%lu ", statNew.st_nlink);
            // -------------------------------------------------------------
            struct passwd *pwd;
            pwd = getpwuid(statNew.st_gid);
            printf("%s ", pwd->pw_name);
            // -------------------------------------------------------------
            struct group *grp;
            grp = getgrgid(statNew.st_gid);
            printf("%s ", grp->gr_name);
            // -------------------------------------------------------------
            printf("4096 ");
            // -------------------------------------------------------------
            char *s = ctime(&statNew.st_mtime);
            for (int i = 4; i <= 15; i++)
            {
                printf("%c", s[i]);
            }
            printf(" ");
            //printf("\nIn is %s\n",in);
            // -------------------------------------------------------------
            printf("\033[0;34m%s", temp);
            printf("\033[0m\n");
        }
    }
    else
    {
        // It is .out file
        int fdf = open(in, O_RDONLY, 0600);
        if (fdf < 0)
        {
            printf("\033[1;31mls: cannot access '%s': No such file or directory", in);
            printf("\033[0m\n");
            return;
        }
        struct stat statNew;
        int flagStatNew = stat(in, &statNew);

        if (flagStatNew < 0)
        {
            printf("\033[1;31mE: unable to obtain permissions of new file");
            printf("\033[0m\n");
            return;
        }

        // Permissions of file
        if (!flagStatNew)
        {
            printf("-");
            if (statNew.st_mode & S_IRUSR)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWUSR)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXUSR)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IRGRP)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWGRP)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXGRP)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IROTH)
            {
                printf("r");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IWOTH)
            {
                printf("w");
            }
            else
            {
                printf("-");
            }
            if (statNew.st_mode & S_IXOTH)
            {
                printf("x");
            }
            else
            {
                printf("-");
            }
            printf(" ");
            // -------------------------------------------------------------
            // Do this separately -> links
            printf("%lu ", statNew.st_nlink);
            // -------------------------------------------------------------
            struct passwd *pwd;
            pwd = getpwuid(statNew.st_gid);
            printf("%s ", pwd->pw_name);
            // -------------------------------------------------------------
            struct group *grp;
            grp = getgrgid(statNew.st_gid);
            printf("%s ", grp->gr_name);
            // -------------------------------------------------------------
            int size = lseek(fdf, 0, SEEK_END);
            printf("%d ", size);
            // -------------------------------------------------------------
            char *s = ctime(&statNew.st_mtime);
            for (int i = 4; i <= 15; i++)
            {
                printf("%c", s[i]);
            }
            printf(" ");
            // -------------------------------------------------------------
            printf("\033[0;32m%s", in);
            printf("\033[0m\n");
        }
    }
}

void ls(char home[])
{
    char delim[] = " \n\t";
    char *token = strtok(NULL, delim);
    int flagl = 0, flaga = 0;
    int count = 0;
    while (token != NULL)
    {
        if (!strcmp(token, "-a"))
        {
            flaga = 1;
        }
        else if (!strcmp(token, "-l"))
        {
            flagl = 1;
        }
        else if (!strcmp(token, "-al") || (!strcmp(token, "-la")))
        {
            flagl = 1;
            flaga = 1;
        }
        else
        {
            strcpy(lsPath[count], token);
            count++;
        }
        token = strtok(NULL, delim);
    }
    if (count == 0)
    {
        lsPath[count][0] = '.';
        lsPath[count][1] = '\0';
        count++;
    }
    lexiPath(count);
    tildaToHome(count, home);
    char cur[maxPathSize];
    getcwd(cur, maxPathSize);

    // For files
    int flag1 = 0;
    int flagg1 = 0;
    for (int c = 0; c < count; c++)
    {
        int flagDir = 0;
        int lsOpen = chdir(lsPath[c]);
        if (lsOpen == 0)
        {
            flagDir = 1;
        }
        chdir(cur);
        if (!flagDir)
        {
            flagg1 = 1;
            flag1 = 1;
            int fdf = open(lsPath[c], O_RDONLY, 0600);
            if (fdf < 0)
            {
                printf("\033[1;31mls: cannot access '%s': No such file or directory", lsPath[c]);
                printf("\033[0m\n");
                continue;
            }
            int l = strlen(lsPath[c]), flagOut = 0;
            if (l >= 4)
            {
                if (lsPath[c][l - 4] == '.')
                {
                    if (lsPath[c][l - 3] == 'o' && lsPath[c][l - 2] == 'u' && lsPath[c][l - 1] == 't')
                    {
                        flagOut = 1;
                    }
                }
            }
            if (!flagl)
            {
                if (flagOut)
                {
                    printf("\033[0;32m%s", lsPath[c]);
                    printf("\033[0m  ");
                    continue;
                }
                printf("%s ", lsPath[c]);
            }
            else
            {
                if (flagOut)
                {
                    minusL(2, lsPath[c], 0);
                    continue;
                }
                minusL(0, lsPath[c], 0);
            }
        }
    }
    if (flag1 && !flagl)
    {
        printf("\n");
    }
    // For folders
    for (int c = 0; c < count; c++)
    {
        int flagg = 0;
        int flagDir = 0;
        //int flagg = 0;
        int lsOpen = chdir(lsPath[c]);
        if (lsOpen == 0)
        {
            flagDir = 1;
        }
        chdir(cur);
        if (flagDir)
        {
            if (flagg1)
            {
                printf("\n%s:\n", lsPath[c]);
            }
            else if (count > 1 && !flagg)
            {
                printf("%s:\n", lsPath[c]);
                flagg = 1;
            }
            else if (count > 1)
            {
                printf("\n%s:\n", lsPath[c]);
            }
            DIR *d;
            struct dirent *dir;
            d = opendir(lsPath[c]);
            int iter = 0;
            if (d)
            {
                while ((dir = readdir(d)) != NULL)
                {
                    data[iter] = dir->d_name;

                    iter++;
                }
                lexi(iter);
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
                    chdir(cur);
                    isdir[i] = flagDirIn;
                }
                closedir(d);
                if (flaga && !flagl)
                {
                    for (int i = 0; i < iter; i++)
                    {
                        if (isdir[i])
                        {
                            printf("\033[0;34m%s", data[i]);
                            printf("\033[0m  ");
                        }
                        else
                        {
                            int l = strlen(data[i]);
                            if (l >= 4)
                            {
                                if (data[i][l - 4] == '.')
                                {
                                    if (data[i][l - 3] == 'o' && data[i][l - 2] == 'u' && data[i][l - 1] == 't')
                                    {
                                        printf("\033[0;32m%s", data[i]);
                                        printf("\033[0m  ");
                                        continue;
                                    }
                                }
                            }
                            printf("%s  ", data[i]);
                        }
                    }
                    printf("\n");
                }
                else if (flaga && flagl)
                {
                    int block = 0;
                    char local[iter][maxPathSize];
                    for (int i = 0; i < iter; i++)
                    {
                        strcpy(local[i], data[i]);
                        struct stat stat1;
                        int flagStat = stat(local[i], &stat1);
                        if (!flagStat)
                        {
                            block += stat1.st_blocks;
                        }
                        else
                        {
                            printf("\033[1;31mls: cannot access '%s': Permission denied", local[i]);
                            printf("\033[0m\n");
                            continue;
                        }
                    }
                    printf("total %d\n",block/2);
                    for (int i = 0; i < iter; i++)
                    {
                        int l1 = strlen(data[i]);
                        char temp1[l1];
                        strcpy(temp1, local[i]);
                        if (isdir[i])
                        {
                            //printf("sent is %s\n",data[i]);
                            
                            minusL(1, temp1, 1);
                        }
                        else
                        {
                            int l = strlen(temp1);
                            if (l >= 4)
                            {
                                if (temp1[l - 4] == '.')
                                {
                                    if (temp1[l - 3] == 'o' && temp1[l - 2] == 'u' && temp1[l - 1] == 't')
                                    {
                                        minusL(2, temp1, 1);
                                        continue;
                                    }
                                }
                            }
                            minusL(0, temp1, 1);
                        }
                    }
                }
                else if (!flaga && !flagl)
                {
                    int ans = 0;
                    for (int i = 0; i < iter; i++)
                    {
                        if (data[i][0] == '.')
                        {
                            continue;
                        }
                        ans = 1;
                        if (isdir[i])
                        {
                            printf("\033[0;34m%s", data[i]);
                            printf("\033[0m  ");
                        }
                        else
                        {
                            int l = strlen(data[i]);
                            if (l >= 4)
                            {
                                if (data[i][l - 4] == '.')
                                {
                                    if (data[i][l - 3] == 'o' && data[i][l - 2] == 'u' && data[i][l - 1] == 't')
                                    {
                                        printf("\033[0;32m%s", data[i]);
                                        printf("\033[0m  ");
                                        continue;
                                    }
                                }
                            }
                            printf("%s  ", data[i]);
                        }
                    }
                    if(ans)
                    {
                        printf("\n");
                    }
                }
                else if (!flaga && flagl)
                {
                    int block = 0;
                    char local[iter][maxPathSize];
                    for (int i = 0; i < iter; i++)
                    {
                        strcpy(local[i], data[i]);
                        if (data[i][0] == '.')
                        {
                            continue;
                        }
                        struct stat stat1;
                        int flagStat = stat(local[i], &stat1);
                        if (!flagStat)
                        {
                            block += stat1.st_blocks;
                        }
                        else
                        {
                            printf("\033[1;31mls: cannot access '%s': Permission denied", local[i]);
                            printf("\033[0m\n");
                            continue;
                        }
                    }
                    printf("total %d\n",block/2);
                    for (int i = 0; i < iter; i++)
                    {
                        if (data[i][0] == '.')
                        {
                            continue;
                        }
                        if (isdir[i])
                        {
                            minusL(1, local[i], 1);
                        }
                        else
                        {
                            int l = strlen(data[i]);
                            if (l >= 4)
                            {
                                if (data[i][l - 4] == '.')
                                {
                                    if (data[i][l - 3] == 'o' && data[i][l - 2] == 'u' && data[i][l - 1] == 't')
                                    {
                                        minusL(2, local[i], 1);
                                        continue;
                                    }
                                }
                            }
                            minusL(0, local[i], 1);
                        }
                    }
                }
            }
            if (count > 1 && c != count - 1)
            {
                printf("\n");
            }
        }
    }

    return;
}