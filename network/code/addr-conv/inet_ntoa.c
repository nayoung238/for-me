#include <stdio.h>
#include <arpa/inet.h>

int main(){
    
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    
    addr1.sin_addr.s_addr = htonl(0x01020304);
    addr2.sin_addr.s_addr = htonl(0x01010101);
    
    str_ptr = inet_ntoa(addr1.sin_addr);
    printf("Dotted-decimal notation: %s\n", str_ptr);
    
    str_ptr = inet_ntoa(addr2.sin_addr);
    printf("Dotted-decimal notation: %s\n", str_ptr);
}
