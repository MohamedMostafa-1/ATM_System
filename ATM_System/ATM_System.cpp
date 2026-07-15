
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void ShowATMMainMenue();
void ShowNormalWithdrawScreen();
void ShowQuickWithdrawScreen();
void Login();

struct stUser
{
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false;
};
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
enum enATMMainMenuOptions
{
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposit = 3,
    eCheckBalances = 4,
    eLogout = 5
};

stUser CurrentUser;
sClient CurrentClient;

int ReadNumberFromTo(int From, int To) {
    int Num=0;

    while (Num < From || Num >To)
    {
        cout << "\nEnter Number From " << From << " To " << To << ": ";
        cin >> Num;

    } 

    return Num;
}

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
stUser ConvertUserLinetoRecord(string Line, string Seperator = "#//#")
{

    stUser User;
    vector<string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;

}
stUser ConvertUserLinetoRecord2(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData;

    vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;

}
string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += User.UserName + Seperator;
    stClientRecord += User.Password + Seperator;
    stClientRecord += to_string(User.Permissions);

    return stClientRecord;

}
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (stUser U : vUsers)
        {

            if (U.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vUsers;

}


vector <stUser> LoadUsersDataFromFile(string FileName)
{

    vector <stUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertUserLinetoRecord(Line);

            vUsers.push_back(User);
        }

        MyFile.close();

    }

    return vUsers;

}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
void GoBackToATMMainMenu()
{
    cout << "\n\nPress any key to go back to ATM Main Menue...";
    system("pause>0");
    ShowATMMainMenue();
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient& Client)
{

    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClient)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }

    }
    return false;

}


//Deposit
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
void ShowDepositScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    double Amount = 0;
    cout << "\nEnter Amount : ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber , Amount , vClient);
    CurrentClient.AccountBalance += Amount;
}


//Total Balance
void ShowCheckBalanceScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\t Total Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "ToTal Balance is: " << CurrentClient.AccountBalance << endl;
};

// Quick Withdraw
short GetWithdrawBalance(short QuickWithDrawOption) {
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}
void PerfromQuickWithdrawOption(int QuickWithdrawOption) {
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawBalance = GetWithdrawBalance(QuickWithdrawOption);
    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }
    vector <sClient> vClients =
        LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,WithdrawBalance * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawBalance;
}
void ShowQuickWithdrawScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\t Quick Withdraw";
    cout << "\n-----------------------------------\n";
    cout << "[1]10 \t\t [2]50 \n";
    cout << "[3]100 \t\t [4]200 \n";
    cout << "[5]500 \t\t [6]600 \n";
    cout << "[7]800 \t\t [8]1000 \n";
    cout << "[9]Exit ";
    cout << "\n-----------------------------------\n";

    PerfromQuickWithdrawOption(ReadNumberFromTo(1, 9));

}

// Normal Withdraw
int ReadWithDrawAmont() {
    int Amount = 0;
    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;

    while (Amount % 5 != 0) {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    }

    return Amount;
}
void NormalWithdraw() {
    int Amount = ReadWithDrawAmont();
    double TotalBalance = CurrentClient.AccountBalance;

    while (Amount > TotalBalance) {
        cout << "\n" << Amount << " is larger than " << TotalBalance << endl;
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
    }
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClient);
    CurrentClient.AccountBalance -= Amount;
}
void ShowNormalWithdrawScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\t Normal Withdraw";
    cout << "\n-----------------------------------\n";
    NormalWithdraw();
}


void PerformATMMainMenuOption(enATMMainMenuOptions ATMMainMenuOption)
{
    switch (ATMMainMenuOption)
    {
    case enATMMainMenuOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eCheckBalances:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATMMainMenu();
        break;

    case enATMMainMenuOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}
void ShowATMMainMenue() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balances.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerformATMMainMenuOption((enATMMainMenuOptions)ReadNumberFromTo(1 ,5));
}


bool  LoadClientInfo(string AccountNumber, string PinCode)
{

    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;

}
void Login()
{
    bool LoginFaild = false;

    string AccountNumber, PinCode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Username/Password!\n";
        }

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowATMMainMenue();

}


int main()
{
	Login();
}

