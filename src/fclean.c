// fclean.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define FILEPATH_BUFFER 500

void cleanupTmpFiles(void);
void cleanupLogFiles(void);
void cleanupCacheFiles(void);
void cleanupAllFiles(void);
void cleanupHelp(void);
void cleanupFiles(char *[]);

void cleanupTmpFiles()
{
    const char *temp_dir = "/tmp/";
    struct dirent *entry;
    char filepath[FILEPATH_BUFFER];

    int count_file_deleted = 0; // 삭제된 파일이 하나라도 있으면 1+, 없으면 0

    // /tmp/ 디렉토리를 엽니다.
    DIR *dir = opendir(temp_dir);
    if (dir == NULL)
    {
        perror("Failed to open /tmp/");
        return;
    }

    // /tmp/의 파일들을 순회합니다.
    while ((entry = readdir(dir)) != NULL)
    {
        // "." 및 ".." 디렉토리는 건너뜁니다.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // 파일의 전체 경로 생성
        snprintf(filepath, sizeof(filepath), "%s%s", temp_dir, entry->d_name);

        // 삭제 명령어 생성
        char command[FILEPATH_BUFFER + 100];
        snprintf(command, sizeof(command), "sudo rm -rf %s", filepath);

        // 시스템 호출로 파일 삭제
        int result = system(command);
        if (result == 0)
        {
            printf("관리자 권한으로 제거됨: %s\n", filepath);
            count_file_deleted++;
        }
        else
        {
            fprintf(stderr, "오류. 파일 제거 실패.: %s\n", filepath);
        }
    }

    // 디렉토리 닫기
    closedir(dir);

    if (count_file_deleted == 0)
    {
        printf("정리할 임시 파일이 없음.\n");
    }
    else if (count_file_deleted > 0)
    {
        printf("모든 임시 파일 정리됨.\n");
    }
}

void cleanupLogFiles(void)
{
    const char *temp_dir = "/var/log/";
    struct dirent *entry;
    char filepath[FILEPATH_BUFFER];

    int count_file_deleted = 0; // 삭제된 파일이 하나라도 있으면 1+, 없으면 0

    // /tmp/ 디렉토리를 엽니다.
    DIR *dir = opendir(temp_dir);
    if (dir == NULL)
    {
        perror("Failed to open /var/log/");
        return;
    }

    // /tmp/의 파일들을 순회합니다.
    while ((entry = readdir(dir)) != NULL)
    {
        // "." 및 ".." 디렉토리는 건너뜁니다.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // 파일의 전체 경로 생성
        snprintf(filepath, sizeof(filepath), "%s%s", temp_dir, entry->d_name);

        // 삭제 명령어 생성
        char command[FILEPATH_BUFFER + 100];
        snprintf(command, sizeof(command), "sudo rm -rf %s", filepath);

        // 시스템 호출로 파일 삭제
        int result = system(command);
        if (result == 0)
        {
            printf("관리자 권한으로 제거됨.: %s\n", filepath);
            count_file_deleted++;
        }
        else
        {
            fprintf(stderr, "오류. 파일 제거 실패.: %s\n", filepath);
        }
    }

    // 디렉토리 닫기
    closedir(dir);

    if (count_file_deleted == 0)
    {
        printf("정리할 로그 파일이 없음.\n");
    }
    else if (count_file_deleted > 0)
    {
        printf("모든 로그 파일 정리됨.\n");
    }
}

void cleanupCacheFiles(void)
{
    const char *temp_dir = "/var/cache/";
    struct dirent *entry;
    char filepath[FILEPATH_BUFFER];

    int count_file_deleted = 0; // 삭제된 파일이 하나라도 있으면 1+, 없으면 0

    // /tmp/ 디렉토리를 엽니다.
    DIR *dir = opendir(temp_dir);
    if (dir == NULL)
    {
        perror("Failed to open /var/cache/");
        return;
    }

    // /tmp/의 파일들을 순회합니다.
    while ((entry = readdir(dir)) != NULL)
    {
        // "." 및 ".." 디렉토리는 건너뜁니다.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // 파일의 전체 경로 생성
        snprintf(filepath, sizeof(filepath), "%s%s", temp_dir, entry->d_name);

        // 삭제 명령어 생성
        char command[FILEPATH_BUFFER + 100];
        snprintf(command, sizeof(command), "sudo rm -rf %s", filepath);

        // 시스템 호출로 파일 삭제
        int result = system(command);
        if (result == 0)
        {
            printf("관리자 권한으로 제거됨.: %s\n", filepath);
            count_file_deleted++;
        }
        else
        {
            fprintf(stderr, "오류. 파일 제거 실패.: %s\n", filepath);
        }
    }

    // 디렉토리 닫기
    closedir(dir);

    if (count_file_deleted == 0)
    {
        printf("정리할 캐시 파일이 없음.\n");
    }
    else if (count_file_deleted > 0)
    {
        printf("모든 캐시 파일 정리됨.\n");
    }
}

void cleanupAllFiles(void)
{
    cleanupTmpFiles();
    printf("\n");

    cleanupLogFiles();
    printf("\n");

    cleanupCacheFiles();
    printf("\n");
}

void cleanupHelp(void)
{
    printf("fclean: 불필요한 파일을 정리하여 디스크 공간을 최적화합니다.\n");
    printf("사용법: fclean <옵션>\n");
    printf("  -t: 임시 파일 정리\n");
    printf("  -l: 로그 파일 정리\n");
    printf("  -c: 캐시 파일 정리\n");
    printf("  -a: 임시, 로그, 캐시 파일 모두 정리\n");
    printf("  -h: 도움말\n");
}

void cleanupFiles(char *fclean_args[])
{
    if (fclean_args[1] == NULL)
    {
        cleanupHelp();
        return;
    }

    if (strcmp(fclean_args[1], "-t") == 0)
    {
        cleanupTmpFiles();
    }
    else if (strcmp(fclean_args[1], "-l") == 0)
    {
        cleanupLogFiles();
    }
    else if (strcmp(fclean_args[1], "-c") == 0)
    {
        cleanupCacheFiles();
    }
    else if (strcmp(fclean_args[1], "-a") == 0)
    {
        cleanupAllFiles();
    }
    else if (strcmp(fclean_args[1], "-h") == 0)
    {
        cleanupHelp();
    }
    else
    {
        fprintf(stderr, "%s: 이런 옵션은 없습니다.\n'fclean -h'를 입력하여 사용법을 확인하세요.\n", fclean_args[1]);
    }
}