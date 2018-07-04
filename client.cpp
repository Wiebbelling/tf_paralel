#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


using std::cout;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 8080

using namespace std;

// send(sock , hello , strlen(hello) , 0 );
// valread = read( sock , buffer, 1024);
char buffer[1024] = {0};
int sock = 0;
void AccountMenu();

void UserMenu() {   //Implements a user interface that allows the user to make selections based on what they want to do

    
    char userSelection;
    string createUserId;
    string createUserPass;
    string usernameCheck;
    string passwordCheck;
    double amountOfDeposit;
    string command;
    
    int ret = 0;

    cout << "l -> Login" << endl;
    cout << "c -> Criar nova conta" << endl;
    cout << "q -> Sair" << endl << endl << ">";
    cin >> userSelection;

    if((userSelection == 'l') || (userSelection == 'L')) {  //Checks to make sure the login is valid and if not, couts an error statement

        cout << endl << "Por favor, digite seu usuário: " << endl;
        cin >> usernameCheck;
        cout << "Por favor, digite a sua senha: " << endl;
        cin >> passwordCheck;

        command = "l|" + usernameCheck + "|" + passwordCheck + "|";

        send(sock , command.c_str() , command.length() , 0);

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
            {
                cout << "Usuário ou senha incorretos" << endl;
                UserMenu();
            }
            else
                AccountMenu();
        }
        else
        {
            cout << "Erro de comunicação com o servidor" << endl;
            UserMenu();
        }
        
    }
    else if((userSelection == 'c') || (userSelection == 'C')) {  //Captures info for a new account

        cout << endl << "Por favor, digite seu usuário: " << endl;
        cin >> createUserId;
        cout << "Por favor, digite sua senha: " << endl;
        cin >> createUserPass;
        cout << "Por favor, digite o saldo inicial da sua conta: " << endl;
        cin >> amountOfDeposit;

        command = "c|" + createUserId + "|" + createUserPass + "|" + to_string(amountOfDeposit)+ "|";

        send(sock , command.c_str() , command.length() , 0);
        
        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro criando nova conta, por favor, fale com um de nossos gerentes" << endl;
            else
                cout << "Conta criada com sucesso, faça login para verificar seu saldo e efetuar operações" << endl;
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;

        UserMenu();
    }
    else if((userSelection == 'q') || (userSelection == 'Q')) {   //Exits the entire program

        command = "q|";
        send(sock , command.c_str() , command.length() , 0);
        cout << endl << "Saindo da aplicação" << endl << endl;

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro ao fechar comunucacao" << endl;
            else
                cout << "Comunicaçao Finalizada " << endl;
            
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;

    }
    else {

        cout << endl << "Comando inválido" << endl;
        UserMenu();
    }

    return;
}

void AccountMenu() {         //This is a separate menu from the user menu because it deals with all options available to a logged in customer

    char userInput;
    double amountOfDeposit;
    double amountOfWithdrawal;

    string command;
    int ret = 0;

    cout << endl << "d -> Depositar dinherio" << endl;   //This has a couple more options than indicated in our project overview, but I feel they make this a more useable program
    cout << "w -> Retirar dinheiro" << endl;
    cout << "r -> Verificar saldo da conta" << endl;
    cout << "z -> Logout" << endl;
    cout << "q -> Sair" << endl;
    cout << endl << ">";
    cin >> userInput;

    if((userInput == 'd') || (userInput == 'D')) {      //Deposit function that changes the balance of the account user and sets the last money movement for later use
        
        cout << endl <<  "Amount of deposit: " << endl;
        cin >> amountOfDeposit;

        command = "d|" + to_string(amountOfDeposit) + "|";

        send(sock , command.c_str() , command.length() , 0);

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro ao depositar novo valor" << endl;
            else
                cout << "Valor depositado com sucesso" << endl;
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;

        AccountMenu();
    }        
    else if((userInput == 'w') || (userInput == 'W')) {   //Withdraw function makes sure that enough funds are present for the operation before removing money

        cout << endl << "Quantidade à ser retirada: " << endl;
        cin >> amountOfWithdrawal;

        command = "w|"+ to_string(amountOfWithdrawal) + "|";

        send(sock , command.c_str() , command.length() , 0);

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro ao depositar novo valor" << endl;
            else if(buffer[0] == '2')
                cout << endl << "******Saldo insuficiente!*******" << endl;
            else
                cout << "Valor retirado com sucesso" << endl;
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;

        AccountMenu();
    }
    else if((userInput == 'r') || (userInput == 'R')) {   //Simply prints the balance before the last transaction, what type and amount the last transaction was then the current balance

        command = "r|";

        send(sock , command.c_str() , command.length() , 0);

        memset(buffer, 0, strlen(buffer));

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro ao depositar novo valor" << endl;
            else
            {
                string saldoAtual = &buffer[1];
                cout << "Saldo atual: " << saldoAtual << endl;
            }
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;

        AccountMenu();
    }
    else if((userInput == 'z') || (userInput == 'Z')) {   //Allows the user to logout of their account and brings them back to the user menu so they can log in with a different account

        command = "z|";
        send(sock , command.c_str() , command.length() , 0);

        UserMenu();
    }
    else if((userInput == 'q') || (userInput == 'Q')) {  //Exits the entire program

        command = "q|";
        send(sock , command.c_str() , command.length() , 0);
        cout << endl << "Saindo da aplicação" << endl << endl;

        ret = recv(sock , buffer, 1024, 0);

        if(ret > 0)
        {
            if(buffer[0] == '1')
                cout << "Erro ao fechar comunucacao" << endl;
            else
                cout << "Comunicaçao Finalizada " << endl;
            
        }
        else
            cout << "Erro de comunicação com o servidor" << endl;
    }
    else {

        cout << endl << "Comando inválido" << endl;
        AccountMenu();
    }

    return;
}

int main() { 

    struct sockaddr_in address;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Erro de criação de socket \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\n Endereço inválido ou não suportado\n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Conexão falhou \n");
        return -1;
    }

    cout << "Bem vindo à central de auto-atendimento Airton Sena" << endl << endl;
    cout << "Por favor, selecione a opção desejada no menu abaixo;" << endl << endl;

    UserMenu();

}