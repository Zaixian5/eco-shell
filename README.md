# eco-shell

최초 커밋: 2024/12/4   최근 커밋: 2024/12/17

---

# 1. 프로젝트 개요

## 1. 프로젝트 정의

eco-shell(이하 에코쉘)은 전력 사용량을 줄이는 PC 사용습관을 기르는데 도움을 주기 위한 환경 친화적 리눅스 쉘이다. 쉘 인터페이스 내에서 간단한 명령어로 하드디스크, 메모리를 최적화 하고 전력 사용량 통계를 보여 주어 PC 사용에 있어 사용자가 전력 소모를 줄일 수 있도록 도와준다.

## 2. 프로젝트 배경

이 프로젝트는 **충북대학교 2024년도 2학기 서버프로그래밍 과목**에서 진행한 팀 프로젝트이다.

서버프로그래밍 과목은 리눅스 환경에서 C언어를 활용한 시스템 프로그래밍을 배우고 실습하는 과목이다. 전력 소모량 증가에 따른 탄소배출이 큰 환경문제를 일으키고 있는 현 상황에서 리눅스 시스템 프로그래밍 지식을 활용해 이를 해결하는데 기여할 수 있는 프로젝트를 생각하게 되었다. 그 결과 생각해낸 주제가 바로 이 환경 친화 리눅스 쉘 - 에코쉘이다.

## 3. 프로젝트 팀 구성

- 사재헌 - 깃허브 아이디: Zaixian5
- 이준혁 - 깃허브 아이디: visualnnz

## 4. 개발 기간

2024년 11월 ~ 2024년 12월

## 5. 주요 기능

### 1. power

1. 기능 설명: 현재 전력 소모 상태에 대한 정보를 표시해 주는 명령어
2. 옵션:
- -i: 현재 전력 소모량, 배터리 잔량, 남은 배터리 사용 시간, 배터리 상태, 배터리 모델명 표시
- -r: power 명령어 사용 기록 확인
- -c: 쉘 초기 실행 이후 전력 소모량 비교. 컴퓨터 사용 시간, 배터리 소모량, 배터리 소모 속도, 순간 전력 소모량 비교, 탄소 배출량 표시
- -h: 도움말 표시

### 2. fclean

1. 기능 설명: 불필요한 파일을 삭제하여 하드디스크를 최적화 해주는 명령어
2. 옵션:
- -t: 임시파일 삭제
- -l: 로그파일 삭제
- -c: 캐시 파일 삭제
- -h: 도움말 표시

### 3. pclean

1. 기능 설명: 불필요한 프로세스를 정리하여 메모리를 최적화 해주는 명령어
2. 옵션:
- -r: 1시간 이상 실행된 백그라운드 프로세스 종료
- -h: 도움말

## 6. 프로젝트 디렉토리 구조

```sql
eco-shell/
├── src/
│ ├── main.c
│ ├── run_shell.c
│ ├── execute_command.c
│ ├── run_process.c
│ ├── fclean.c
│ ├── pclean.c
│ ├── power.c
│ └── time.c
├── Makefile
└── README.md
```

### 1. eco-shell/

프로젝트 최상위 디렉토리. 해당 리포지터리를 clone 하면 생성된다.

### 2. src/

프로젝트 소스코드 디렉토리. 각 소스파일은 아래의 역할을 가진다.

1. **main.c**: main함수 소스파일
2. **run_shell.c**: 쉘 기본 인터페이스. 쉘 무한반복문이 구현됨
3. **execute_command.c**: 명령어를 해석하기 위한 파싱 알고리즘이 구현됨
4. **run_process.c**: 다른 bash 명령을 실행하기 위한 함수들
5. **fclean.c**: fclean 기능 구현을 위한 함수들
6. **pclean.c**: pclean 기능 구현을 위한 함수들
7. **power.c**: power 기능 구현을 위한 함수들
8. **time.c:** 시간 관련 기능 구현을 위한 함수들

### 3. Makefile

빌드를 위한 메이크 파일

### 4. README.md

프로젝트 설명을 위한 리드미 파일

# 2. 사용법

## 1. 설치 및 빌드

- 소스 파일을 내려 받은 후 Make로 빌드한다.

```bash
# 빌드
make

# 빌드 후 불필요한 오브젝트 파일 삭제
make clean
```

## 2. 에코쉘 실행 법

- 실행파일 이름은 eco-shell이다. 이를 실행하면 에코쉘로 진입한다.

```bash
# 실행 파일이 위치한 디렉토리 에서 아래 명령 입력
./eco-shell
```

# 3. 주의 사항

## 1. 시스템 요구 사항

bash 쉘을 사용할 수 있는 모든 리눅스 시스템에서 동작 가능

## 2. 하드웨어 요구 사항

배터리를 사용하는 PC에서 사용하는 것을 권장. 

power 명령은 배터리의 사용량을 바탕으로 전력 소모량을 측정하므로 전기 콘센트를 사용하는 PC에서는 정상적으로 동작하지 않을 수 있음.

## 3. 기타 사항

pclean 명령은 중요한 프로세스와 불필요한 프로세스를 확실하게 구분하지 못함. 

사용자가 쉘에 표시되는 종료 프로세스 후보를 확인하고 스스로 정리해야 함.