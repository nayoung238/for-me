#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit

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
            puts("Hi! im child2");
            exit(7);
        }
        else {
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
/*
child1 id : 5935
Hi! im child1
child2 id: 5936
child return : 3
Hi! im child2
child return : 7
*/