#include <stdio.h>
#include <unistd.h> // fork
#include <stdlib.h> // exit
#include <sys/wait.h>

void timeout(int sig);
void keycontrol(int sig);

int main(){
    
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);
    
    for(i = 0; i < 3; ++i) {
        puts("wait...");
        sleep(20);
    }
    return 0;
}
void timeout(int sig) {
    if(sig == SIGALRM)
        puts("Time-out!");
    alarm(2);
}
void keycontrol(int sig){
    if(sig == SIGINT)
        puts("CTRL + C pressed");
}
