# Blocking mode 파일의 read()

- 읽을 데이터가 없는 경우 읽을 데이터를 받을 때까지 block 상태가 됨
- 읽은 데이터의 크기를 return
- 에러가 발생된 경우 return -1

## 일반 파일

- 읽을 데이터에 EOF가 포함된 경우 EOF까지 읽은 데이터 수를 return
  - BUF_SIZE 보다 작은 값이 return 될 수 있음
- 읽을 데이터가 EOF밖에 없는 경우 0을 return 하며 해당 경우는 **error 아님!!**
- read buffer에 있는 데이터가 BUF_SIZE보다 작은 경우 일단 기다린다. -> block

예를 들면 영어 147글자인 일반 파일을 receiver가 **10byte씩 읽는 경우** return 되는 과정은 다음과 같다. (영어는 1글자당 1byte)<br>

- 10byte x 14번 read -> ```return 10```이 14번 발생
- 마지막 7byte -> ```return 7```이 1번 발생
- EOF -> ```return 0``` (**error 아님!!**)

## 느린 파일(특수 파일)

소켓, 파이프 같은 느린 파일은 읽도록 지정한 byte 보다 작은 값이 return 될 수 있다.<br>

- 실제 버퍼에 있는 데이터만 읽기 때문에 BUF_SIZE 보다 작은 값이 return 될 수 있음
  - 일반 파일에서는 이런 일이 발생하지 않음 (읽도록 지정한 크기만큼 read)
- 읽을 데이터에 EOF가 포함된 경우 EOF까지 읽은 데이터 수를 return
- 읽을 데이터가 EOF 밖에 없는 경우 0을 return 하며 해당 경우는 **error 아님!!**

일반 파일과 느린 파일의 차이는 버퍼에 읽을 수 있는 데이터의 크기 뿐이다.<br>
느린 파일도 EOF를 읽어야 끝나는데 상대가 close(socket) 하면 EOF가 날라온다.<br>

즉, 느린파일은 read buffer에 있는 만큼 읽고, 없으면 기다린다.<br><br>

# Non-blocking mode 파일의 read()

- 읽을 데이터가 있는 경우 읽은 데이터의 크기를 return
- 읽을 데이터가 없는 경우 block 되는 blocking 모드와 다르게 ```0 또는 -1```을 return
  - return 0 : read pointer가 EOF에 있는 경우 -> ```error 아님!!```
  - return -1 : EOF까지 읽은 상태이거나 어떠한 이유로 읽을 수 없는 경우 -> ```error !!```
  - 0 또는 -1을 return 하고 다른 일을 수행
- non-blocking mode는 일반 파일에서 거의 사용되지 않고 소켓, 파이프 같은 느린 파일에서 사용

## Non-blocking mode 설정

- open()에 O_NONBLOCK 옵션을 설정하면 non-blocking 모드로 열림
- 일반적으로 아무런 설정이 없을 경우 blocking 모드로 열림
- 추후에 fctl()에 O_NONBLOCK 옵션으로 모드를 변경할 수 있음