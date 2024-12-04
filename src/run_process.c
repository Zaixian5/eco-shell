// run_process.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// 현재 작업 디렉터리
extern char *cwd;

/*
    run_shell.c에서 선언한 home_dir
    └ 홈 디렉터리 (e.g. /home/user1)
*/
extern char *home_dir;

int i = 0;

// 명령어 처리를 마치고 일반 쉘 명령어를 실행한다.
int runProcess(char *args[])
{
    pid_t pid = fork();

    // fork 실패
    if (pid < 0)
    {
        perror("Fork fail");
        exit(EXIT_FAILURE);
    }
    // 자식 프로세스
    else if (pid == 0)
    {
        // 명령어가 cd인 경우 자식 프로세스가 아닌 부모 프로세스에서 cd를 처리하도록 함
        if (strcmp(args[0], "cd") == 0)
        {
            exit(EXIT_SUCCESS);
        }

        // pwd, ls 등 일반적인 명령어 처리
        if (execvp(args[0], args) == -1)
        {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }
    // 부모 프로세스
    else
    {
        // 자식 프로세스의 종료 상태 저장
        int status;

        wait(&status);

        /*
            자식 프로세스의 종료 코드가 1일 경우 runProcess 함수 -> executeCommand 함수 ->runShell 함수
            경로로 종료 코드 1을 전달해준다.
        */
        if (WEXITSTATUS(status) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }

        /*
            명령어가 cd인 경우 작업 디렉터리 위치를 변경하는 것이기 때문에
            자식 프로세스에서 cd를 처리하고 자식 프로세스가 종료되어버리면 의미가 없음.
            따라서 cd의 경우 예외적으로 부모 프로세스에서 별도로 명령어를 처리한다.
        */
        if (strcmp(args[0], "cd") == 0)
        {
            // 명령행에 "cd"를 입력할 경우
            if (args[1] == NULL)
            {
                if (chdir(home_dir) != 0)
                {
                    perror("cd");
                }
            }
            // 명령행에 "cd ~"를 입력할 경우
            else if (strcmp(args[1], "~") == 0)
            {
                if (chdir(home_dir) != 0)
                {
                    perror("cd");
                }
            }
            // 그 외에 명령행에 "cd (절대경로)"를 입력한 경우
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cd");
                }
            }
        }
    }

    return 0;
}
