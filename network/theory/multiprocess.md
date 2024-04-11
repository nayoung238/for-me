# 다중 접속 서버

다중 접속 서버란 둘 이상의 클라이언트에게 동시 접속을 허용해 서비스를 제공하는 서버를 말한다.

- multi process 기반 서버 : fork()로 자식 프로세스를 생성해 서비스 제공
- multi thread 기반 서버 : 클라이언트의 수만큼 쓰레드를 생성해 서비스 제공 (stack만 따로 사용하고 나머지는 부모와 공유하기 때문에 multiprocess 보다 좋은 방식)
- multiplexing 기반 서버 : 하나의 프로세스가 file descriptor 로 여러 입출력을 묶어서 관리하는 방식으로 서비스 제공

// 내용 추가 예정

## echo server

- multiprocess 기반 echo server : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/multiprocess/mp_echo_serv.c


## echo client

- echo client : https://github.com/evelyn82ny/for-developer/blob/master/Network/code/multiprocess/mp_echo_client.c