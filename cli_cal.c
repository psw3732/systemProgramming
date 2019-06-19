#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char* message);

int main(int argc, char* argv[])
{
        int sock;
        struct sockaddr_in serv_addr;
        char opmsg[BUF_SIZE];
        int result, opnd_cnt, i;

        if(argc!=3)
        {
               printf("Usage : %s <IP> <port> \n", argv[0]);
               exit(1);
        }
        sock=socket(PF_INET, SOCK_STREAM, 0);

        if(sock==-1)
               error_handling("socket() error");
       
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
        serv_addr.sin_port=htons(atoi(argv[2]));

        if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
               error_handling("connect() error");

        fputs("oper count : ", stdout);
        scanf("%d", &opnd_cnt);
        // 피연산자의갯수를입력
        opmsg[0]=(char)opnd_cnt;
        // 피연산자의갯수를프로토콜첫번째바이트에입력
        for(i=0; i<opnd_cnt; i++)
        {
               printf("operand %d : ", i+1);
               scanf("%d", (int*)&opmsg[i*OPSZ+1]);
               // 피연산자갯수만큼피연산자를입력
               // 피연산자한개당4바이트의공간을갖음
        }
        fgetc(stdin);
        fputs("operator : ", stdout);
        scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
        //마지막피연산자다음1바이트에연산자입력
        write(sock, opmsg, opnd_cnt*OPSZ+2);
        //서버에게프로토콜을전송
        read(sock, &result, RLT_SIZE);
        //결과값을전송받음
        printf("result = %d\n\n", result);
        close(sock);
        return 0;
}
void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
