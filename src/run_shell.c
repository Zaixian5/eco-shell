// run_shell.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>

// 명령행 버퍼 크기
#define STDIN_BUFFER 1000

// 터미널에 출력되는 문자에 색상을 입히기 위한 ANSI 이스케이프 코드
#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_RESET "\033[0m"

// 현재 작업 디렉터리(절대 경로)
char *cwd = NULL;

// '~'을 포함한 현재 작업 디렉터리('~' == 홈 디렉터리)
char cwd_tilde[BUFSIZ];

/*
    홈 디렉터리 (e.g. /home/user1)
    최초 1회 홈 디렉터리 정보를 받아서 home_dir에 저장한다.
*/
char *home_dir;

int executeCommand(char *);

/*
    eco-shell을 작동시킨다.

    최초 1회 홈 디렉터리 정보를 받아 home_dir에 저장하고
    이후엔 while 무한 루프를 통해 명령행에 명령어를 계속 입력받고
    명령행에 "exit"을 입력하면 무한 루프를 빠져나와 eco-shell을 정상 종료한다.
*/
void runShell(void)
{
    // 명령행에서 입력받은 문자열을 저장할 버퍼
    char input[STDIN_BUFFER];

    // 임시 문자열 저장 버퍼
    char temp[BUFSIZ];

    int i = 0;
    int j = 0;

    home_dir = getenv("HOME");

    while (1)
    {
        cwd = getcwd(cwd, BUFSIZ);
        strcpy(cwd_tilde, "~");

        /*
            cwd에 home_dir이 포함되어 있거나 cwd == home_dir일 경우
            커맨드라인에서 현재 작업 디렉터리를 '~'를 포함해서 출력
        */
        if (strstr(cwd, home_dir) != NULL || strcmp(cwd, home_dir) == 0)
        {
            for (i = strlen(home_dir), j = 0; cwd[i] != '\0'; i++, j++)
            {
                temp[j] = cwd[i];
            }
            temp[j] = '\0';
            strcat(cwd_tilde, temp);
            cwd_tilde[strlen(cwd_tilde)] = '\0';

            printf(COLOR_GREEN "eco-shell" COLOR_RESET ":" COLOR_BLUE "%s" COLOR_RESET "$ ", cwd_tilde);
        }
        /*
            cwd에 home_dir이 포함되어 있지 않고 cwd != home_dir일 경우
            커맨드라인에서 현재 작업 디렉터리를 절대 경로로 출력
        */
        else
        {
            printf(COLOR_GREEN "eco-shell" COLOR_RESET ":" COLOR_BLUE "%s" COLOR_RESET "$ ", cwd);
        }

        // 명령행에서 입력을 받을 때 에러 발생 시 eco-shell 종료
        if (fgets(input, STDIN_BUFFER, stdin) == NULL)
        {
            break;
        }

        // 개행 문자 제거
        input[strcspn(input, "\n")] = '\0';

        // 입력이 빈 문자열인 경우 무시하고 다음 루프로 넘어감
        if (strlen(input) == 0)
        {
            continue;
        }

        // exit 명령어 처리
        if (strcmp(input, "exit") == 0)
        {
            printf("에코쉘을 종료합니다. 다음에 또 봐요!\n");
            break;
        }

        // 명령어 실행
        if (executeCommand(input) == EXIT_FAILURE)
        {
            fprintf(stderr, "명령어를 실행할 수 없습니다. 다시 시도해주세요.\n");
        }

        free(cwd);
        cwd = NULL;
    }
}