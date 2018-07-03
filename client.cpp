#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define IP "127.0.0.1"

using namespace std;

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

int main() { 

    cout << "Bem vindo à central de auto-atendimento Airton Sena" << endl << endl;
    cout << "Por favor, selecione a opção desejada no menu abaixo;" << endl << endl;





    UserMenu();

}