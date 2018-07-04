// testecli.cpp : Defines the entry point for the console application.
//
#include <iostream>
    using std::cout;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#define	SOCKET	int
	#define INVALID_SOCKET  ((SOCKET)~0)
#endif


#define PORTA_CLI 2345 // porta TCP do cliente
#define PORTA_SRV 2023 // porta TCP do servidor
#define STR_IPSERVIDOR "192.168.0.4"
//#define STR_IPSERVIDOR "192.168.0.146"

int main(int argc, char* argv[])
{
    SOCKET s;
    struct sockaddr_in  s_cli, s_serv;
    int portaSRV, portaCli;
    char ip[16];
    time_t start, end, tempo;
    int pacotes = 0;
    float taxa;

#ifdef _WIN32
    WSADATA wsaData;
  
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("Erro no startup do socket\n");
		exit(1);
	}
#endif

    if(argc < 7) {
        cout << "Usage: ./testecli -h <ip> -p <portaSRV> -k <portaCli>";
        exit(1);
    }

    for(int i=1; i<argc; i++) {
        if(argv[i][0]=='-') {
            switch(argv[i][1]) {
            case 'h': // Numero IP
                i++;
                strcpy(ip, argv[i]);
            break;               
            case 'p': // portaSRV
                i++;
                portaSRV = atoi(argv[i]);
                if(portaSRV < 1024) {
                    cout << "Invalid port\n";
                    exit(1);
                }
            break;
            case 'k':
                i++;
                portaCli = atoi(argv[i]);
                if(portaCli < 1024){
                    cout << "Invalid port\n";
                    exit(1);
                }
            break;
            }
        }
    }

  

  // abre socket TCP
    if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
    {
        printf("Erro iniciando socket\n");
        return(0);
    }

  // seta informacoes IP/Porta locais
    s_cli.sin_family = AF_INET;
    s_cli.sin_addr.s_addr = htonl(INADDR_ANY);
    s_cli.sin_port = htons(portaCli);

  // associa configuracoes locais com socket
    if ((bind(s, (struct sockaddr *)&s_cli, sizeof(s_cli))) != 0)
    {
        printf("erro no bind\n");
        close(s);
        return(0);
    }

  // seta informacoes IP/Porta do servidor remoto
    s_serv.sin_family = AF_INET;
    s_serv.sin_addr.s_addr = inet_addr(ip);
    s_serv.sin_port = htons(portaSRV);

  // connecta socket aberto no cliente com o servidor
    if(connect(s, (struct sockaddr*)&s_serv, sizeof(s_serv)) != 0)
    {
        //printf("erro na conexao - %d\n", WSAGetLastError());
        printf("erro na conexao");
        close(s);
        exit(1);
    }

#if 0
  // envia mensagem de conexao - aprimorar para dar IP e porta
  if ((send(s, "Conectado\n", 11,0)) == SOCKET_ERROR);
  {
    printf("erro na transmissão - %d\n", WSAGetLastError());
    closesocket(s);
    return 0;
  }
#endif

    char recvbuf[10];
    char str[1250] = "Teste novo";
    char ch;
    int i;

     if ((send(s, (const char *)&str, sizeof(str),0)) < 0)
        {
            printf("erro na transmissao\n");
            close(s);
            return 0;
        }
    if((recv(s, recvbuf, 1250, 0))){
        printf("%s\n", recvbuf);
    }


  // fecha socket e termina programa
    printf("Fim da conexao\n");
    close(s);
    return 0;
}
