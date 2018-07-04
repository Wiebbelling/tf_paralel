#include <iostream>
	using std::cout;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iomanip>
#include <vector>
	using namespace std;
#define	SOCKET	int
#define INVALID_SOCKET  ((SOCKET)~0)

#define MAX_PACKET 1250
#define PORTA_SRV 8080 //2023 

enum erros {WSTARTUP, ABRESOCK, BIND, ACCEPT, LISTEN,RECEIVE};

void TrataErro(SOCKET, int);

class AutoTellerMachine {         //Object to represent each customer                  who uses the ATM program
    public:
    void CreateNewAccount(string newUsername, string newPassword);
    int AccountLogin(string loginUsername, string loginPassword);
    void DepositMoney(double depositAmount);
    void WithdrawMoney(double withdrawalAmount);
    void SetAccountLogin(int setAccountLocation);
    void SetLastMoneyMovement(int accountID, double amount);
    void SetBeginningBalance(int accountID);
    void SetLastOperation(int accountID, char userInput);
    void AccountMenu();
    int GetAccountLogin() const;
    double GetLastMoneyMovement(int accountID) const;
    double GetAccountBalance(int accountID) const;
    double GetBeginningBalance(int accountID) const;
    char GetLastOperation(int accountID) const;
    string GetUsername(int accountID) const;

    private:
    int loggedInAccountLocation;
    double accountBalance;
    double beginningBalance;
    double lastMoneyMovement;
    char lastOperation;
    string username;
    string password;
};

AutoTellerMachine account;

vector<AutoTellerMachine> AccountList;  //This vector allows for multiple accounts to be stored so that if more than one person uses the account, all information is retained

void AccountMenu();
void UserMenu();

void AutoTellerMachine:: SetAccountLogin(int setAccountLocation) {

    loggedInAccountLocation = setAccountLocation;

    return;
}

int AutoTellerMachine::GetAccountLogin() const {

    return loggedInAccountLocation;
}

void AutoTellerMachine::CreateNewAccount(string newUsername, string newPassword) {    //Adds the new information to the vector to give the account personalized info

    int accountListSize = AccountList.size();

    AccountList.at(accountListSize - 1).accountBalance = 0.0;
    AccountList.at(accountListSize - 1).username = newUsername;
    AccountList.at(accountListSize - 1).password = newPassword;

}

int AutoTellerMachine::AccountLogin(string loginUsername, string loginPassword) {

    int accountListSize = AccountList.size();
    bool successfulLogin = false;
    int accountLocation = 0;

    for(int i = 0; i < accountListSize; i++) {

        if(loginUsername == AccountList.at(i).username) {

            if(loginPassword == AccountList.at(i).password) {

                successfulLogin = true;
                accountLocation = i;
            }
        }                
    }

    if(successfulLogin != true) {

        return 1;
    }

    else{//if(successfulLogin == true) {

        SetAccountLogin(accountLocation);
        cout << endl << "Access Granted - " << AccountList.at(loggedInAccountLocation).username << endl;
        //AccountMenu();
        return 0;
    }

}

void AutoTellerMachine::DepositMoney(double depositAmount) {

    AccountList.at(loggedInAccountLocation).accountBalance += depositAmount;

    return;
}

void AutoTellerMachine::WithdrawMoney(double withdrawalAmount) {

    AccountList.at(loggedInAccountLocation).accountBalance -= withdrawalAmount;

    return;
}

double AutoTellerMachine::GetAccountBalance(int accountID) const {

    return AccountList.at(loggedInAccountLocation).accountBalance;
}

void AutoTellerMachine::SetLastMoneyMovement(int accountID, double amount) {

    AccountList.at(accountID).lastMoneyMovement = amount;
}

void AutoTellerMachine::SetBeginningBalance(int accountID) {

    AccountList.at(accountID).beginningBalance = AccountList.at(loggedInAccountLocation).accountBalance;
}

void AutoTellerMachine::SetLastOperation(int accountID, char userInput) {

    AccountList.at(accountID).lastOperation = userInput;
}

double AutoTellerMachine::GetLastMoneyMovement(int accountID) const {

    return AccountList.at(accountID).lastMoneyMovement;
}

double AutoTellerMachine::GetBeginningBalance(int accountID) const {

    return AccountList.at(accountID).beginningBalance;
}

char AutoTellerMachine::GetLastOperation(int accountID) const {

    return AccountList.at(accountID).lastOperation;
}

string AutoTellerMachine::GetUsername(int accountID) const {

    return AccountList.at(GetAccountLogin()).username;
}

