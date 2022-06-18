#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <sys/wait.h>

int main(){

    pid_t pid = fork();
    int state;

    if(pid == 0) {
        puts("Hi! im child1");
        return 3;
    }
    else {
        printf("child1 id : %d\n", pid);

        pid = fork();
        if(pid == 0) {
            exit(7);
        }
        else {
            puts("Hi! im child2");
            printf("child2 id : %d\n", pid);

            wait(&state);
            if(WIFEXITED(state))
                printf("child return value : %d\n", WEXITSTATUS(state));

            wait(&state);
            if(WIFEXITED(state))
                printf("child return value : %d\n", WEXITSTATUS(state));
        }
    }
    return 0;
}