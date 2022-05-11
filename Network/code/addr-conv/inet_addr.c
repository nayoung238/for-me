#include <stdio.h>
#include <arpa/inet.h> // inet_addr, inet_aton

void conv_result_print(unsigned long addr);
int main(){
    
    char *properAddr = "1.2.3.4";
    char *improperAddr = "1.2.3.256";
    
    unsigned long conv_addr = inet_addr(properAddr);
    conv_result_print(conv_addr);
    
    conv_addr = inet_addr(improperAddr);
    conv_result_print(conv_addr);
}
void conv_result_print(unsigned long conv_addr){
    if(conv_addr == INADDR_NONE)
        printf("Error occured!\n");
    else
        printf("Network ordered integer addr: %#lx\n", conv_addr);
}