void UserMenu() {   //Implements a user interface that allows the user to make selections based on what they want to do

    char userSelection;
    string createUserId;
    string createUserPass;
    string usernameCheck;
    string passwordCheck;

    cout << "l -> Login" << endl;
    cout << "c -> Create New Account" << endl;
    cout << "q -> Quit" << endl << endl << ">";
    cin >> userSelection;

    if((userSelection == 'l') || (userSelection == 'L')) {  //Checks to make sure the login is valid and if not, couts an error statement

        cout << endl << "Please enter your user name: " << endl;
        cin >> usernameCheck;
        cout << "Please enter your password: " << endl;
        cin >> passwordCheck;

        account.AccountLogin(usernameCheck, passwordCheck);

    }

    else if((userSelection == 'c') || (userSelection == 'C')) {  //Captures info for a new account

        cout << endl << "Please enter your user name: " << endl;
        cin >> createUserId;
        cout << "Please enter your password: " << endl;
        cin >> createUserPass;

        AccountList.push_back(account);            //This creates a new object in the vector to be filled with the information gathered

        account.CreateNewAccount(createUserId, createUserPass);

        cout << endl << "Thank You! Your account has been created!" << endl << endl;

        UserMenu();
    }

    else if((userSelection == 'q') || (userSelection == 'Q')) {   //Exits the entire program

        cout << endl << "You selected quit!" << endl << endl;
    }

    else {

        cout << endl << "Invalid selection." << endl;
        UserMenu();
    }

    return;
}

void AutoTellerMachine::AccountMenu() {         //This is a separate menu from the user menu because it deals with all options available to a logged in customer

    char userInput;
    double amountOfDeposit;
    double amountOfWithdrawal;

    cout << endl << "d -> Deposit Money" << endl;   //This has a couple more options than indicated in our project overview, but I feel they make this a more useable program
    cout << "w -> Withdraw Money" << endl;
    cout << "r -> Request Balance" << endl;
    cout << "z -> Logout" << endl;
    cout << "q -> Quit" << endl;
    cout << endl << ">";
    cin >> userInput;

    if((userInput == 'd') || (userInput == 'D')) {      //Deposit function that changes the balance of the account user and sets the last money movement for later use

        SetBeginningBalance(GetAccountLogin());
        cout << endl <<  "Amount of deposit: " << endl;
        cin >> amountOfDeposit;
        SetLastMoneyMovement(GetAccountLogin(), amountOfDeposit);
        SetLastOperation(GetAccountLogin(), userInput);
        DepositMoney(amountOfDeposit);
        AccountMenu();
    }        

    else if((userInput == 'w') || (userInput == 'W')) {   //Withdraw function makes sure that enough funds are present for the operation before removing money

        cout << endl << "Amount of withdrawal: " << endl;
        cin >> amountOfWithdrawal;

        if(amountOfWithdrawal > GetAccountBalance(GetAccountLogin())) {

            cout << endl << "******Insfficient Funds!*******" << endl;
        }

        else {

           SetBeginningBalance(GetAccountLogin());
           SetLastMoneyMovement(GetAccountLogin(), amountOfWithdrawal);
           SetLastOperation(GetAccountLogin(), userInput);
           WithdrawMoney(amountOfWithdrawal);
        }

        AccountMenu();
    }

    else if((userInput == 'r') || (userInput == 'R')) {   //Simply prints the balance before the last transaction, what type and amount the last transaction was then the current balance

        cout << endl << "Beginning balance: $" << fixed << setprecision(2) << GetBeginningBalance(GetAccountLogin()) << endl;

        if(GetLastOperation(GetAccountLogin()) == 'd') {

            cout << "Deposit amount: $" << fixed << setprecision(2) << GetLastMoneyMovement(GetAccountLogin()) << endl;
        }

        else if(GetLastOperation(GetAccountLogin()) == 'w') {

            cout << "Withdrawal amount: $" << fixed << setprecision(2) << GetLastMoneyMovement(GetAccountLogin()) << endl;
        }

        cout << "Your balance is $" << fixed << setprecision(2) << GetAccountBalance(GetAccountLogin()) << endl;

        AccountMenu();
    }

    else if((userInput == 'z') || (userInput == 'Z')) {   //Allows the user to logout of their account and brings them back to the user menu so they can log in with a different account

        cout << endl << "You have successfully logged out, " << GetUsername(GetAccountLogin()) << "!" << endl << endl;
        UserMenu();
    }

    else if((userInput == 'q') || (userInput == 'Q')) {  //Exits the entire program

        cout << endl << "Thanks for banking with COP2513.F16, " << GetUsername(GetAccountLogin()) << "!" << endl;
    }

    else {

        cout << endl << "Invalid selection." << endl;
        AccountMenu();
    }

    return;
}

