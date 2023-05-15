#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <sys/wait.h> // waitpid

int main(){

    pid_t pid = fork();
    int state;

    if(pid == 0) {
        puts("Hi! im child");
        sleep(8);
        return 3;
    }
    else {
        while(!waitpid(-1, &state, WNOHANG)) {
            sleep(2);
            puts("sleep 2sec");
        }
        if(WIFEXITED(state))
            printf("child return value : %d\n", WEXITSTATUS(state));
    }
    return 0;
}
/*
Hi! im child
sleep 2sec
sleep 2sec
sleep 2sec
sleep 2sec
child return value : 3
*/