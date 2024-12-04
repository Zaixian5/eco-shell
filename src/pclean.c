// pclean.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define INACTIVE_LIMIT 3600 // 1시간
#define PATH_BUF 1000

void pclean(void);
int getProcessName(pid_t, char*, size_t);
int isBackgroundProcess(pid_t);
void pcleanHelp(void);
void cleanupProcess();

// 오래된 백그라운드 프로세스를 종료하는 함수
void pclean(void)
{
    DIR *procDir = opendir("/proc");
    struct dirent *entry;

    // 현재 시간
    time_t currentTime = time(NULL);

    char path[PATH_BUF];
    struct stat statBuf;
    int count_process_terminated = 0;

    if (!procDir)
    {
        perror("Failed to open /proc");
        return;
    }

    while ((entry = readdir(procDir)) != NULL)
    {
        pid_t pid = atoi(entry->d_name);

        // /proc/[pid] 디렉토리의 수정 시간 확인
        snprintf(path, sizeof(path), "/proc/%d", pid);
        if (stat(path, &statBuf) == 0)
        {
            double elapsedTime = difftime(currentTime, statBuf.st_mtime);

            // 오래된 백그라운드 프로세스
            if (elapsedTime >= INACTIVE_LIMIT && isBackgroundProcess(pid))
            {
                printf("프로세스 종료 후보: PID = %d, 이름 = ", pid);
                char process_name[256];
                if (getProcessName(pid, process_name, sizeof(process_name)))
                {
                    printf("%s", process_name);
                }
                else
                {
                    printf("알 수 없음");
                }
                printf(", 경과 시간 = %.0f 초\n", elapsedTime);

                // 사용자 확인
                printf("종료하시겠습니까? (Y/N): ");
                char response = getchar();
                while (getchar() != '\n'); // 입력 버퍼 비우기
                if (response == 'Y' || response == 'y')
                {
                    if (kill(pid, SIGKILL) == 0)
                    {
                        printf("프로세스 PID = %d 종료됨.\n", pid);
                        count_process_terminated++;
                    }
                    else
                    {
                        perror("프로세스를 종료할 수 없음");
                    }
                }
                else
                {
                    printf("프로세스 PID = %d 종료 취소됨.\n", pid);
                }
            }
        }
    }

    closedir(procDir);

    if (count_process_terminated == 0)
    {
        printf("종료된 프로세스가 없습니다.\n");
    }
    else
    {
        printf("총 %d개의 프로세스가 종료되었습니다.\n", count_process_terminated);
    }
}

// 프로세스 이름을 가져오는 함수
int getProcessName(pid_t pid, char *name, size_t name_size)
{
    char comm_path[PATH_BUF];
    snprintf(comm_path, sizeof(comm_path), "/proc/%d/comm", pid);

    FILE *comm_file = fopen(comm_path, "r");
    if (!comm_file)
        return 0; // 이름을 읽을 수 없음

    fgets(name, name_size, comm_file);
    name[strcspn(name, "\n")] = '\0'; // 개행 문자 제거
    fclose(comm_file);
    return 1; // 이름 읽기 성공
}

// 프로세스가 백그라운드 상태인지 확인하는 함수
int isBackgroundProcess(pid_t pid)
{
    char stat_path[PATH_BUF];
    FILE *stat_file;
    char state;

    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
    stat_file = fopen(stat_path, "r");
    if (!stat_file)
        return 0; // 파일을 열 수 없으면 프로세스가 없음

    // 프로세스의 상태(state) 필드 확인
    fscanf(stat_file, "%*d %*s %c", &state);
    fclose(stat_file);

    // 'S'(Sleeping), 'I'(Idle), 'Z'(Zombie)는 백그라운드 상태로 간주
    return (state == 'S' || state == 'I' || state == 'Z');
}

void pcleanHelp(void)
{
    printf("pclean: 오래 사용된 백그라운드 프로세스를 정리하여 메모리 공간을 최적화합니다.\n");
    printf("사용법: pclean <옵션>\n");
    printf("  -r: pclean 실행\n");
    printf("  -h: 도움말\n\n");
    printf("!!! 주의사항 !!!\n");
    printf("pclean은 시스템 운영에 필수적인 프로세스를 구분하지 않습니다.\n");
    printf("종료되는 프로세스를 확인 후 확실히 종료해도 되는 프로세스인 경우만 Y를 입력해 종료하세요.\n");
}

void cleanupProcess(char *pclean_args[])
{
    if (pclean_args[1] == NULL)
    {
        pcleanHelp();
        return;
    }

    if (strcmp(pclean_args[1], "-h") == 0)
    {
        pcleanHelp();
    }
    else if (strcmp(pclean_args[1], "-r") == 0)
    {
        pclean();
    }
    else
    {
        fprintf(stderr, "%s: 이런 옵션은 없습니다.\n'pclean -h'를 입력하여 사용법을 확인하세요.\n", pclean_args[1]);
    }
}
