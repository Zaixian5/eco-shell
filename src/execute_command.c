// command.c

#include <stdio.h>
#include <string.h>

#define ARGS_BUFFER 50

void power(char *[]);
void cleanupFiles(char *[]);
void cleanupProcess(char *[]);

// 일반적인 쉘 명령어를 처리할 때 사용된다.
int runProcess(char *[]);

// 명령행에서 입력받은 명령어를 처리한다.
int executeCommand(char *input)
{
    /*
        명령행에서 입력받은 하나의 전체 문자열을 strtok을 통해 공백(" ")을 기준으로 잘라서
        토큰화된 문자열을 저장하는 포인터 배열
    */
    char *args[ARGS_BUFFER];

    /*
        명령행에서 입력받은 문자열을 공백(" ")을 기준으로 자른
        토큰화된 문자열 하나(명령행 인자)를 임시로 저장하는 버퍼
    */
    char *token;

    int i = 0;

    // 명령어 토큰화
    token = strtok(input, " ");

    // 명령행에서 입력받은 문자열의 토큰화 처리 과정
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    // 마지막 명령행 인자 뒤에 NULL 종료 값을 할당함으로써 배열의 끝을 나타냄
    args[i] = NULL;

    // 환경 친화 명령어 실행
    if (strcmp(args[0], "power") == 0)
    {
        power(args);

        return 0;
    }
    else if (strcmp(args[0], "fclean") == 0)
    {
        // 불필요한 파일 정리 명령 - fclean
        cleanupFiles(args);

        return 0;
    }
    else if (strcmp(args[0], "pclean") == 0)
    {
        // 불필요한 프로세스 종료 명령 - pclean
        cleanupProcess(args);

        return 0;
    }

    // 일반 쉘 명령어 실행
    return runProcess(args);
}
