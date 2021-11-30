## 파일 디스크립터

- 리눅스는 소켓도 파일로 간주
- 저 수준 파일 입출력 함수를 기반으로 소켓 기반의 데이터 송수신 가능
- 파일 디스크립터 : os가 만든 파일(소켓) 을 구분하기 위한 숫자
- 저 수준 파일 입출력 함수는 입출력을 목적으로 파일 디스크립터를 요구
- 저 수준 파일 입출력 함수에게 소켓의 파일 디스크립터를 전달하면 해당 소켓의 입출력을 진행
<br>

- 0 : standard input
- 1 : standard output
- 2 : standard error
<br>

## 파일 open

```c
#include <fcntl.h>

int open(const char *path, int flag);
// 성공 시 파일 디스크립터, 실패 시 -1 return
```

- path : 파일이름을 나타내는 문자열의 주소 값
- flag : 파일의 오픈 모드

### 오픈 모드

- O_CREAT : 필요하면 파일 생성
- O_TRUNC : 기존 데이터 전부 삭제
- O_APPEND : 기존 데이터 모두 보존하고 이어서 저장
- O_RDONLY : 읽기 전용으로 파일 오픈
- O_WRONLY : 쓰기 전용으로 파일 오픈
- O_RDWR : 읽기, 쓰기 겸용으로 파일 오픈

```c
int fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC)
```

- data.txt 파일이 없다면 생성
- data.txt 파일이 이미 존재한다면 기존 데이터 모두 삭제
<br>

## 파일 close

```c
#include <fcntl.h>

int close(int fd);
// 성공 시 0, 실패 시 -1 return
```

- fd : 닫고자 하는 파일 또는 소켓의 파일 디스크립터
<br>

## 쓰기 write

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t nbytes);
// 성공 시 전달한 바이트 수, 실패 시 -1 return
```

- fd : 데이터 전송대상을 나타내는 파일 디스크립터
- buf : 전송할 데이터가 저장된 버퍼의 주소 값
- nbytes : 전송할 데이터의 바이트 수
<br>

## 읽기 read

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes);
// 성공 시 수신한 바이트 수(파일 끝은 0), 실패 시 -1 return
```

- fd : 데이터 수신 대상을 나타내는 파일 디스크립터
- buf : 수신한 데이터를 저장할 버퍼의 주소 값
- nbytes : 수신할 최대 바이트 수
<br>

## 실습

코드 : [https://github.com/evelyn82/network/tree/master/code/fileopen/fileopen.c](https://github.com/evelyn82/network/tree/master/code/fileopen/fileopen.c)
