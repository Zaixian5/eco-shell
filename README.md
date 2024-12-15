# eco-shell
2024-2 서버프로그래밍 Term Project

<hr>

<h3>1. 프로젝트 개요</h3>
    <p>eco-shell은 환경 친화적 리눅스 쉘로 아래와 같은 기능을 제공한다.</p>
    <ol>
        <li>불필요한 파일 정리</li>
        <li>불필요한 프로세스 정리</li>
        <li>전력 소모량 정보 확인</li>
    </ol>

<h3>2. 프로젝트 디렉토리 구조</h3>

eco-shell/<br>
├── src/<br>
│   ├── main.c<br>
│   ├── run_shell.c<br>
│   ├── execute_command.c<br>
│   ├── run_process.c<br>
│   ├── fclean.c<br>
│   ├── pclean.c<br>
│   ├── power.c<br>
│   └── time.c<br>
├── Makefile<br>
└── README.md<br>

<h3>3. 빌드 및 사용법</h3>
<ol>
<li>빌드</li>

```bash
$ make        # eco-shell 디렉토리에 들어가서 입력
$ make clean  # 빌드 후 불필요한 파일 삭제
```

<li>쉘 실행</li>

```bash
$ ./eco-shell
```

<li>쉘 종료</li>

```bash
$ exit
```
</ol>

<h4>3. 기능</h4>
<ol>
    <li>power 명령</li>
        <ul>
            <li>-i: 현재 전력 소모량, 배터리 잔량, 남은 배터리 사용 시간, 배터리 상태, 배터리 모델명 표시</li>
            <li>-r: power 명령어 사용 기록 확인</li>
            <li>-c: 쉘 초기 실행 이후 전력 소모량 비교. 컴퓨터 사용 시간, 배터리 소모량, 배터리 소모 속도, 순간 전력 소모량 비교, 탄소 배출량 표시</li>
            <li>-h: 도움말 표시</li>
        </ul>
    <li>fclean 명령</li>
        <ul>
            <li>-t: 임시파일 삭제</li>
            <li>-l: 로그파일 삭제</li>
            <li>-c: 캐시 파일 삭제</li>
            <li>-h: 도움말 표시</li>
        </ul>
    <li>power 명령</li>
        <ul>
            <li>-i: 현재 전력 소모량, 배터리 잔량, 남은 배터리 사용 시간, 배터리 상태, 배터리 모델명 표시</li>
            <li>-r: power 명령어 사용 기록 확인</li>
            <li>-c: 쉘 초기 실행 이후 전력 소모량 비교. 컴퓨터 사용 시간, 배터리 소모량, 배터리 소모 속도, 순간 전력 소모량 비교, 탄소 배출량 표시</li>
            <li>-h: 도움말 표시</li>
        </ul>
    <li>pclean 명령</li>
        <ul>
            <li>-r: 1시간 이상 실행된 백그라운드 프로세스 종료</li>
            <li>-h: 도움말</li>
        </ul>
</ol>
<h3>4. 팀구성</h3>
        <ul>
            <li>사재헌 - 깃허브 아이디: Zaixian5</li>
            <li>이준혁 - 깃허브 아이디: visualnnz</li>
        </ul>
