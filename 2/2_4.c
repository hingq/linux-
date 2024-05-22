/*
 *File: mychomd_example.c
 *A simple chmod command.
 *format: mychmod 777 demo.c
 *Writen by Shawn Yang
 *Date:13 Sep,2023
 *Version:1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void getcomand(char *permission)
{
    switch (*permissions)
    {
    case 'u':
        // 对 mode 变量执行按位或运算，将这三个权限组合起来 1|0=1
        mode |= S_IRUSR | S_IWUSR | S_IXUSR;
        break;
    case 'g':
        mode |= S_IRGRP | S_IWGRP | S_IXGRP;
        break;
    case 'o':
        mode |= S_IROTH | S_IWOTH | S_IXOTH;
        break;
    }
}
int main(int argc, char *argv[])
{
    int mode;
    int mode_u;
    int mode_g;
    int mode_o;

    char *path;

    /*Checking the legitimacy of parameters*/
    if (argc < 3)
    {
        printf("%s,need 3 arguments \n", argv[0]);
    }

    /*Getting command line arguments*/
    mode = atoi(argv[1]);
    if (mode > 777 | mode < 0)
    {
        printf("mode number error!\n");
        exit(0);
    }
    else if (mode == 0)
    {
        char *permissions = argv[1];
        path = argv[2];

        while (*permissions)
        {
            switch (*permissions)
            {
            case 'u':
                // 对 mode 变量执行按位或运算，将这三个权限组合起来 1|0=1
                mode |= S_IRUSR | S_IWUSR | S_IXUSR;
                break;
            case 'g':
                mode |= S_IRGRP | S_IWGRP | S_IXGRP;
                break;
            case 'o':
                mode |= S_IROTH | S_IWOTH | S_IXOTH;
                break;
            case '+':
                permissions++;
                if (*permissions == 'x')
                {
                    mode |= S_IXUSR | S_IXGRP | S_IXOTH;
                }
                else if (*permissions == 'w')
                {
                    mode |= S_IWUSR | S_IWGRP | S_IWOTH;
                }
                break;
            case '-':
                permissions++;
                if (*permissions == 'w')
                {
                    // 变为不具备写入权限的掩码。然后通过与原始 mode 变量进行按位与操作，将这些权限从 mode 变量中清除。 ~1=0
                    mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);
                }
                break;
            default:
                exit(EXIT_FAILURE);
            }
            permissions++;
        }
    }
    else
    {
        mode_u = mode / 100;
        mode_g = (mode - (mode_u * 100)) / 10;
        mode_o = mode - (mode_u * 100) - (mode_g * 10);

        /*Convert to octal*/
        mode = (mode_u * 8 * 8) + (mode_g * 8) + mode_o;
    }

    path = argv[2];

    if (chmod(path, mode) == -1)
    {
        perror("chmod error");
    }
    return 0;
}