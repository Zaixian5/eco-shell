// power.c

/*
    TODO:
    1. 전체적으로 함수 주석 및 지역 변수, 전역 변수, 구조체 변수 등의 변수 주석 추가 필요
    2. 함수 주석은 함수 구현부의 함수 이름 위에 함수의 동작을 설명해야 함
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 1000

typedef struct
{
    int energy_now;
    int energy_full;
    int power_now;
    char model_name[MAX_BUF];
    char status[MAX_BUF];
} POWER_INF;

typedef struct
{
    double energy_now_wh;
    double energy_now_percent;
    double power_now_w;
    double remaining_hours;
    char time[MAX_BUF];
    int time_s;
} POWER_CAL;

typedef struct
{
    int number;
    double energy_now_wh;
    double energy_now_percent;
    double power_now_w;
    char date[50];
    char time[50];
    int time_s;
} POWER_CMP;

void power(char *[]);
void showPowerConsumption(void);
void calculatePowerConsumption(void);
void showPowerReport(void);
void reportPower(void);
void comparePowerConsumption(void); // 미완
void powerHelp(void);
FILE *openFile(const char *);
void nowTime(char[], int *, int);
int compareTime(int, int);

/* /sys/class/power_supply/BAT1/...: 컴퓨터의 배터리 정보를 기록하는 파일 - POWER_INF에 저장되는 내용 */

// 문제점 및 해결과제:
// 1. 모든 리눅스 시스템에 이 파일이 존재하는 지는 확실치 않음.
// 2. 전기 콘센트를 사용하는 데스크톱에선 BAT 파일이 없음. 즉 이 방법으로 전력 사용량 측정이 불가능함.
const char *energy_now_path = "/sys/class/power_supply/BAT1/energy_now";
const char *energy_full_path = "/sys/class/power_supply/BAT1/energy_full";
const char *power_now_path = "/sys/class/power_supply/BAT1/power_now";
const char *model_name_path = "/sys/class/power_supply/BAT1/model_name";
const char *status_path = "/sys/class/power_supply/BAT1/status";
const char *power_report_path = ".eco-shell_power_report";

// 배터리 정보 구조체 전역변수
POWER_INF power_inf;
POWER_CAL power_cal;

void power(char *power_args[])
{
    if (power_args[1] == NULL)
    {
        powerHelp();
        return;
    }

    if (strcmp(power_args[1], "-i") == 0)
    {
        calculatePowerConsumption();
        showPowerConsumption();
        reportPower();
    }
    else if (strcmp(power_args[1], "-r") == 0)
    {
        showPowerReport();
    }
    else if (strcmp(power_args[1], "-c") == 0)
    {
        calculatePowerConsumption();
        comparePowerConsumption();
        reportPower();
    }
    else if (strcmp(power_args[1], "-h") == 0)
    {
        powerHelp();
    }
    else
    {
        fprintf(stderr, "%s: 이런 옵션은 없습니다.\n'power -h'를 입력하여 사용법을 확인하세요.\n", power_args[1]);
    }
}

void calculatePowerConsumption(void)
{
    FILE *fp_energy_now = openFile(energy_now_path);
    FILE *fp_energy_full = openFile(energy_full_path);
    FILE *fp_power_now = openFile(power_now_path);
    FILE *fp_model_name = openFile(model_name_path);
    FILE *fp_status = openFile(status_path);

    char buf[MAX_BUF];

    if (fgets(buf, sizeof(buf), fp_energy_now) != NULL)
        power_inf.energy_now = atoi(buf);

    if (fgets(buf, sizeof(buf), fp_energy_full) != NULL)
        power_inf.energy_full = atoi(buf);

    if (fgets(buf, sizeof(buf), fp_power_now) != NULL)
        power_inf.power_now = atoi(buf);

    if (fgets(buf, sizeof(buf), fp_model_name) != NULL)
    {
        strncpy(power_inf.model_name, buf, MAX_BUF - 1);
        power_inf.model_name[MAX_BUF - 1] = '\0';
    }

    if (fgets(buf, sizeof(buf), fp_status) != NULL)
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, "Discharging") == 0)
        {
            strncpy(power_inf.status, "충전 중이 아님", MAX_BUF - 1);
        }
        else if (strcmp(buf, "Charging") == 0)
        {
            strncpy(power_inf.status, "충전 중", MAX_BUF - 1);
        }
        else
        {
            strncpy(power_inf.status, "충전 상태 확인 불가", MAX_BUF - 1);
        }
    }

    power_cal.energy_now_wh = (double)power_inf.energy_now / 1000000;
    power_cal.energy_now_percent = ((double)power_inf.energy_now / power_inf.energy_full) * 100;
    power_cal.power_now_w = (double)power_inf.power_now / 1000000;
    power_cal.remaining_hours = (power_cal.power_now_w > 0) ? power_cal.energy_now_wh / power_cal.power_now_w : -1;

    fclose(fp_energy_now);
    fclose(fp_energy_full);
    fclose(fp_power_now);
    fclose(fp_model_name);
    fclose(fp_status);
}

