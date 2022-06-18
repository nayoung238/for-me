#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <sys/wait.h>

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
            sleep(3);
            puts("sleep 3sec");
        }
        if(WIFEXITED(state))
            printf("child return value : %d\n", WEXITSTATUS(state));
    }
    return 0;
}