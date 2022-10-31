#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//Collaborated with Nicholas Wayoe

int main(int argc, char **argv)
{
    int pipefd1[2];
    int pipefd2[2];
    
    int pid;
    int pid2;

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", argv[1], NULL};
    char *sort_args[] = {"sort", NULL};

    if (pipe(pipefd1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(pipefd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    if (pid == 0)
    {
        pid2 = fork();
        if (pid2 < 0)
        {
            fprintf(stderr, "fork Failed");
            return 1;
        }

        if (pid2 == 0)
        {
            dup2(pipefd2[0], 0);
            close(pipefd2[1]);
            close(pipefd1[1]);
            close(pipefd1[0]);
            execvp("sort", sort_args);
        }
        else
        {
            dup2(pipefd1[0], 0);
            dup2(pipefd2[1], 1);
            close(pipefd1[1]);
            close(pipefd2[0]);
            execvp("grep", grep_args);
        }
    }
    else
    {
        dup2(pipefd1[1], 1);
        close(pipefd1[0]);
        close(pipefd2[1]);
        execvp("cat", cat_args);
    }
    return 1;
}