int main(int argc, char* argv[])
{
	SOCKET s=0, s_cli;
 	struct sockaddr_in  addr_serv, addr_cli;
 	socklen_t addr_cli_len=sizeof(addr_cli);
 	int porta;

 	if(argc < 3) {
		cout << "Usage: ./testesrv -p <port>";
		exit(1);
	}
	for(int i=1; i<argc; i++) {
		if(argv[i][0]=='-') {
			switch(argv[i][1]) {				 
			case 'p': // porta
				i++;
				porta = atoi(argv[i]);
				if(porta < 1024) {
				cout << "Invalid port\n";
				exit(1);
				}
			break;
			}
		}
	}
	char recvbuf[MAX_PACKET];

	// Cria o socket na familia AF_INET (Internet) e do tipo TCP (SOCK_STREAM)
	if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET){
		TrataErro(s, ABRESOCK);	
	}

	// Define domínio, IP e porta a receber dados
	addr_serv.sin_family = AF_INET;
  	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); // recebe de qualquer IP
  	addr_serv.sin_port = htons(porta);

  	// Associa socket com estrutura addr_serv
  	if ((bind(s, (struct sockaddr *)&addr_serv, sizeof(addr_serv))) != 0){
		TrataErro(s, BIND);
  	}

  	// Coloca socket em estado de escuta para as conexoes na porta especificada
  	if((listen(s, 8)) != 0){ // permite ateh 8 conexoes simultaneas
	  	TrataErro(s, LISTEN);
	}

	// permite conexoes entrantes utilizarem o socket
  	if((s_cli=accept(s, (struct sockaddr *)&addr_cli, (socklen_t *)&addr_cli_len)) < 0){
	  	TrataErro(s, ACCEPT);
  	}

  	char ack[10] = "0";
  	char nack[10] = "1";
  	std::string delimiter = "|";
  	size_t pos = 0;
  	std::vector<std::string> parsed;
  	std::string NewStr;

  	while(1)
  	{	
  		memset(recvbuf, '\0', 1024);
  		if((recv(s_cli, recvbuf, MAX_PACKET, 0)) > 0){
  			printf("Message Received: %s\n", recvbuf);
  			switch(recvbuf[0]){
  				case 'q':
  					if ((send(s_cli, (const char *)&ack, sizeof(ack),0)) > 0)
  						printf("ACK SENDED\n");
  					break;
  				case 'l':
  					NewStr.assign(recvbuf, 1024);
  					while((pos = NewStr.find(delimiter)) != std::string::npos) {
  						parsed.push_back(NewStr.substr(0, pos));
  						NewStr.erase(0, pos + delimiter.length());
  					}
  					//std::cout << parsed[0] << parsed[1] << parsed[2] << parsed.size() << std::endl;
  					if(account.AccountLogin(parsed[1], parsed[2])==1){
  						if ((send(s_cli, (const char *)&nack, sizeof(nack),0)) > 0)
  							printf("NACK SENDED\n");
  					}
  					else{
  						if ((send(s_cli, (const char *)&ack, sizeof(ack),0)) > 0)
  							printf("ACK SENDED\n");
  					}
  					break;

  				default:
  					break;
  			}
  		}
    	if (recv(s_cli, recvbuf, MAX_PACKET, 0))
    	{
      		close(s_cli);
	  		TrataErro(s, RECEIVE);
    	}
    }

  printf("Fim da conexao\n");
  close(s);
  close(s_cli);
  exit(1);
}

void TrataErro(SOCKET s, int tipoerro)
{
	char tipo[20];

	switch(tipoerro) {
		case WSTARTUP:
			strcpy(tipo, "Windows Startup");
			break;
		case ABRESOCK:
			strcpy(tipo, "Open Socket");
			break;
		case BIND:
			strcpy(tipo, "Bind");
			break;
		case ACCEPT:
			strcpy(tipo, "Accept");
			break;
		case LISTEN:
			strcpy(tipo, "Listen");
			break;
		case RECEIVE:
			strcpy(tipo, "Receive");
			break;
		default:
			strcpy(tipo, "Indefinido. Verificar");
			break;
	}
    printf("erro no %s", tipo);
    close(s);
    exit(1);
}
