# Blocking mode 파일의 read()

- 읽을 데이터가 없는 경우 읽을 데이터를 받을때까지 block 된다.
- 읽은 데이터의 크기를 return 한다.
- 에러가 발생된 경우 return -1

## 일반 파일

- 읽을 데이터에 EOF가 포함된 경우 EOF까지 읽은 데이터 수를 return 한다.
- 읽을 데이터가 EOF밖에 없는 경우 0을 return하며 해당 경우는 ```error가 아니다.```

예를 들면 영어 147글자인 일반 파일을 receiver가 10byte씩 읽는 경우 return 되는 과정은 다음과 같다.(영어는 1글자당 1byte)<br

- 10byte x 14번 read -> ```return 10```이 14번 발생
- 마지막 7byte -> ```return 7```이 1번 발생
- EOF -> ```return 0``` (에러 아님)


## 느린 파일(특수 파일)

소켓, 파이프 같은 느린 파일은 읽도록 지정한 byte보다 작은 값이 return될 수 있다.<br>

- 실제 버퍼에 있는 데이터만 읽는데 일반 파일에서는 이런 일이 발생하지 않는다.
- 읽을 데이터에 EOF가 포함된 경우 EOF까지 읽은 데이터 수를 return 한다.
- 읽을 데이터가 EOF밖에 없는 경우 0을 return하며 해당 경우는 ```error가 아니다.```

일반 파일과 느린 파일의 차이는 버퍼에 읽을 수 있는 데이터의 크기 뿐이다.<br>
느린 파일도 EOF를 읽어야 끝나는데 상대가 close(socket) 하면 EOF가 날라온다.<br>


# Non-blocking mode 파일의 read()

- 읽을 데이터가 있는 경우 읽은 데이터의 크기를 return 한다.
- 읽을 데이터가 없는 경우 block 되는 blocking 모드와 다르게 ```0 또는 1```을 return 한다.
  - return 0 : read pointer가 EOF에 있는 경우 -> ```에러 아님!!```
  - return -1 : EOF까지 읽은 상태이거나 어떠한 이유로 읽을 수 없는 경우 -> ```에러 !!```
  - 0 또는 1을 return 하고 댜른 일을 수행한다.
- non-blocking mode는 일반 파일에서 거의 사용되지 않고 소켓, 파이프 같은 느린 파일에서 사용된다.