void reportPower(void)
{
    FILE *fp_report_power = openFile(power_report_path);
    static int idx = 1;

    nowTime(power_cal.time, &(power_cal.time_s), MAX_BUF);

    fprintf(
        fp_report_power,
        "%d %lf %lf %lf %s %d\n",
        idx++,
        power_cal.power_now_w,
        power_cal.energy_now_wh,
        power_cal.energy_now_percent,
        power_cal.time,
        power_cal.time_s);
    fclose(fp_report_power);
}

void showPowerConsumption(void)
{
    printf("현재 전력 소비량: %.2lfW\n", power_cal.power_now_w);
    printf("현재 배터리 잔량: %.2lfWH(%.2lf%%)\n", power_cal.energy_now_wh, power_cal.energy_now_percent);

    if (power_cal.remaining_hours > 0)
    {
        printf("남은 배터리 지속 시간: %.2lfH\n", power_cal.remaining_hours);
    }
    else
    {
        printf("남은 배터리 지속 시간: 계산 불가\n");
    }

    printf("배터리 상태: %s\n", power_inf.status);
    printf("배터리 모델명: %s\n", power_inf.model_name);
}

void showPowerReport(void)
{
    FILE *fp_report_power = openFile(power_report_path);
    char buf[MAX_BUF];

    printf("%s %s %s\n", "번호", "전력소모량(W)", "배터리잔량(WH) 배터리잔량(%) 날짜 시간 시리얼넘버");
    printf("------------------------------------------------------------------\n");
    while ((fgets(buf, sizeof(buf), fp_report_power)) != NULL)
    {
        printf("%s", buf);
    }

    fclose(fp_report_power);
}

/* comparePowerConsumption(): 전력 사용량 비교를 위한 함수 */
// TODO: 전력 소모량을 바탕으로 쉘 실행 시간 동안의 탄소 배출량 추산.
void comparePowerConsumption(void)
{
    FILE *fp_report_power = openFile(power_report_path);

    char buf[MAX_BUF];
    char first_line[MAX_BUF];
    int use_time;
    POWER_CMP init;

    // 파일의 첫번째 줄을 읽음
    fseek(fp_report_power, 0, SEEK_SET);
    if ((fgets(buf, sizeof(buf), fp_report_power)) != NULL)
    {
        strncpy(first_line, buf, sizeof(first_line) - 1);
        sscanf(
            first_line,
            "%d %lf %lf %lf %s %s %d",
            &init.number,
            &init.power_now_w,
            &init.energy_now_wh,
            &init.energy_now_percent,
            init.date,
            init.time,
            &init.time_s);
    }
    else
    {
        fprintf(stderr, "오류. power 기록을 읽을 수 없음.\n");
    }

    use_time = compareTime(init.time_s, power_cal.time_s);

    // 결과 출력
    printf("에코쉘 실행 이후...\n");
    printf("1. 컴퓨터 사용 시간: %d분\n", use_time);
    printf(
        "2. 배터리 소모량: 약 %.2lfWH(%.2lf%%)\n",
        init.energy_now_wh - power_cal.energy_now_wh,
        init.energy_now_percent - power_cal.energy_now_percent);
    printf("3. 배터리 소모 속도: ");
    if (use_time == 0)
    {
        printf("컴퓨터 사용시간이 매우 짧아 배터리 소모 속도를 계산할 수 없습니다. 나중에 다시 시도하세요.\n");
    }
    else
    {
        printf(
            "분당 %.2lfWH(%.2lf%%)\n",
            (init.energy_now_wh - power_cal.energy_now_wh) / use_time,
            (init.energy_now_percent - power_cal.energy_now_percent) / use_time);
    }

    printf("4. 순간 전력 소모량: ");
    if (power_cal.power_now_w > init.power_now_w)
    {
        printf("%.2lfW 더 낮음\n", (power_cal.power_now_w - init.power_now_w));
    }
    else if (power_cal.power_now_w < init.power_now_w)
    {
        printf("%.2lfW 더 높음\n", (init.power_now_w - power_cal.power_now_w));
    }
    else
    {
        printf("차이 없음\n");
    }

    printf("5. 탄소 배출량: 약 %.2lfg\n", ((init.energy_now_wh - power_cal.energy_now_wh) / 1000) * 424);

    fclose(fp_report_power);
}

void powerHelp(void)
{
    printf("power: 전력 사용량 확인 명령어.\n");
    printf("사용법: fclean <옵션>\n");
    printf("  -i: 배터리 잔량, 배터리 소모량, 남은 배터리 사용 시간, 배터리 정보 출력\n");
    printf("  -r: power 명령어 사용 기록 확인.\n");
    printf("  -c: 최근 power 명령어 사용 이후 전력 소모량 비교\n");
    printf("  -h: 도움말\n");
}

FILE *openFile(const char *path)
{
    FILE *file;

    if (strcmp(path, power_report_path) == 0)
    {
        file = fopen(path, "a+");
    }
    else
    {
        file = fopen(path, "r");
    }

    if (file == NULL)
    {
        perror(path);
        exit(EXIT_FAILURE);
    }

    return file;
}