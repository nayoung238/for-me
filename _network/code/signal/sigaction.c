#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig);
int main(){
    
    pid_t pid;
    struct sigaction act;
    int state;
    
    act.sa_handler = read_childproc;
    
    // 시그널 집합 변수의 모든 내용 삭제 <-> sigfillset (모든 시그널 추가)
    // sa_mask, sa_flags 는 좀비 소멸 목적으로 사용되지 않기 때문에 모두 0으로 초기화한다.
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    sigaction(SIGCHLD, &act, 0);
    
    pid = fork();
    if(pid == 0) { // child1
        return 9;
    }
    else {
        wait(&state); // child1 보다 부모가 먼저 종료되는 것을 방지
        pid = fork();
        if(pid == 0) { // child2
            return 15;
        }
        wait(&state); // child2 보다 부모가 먼저 종료되는 것을 방지
    }
    return 0;
}
void read_childproc(int sig) {
    int state;
    pid_t child_id;
   
    child_id = waitpid(-1, &state, WNOHANG);
    
    // waitpid()는 while문으로 계속 체크해야 한다고 배웠는데
    // 아래와 같이 작성시 block 상태가 됨
    // 211203: 해결못함
    /*
    while(child_id = waitpid(-1, &state, WNOHANG)) {
        sleep(3);
        puts("sleep 3sec");
    }*/

    if(WIFEXITED(state)){
        printf("child id : %d\n", child_id);
        printf("child return value : %d\n", WEXITSTATUS(state));
    }
}
