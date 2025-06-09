#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <windows.h>
using namespace std;

enum enChoiceResulte {
    ShowClientsList = 1,
    AddNewClient = 2,
    DeleteClient = 3,
    UpdateClientInfo = 4,
    FindClient = 5,
    Transactions = 6,
    ManageUsers = 7,
    Logout = 8
};

enum enTransactionsChoice {
    Deposit = 1,
    Withdraw = 2,
    TotalBalance = 3,
    MainMenu = 4
};

enum enManageUsers {
    eListUsers = 1,
    eAddNewUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenu = 6
};

enum enPermissions {
    P_All = -1,
    P_ShowClientsList = 1,
    P_AddNewClient = 2,
    P_DeleteClient = 4,
    P_UpdateClientInfo = 8,
    P_FindClient = 16,
    P_Transactions = 32,
    P_ManageUsers = 64
};

struct stClientData {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
    bool MarkForDeposit = false;
    bool MarkForWithdraw = false;
};

struct stUser {
    string UserName;
    string Password;
    short Permissions;
    bool eMarkForDelete = false;
};

stUser CurrentUser;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

void BackToMainMenu()
{
    cout << "\nPress any key to go back to main menue....";
    system("pause>0");
}

void BackToManageUsersMenu()
{
    cout << "\nPress any key to go back to Transactions menue....";
    system("pause>0");
}

string Under_Scores(int num)
{
    string us = "";
    for (int i = 1; i <= num; i++)
    {
        us += "-";
    }
    return us;
}

void ShowAccessDeniedMessage()
{
    system("cls");
    cout << endl << Under_Scores(38) << endl;
    cout << "Access Denied,\n";
    cout << "You don't have permission to do this,\n";
    cout << "Please contact your admin.\n";
    cout << Under_Scores(38) << endl << endl << endl;
    BackToMainMenu();
}

vector <stClientData> FillLinesFromFileToVectorForClients(string FileName);
vector <string> SplitString(string str, string delim);
stUser ConvertLineToRecordForUsers(string str, string Seperator = "#//#");
vector <stUser> FillLinesFromFileToVectorForUsers(string FileName);

bool CheckAccessPermission(enPermissions Permission)
{
    if (CurrentUser.Permissions == enPermissions::P_All)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}


bool IsClientExist(string AccountNumber)
{
    vector <stClientData> vClient = FillLinesFromFileToVectorForClients(ClientsFileName);

    for (stClientData& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            return true;
        }
    }
    return false;
}

stClientData FillClientData()
{
    stClientData ClientData;
    cout << "Enter Account Number ? ";
    getline(cin >> ws, ClientData.AccountNumber);
    if (IsClientExist(ClientData.AccountNumber))
    {
        do
        {
            cout << "Client with [" << ClientData.AccountNumber << "] already Exists, "
                << "Enter another Account Number? ";
            getline(cin >> ws, ClientData.AccountNumber);
        } while (IsClientExist(ClientData.AccountNumber));
    }
    cout << "Enter PIN Code ? ";
    getline(cin, ClientData.PinCode);
    cout << "Enter Phone Number ? ";
    getline(cin, ClientData.Phone);
    cout << "Enter Name ? ";
    getline(cin, ClientData.Name);
    cout << "Enter Account Balance ? ";
    cin >> ClientData.AccountBalance;
    return ClientData;
}

string ConvertRecordToLineForClients(stClientData Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

void AddDataLineToFile(string ClientData, string FileName)
{
    fstream ClientDataFile;
    ClientDataFile.open(FileName, ios::out | ios::app);
    if (ClientDataFile.is_open())
    {
        ClientDataFile << ClientData << endl;
        ClientDataFile.close();
    }

}

void AddClient()
{
    stClientData ClientData = FillClientData();
    AddDataLineToFile(ConvertRecordToLineForClients(ClientData), ClientsFileName);
}

void AddNewClientsScreenHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Add New Client Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Adding New Client :\n\n";
}

void Add_New_Clients()
{
    if (!CheckAccessPermission(enPermissions::P_AddNewClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    char Check;
    do
    {
        AddNewClientsScreenHeader();
        AddClient();
        cout << "\nClient Added Successfully , Do you want add new client ? y/n ?";
        cin >> Check;

    } while (toupper(Check) == 'Y');
}

vector <string> SplitString(string str, string delim)
{
    vector <string> Takens;
    int pos;
    string word;
    while ((pos = str.find(delim)) != string::npos)
    {
        word = str.substr(0, pos);
        if (word != "")
        {
            Takens.push_back(word);
        }
        str.erase(0, pos + delim.length());
    }
    if (str != "")
    {
        Takens.push_back(str);
    }
    return Takens;
}

stClientData ConvertLineToRecordForClients(string str, string Seperator = "#//#")
{
    stClientData ClientData;
    vector <string> vstr;
    vstr = SplitString(str, Seperator);
    ClientData.AccountNumber = vstr[0];
    ClientData.PinCode = vstr[1];
    ClientData.Name = vstr[2];
    ClientData.Phone = vstr[3];
    ClientData.AccountBalance = stod(vstr[4]);
    return ClientData;
}

vector <stClientData> FillLinesFromFileToVectorForClients(string FileName)
{
    fstream MyFile;
    vector <stClientData> vClients;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLineToRecordForClients(Line));
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientData(stClientData Client)
{
    cout << "| " << setw(17) << left << Client.AccountNumber;
    cout << "| " << setw(11) << left << Client.PinCode;
    cout << "| " << setw(22) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
    cout << endl;
}

void PrintClientsListHeader()
{
    system("cls");
    cout << left;
    cout << "\t\t\t\tClients List \n\n";
    cout << Under_Scores(85) << "\n\n";
    cout << "| " << setw(15) << "Account Number   "
        << "| " << setw(10) << "PIN Code   "
        << "| " << setw(22) << "Client Name           "
        << "| " << setw(12) << "Phone       "
        << "| " << setw(12) << "Balance    \n\n";

    cout << Under_Scores(85) << "\n\n";
}

void Show_Clients_List()
{
    if (!CheckAccessPermission(enPermissions::P_ShowClientsList))
    {
        ShowAccessDeniedMessage();
        return;
    }

    PrintClientsListHeader();
    vector <stClientData> vClient = FillLinesFromFileToVectorForClients(ClientsFileName);
    for (stClientData& Data : vClient)
    {
        PrintClientData(Data);
    }
    BackToMainMenu();
}

void SaveVectorToFileWithDelete(vector <stClientData> vFileContent)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClientData Line : vFileContent)
        {
            if (Line.MarkForDelete == false)
            {

                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
        }
        MyFile.close();
    }
}

void PrintClientCard(stClientData ClientData)
{
    cout << "The Following are the client details : \n";
    cout << Under_Scores(35) << endl;
    cout << "Account Number   : " << ClientData.AccountNumber << endl;
    cout << "Account Balance : " << ClientData.AccountBalance << endl;
    cout << "Name            : " << ClientData.Name << endl;
    cout << "Phone           : " << ClientData.Phone << endl;
    cout << "Pin Code        : " << ClientData.PinCode << endl;
    cout << Under_Scores(35) << endl << endl;

}

bool CheckAccountNumberForDelete(string accnum, vector <stClientData>& vClient, stClientData& Client)
{

    for (stClientData& Data : vClient)
    {
        if (Data.AccountNumber == accnum)
        {
            Data.MarkForDelete = true;
            Client = Data;
            return true;
        }
    }
    return false;
}

string ReadClientAccountNumber()
{
    string accnum;
    cout << "Please Enter Account Number ? ";
    cin >> accnum;
    cout << "\n";
    return accnum;
}

void DeleteClientHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Delete Client Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Delete Client :\n\n";
}

void Delete_Client()
{
    if (!CheckAccessPermission(enPermissions::P_DeleteClient))
    {
        ShowAccessDeniedMessage();
        return;
    }

    DeleteClientHeader();

    string AccountNumber = ReadClientAccountNumber();

    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);

    stClientData Client;
    char Check = 'n';


    if (CheckAccountNumberForDelete(AccountNumber, vClients, Client) == false)
    {
        cout << "Client With Account number (" << AccountNumber << ") Not Found!" << endl;
    }
    else
    {
        PrintClientCard(Client);

        cout << "Are You sure you want delete this client ? y/n ? ";
        cin >> Check;

        if (toupper(Check) == 'Y')
        {
            SaveVectorToFileWithDelete(vClients);

            cout << "\n\nClient Deleted Successfully\n";
        }
    }
    BackToMainMenu();
}

stClientData FillUpdateClientData(string Accnum)
{
    stClientData ClientData;
    ClientData.AccountNumber = Accnum;
    cout << "Enter PIN Code ? ";
    getline(cin >> ws, ClientData.PinCode);
    cout << "Enter Phone Number ? ";
    getline(cin, ClientData.Phone);
    cout << "Enter Name ? ";
    getline(cin, ClientData.Name);
    cout << "Enter Account Balance ? ";
    cin >> ClientData.AccountBalance;
    return ClientData;
}

void SaveVectorToFileWithUpdate(vector <stClientData> vFileContent, stClientData UpdatedData)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClientData Line : vFileContent)
        {
            if (Line.AccountNumber == UpdatedData.AccountNumber)
            {
                MyFile << ConvertRecordToLineForClients(UpdatedData) << endl;
            }
            else
            {
                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
        }
        MyFile.close();
    }
}

bool CheckAccountNumberForUpdate(string accnum, vector <stClientData> vClient, stClientData& Client)
{

    for (stClientData& Data : vClient)
    {
        if (Data.AccountNumber == accnum)
        {
            Client = Data;
            return true;
        }
    }
    return false;
}

void UpdateClientInfoHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Update Client Info Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Update Client Info :\n\n";
}

void Update_Client_Info()
{
    if (!CheckAccessPermission(enPermissions::P_UpdateClientInfo))
    {
        ShowAccessDeniedMessage();
        return;
    }

    UpdateClientInfoHeader();

    string AccountNumber = ReadClientAccountNumber();
    stClientData UpdatedData;

    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);

    stClientData Client;
    char Check = 'n';


    if (CheckAccountNumberForUpdate(AccountNumber, vClients, Client) == false)
    {
        cout << "Client With Account number (" << AccountNumber << ") Not Found!" << endl;
    }
    else
    {
        PrintClientCard(Client);

        cout << "Are You sure you want update this client ? y/n ? ";
        cin >> Check;

        if (toupper(Check) == 'Y')
        {
            UpdatedData = FillUpdateClientData(AccountNumber);
            SaveVectorToFileWithUpdate(vClients, UpdatedData);

            cout << "\n\nClient Updated Successfully\n";
        }
        else
        {
            cout << "\n\nThe Transaction has been cancelled.";
            Sleep(3000);
        }
    }
    BackToMainMenu();
}

void FindClientHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Find Client Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Find Client :\n\n";
}

void Find_Client()
{
    if (!CheckAccessPermission(enPermissions::P_FindClient))
    {
        ShowAccessDeniedMessage();
        return;
    }
    FindClientHeader();

    string AccountNumber = ReadClientAccountNumber();

    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);

    stClientData Client;



    if (CheckAccountNumberForUpdate(AccountNumber, vClients, Client) == false)
    {
        cout << "Client With Account number (" << AccountNumber << ") Not Found!" << endl;
    }
    else
    {
        PrintClientCard(Client);
    }
    BackToMainMenu();
}

void ShowTransactionsScreen()
{
    system("cls");
    cout << "========================================\n";
    cout << "\t    Transactions Menue Screen\n";
    cout << "========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "========================================\n";
}

enTransactionsChoice ReadTransactionsChoice()
{
    int Number = 0;
    do
    {
        cout << "Choose What do you want to do? [1,4]?" << endl;
        cin >> Number;
    } while (Number < 1 || Number > 4);
    return (enTransactionsChoice)Number;
}

void DepositHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Deposit Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "\n\n";
}

bool CheckAccountNumberForTransaction(string accnum, vector <stClientData>& vClient, stClientData& Client)
{

    for (stClientData& Data : vClient)
    {
        if (Data.AccountNumber == accnum)
        {
            Data.MarkForDeposit = true;
            Data.MarkForWithdraw = true;
            Client = Data;
            return true;
        }
    }
    cout << "Client with [" << accnum << "] does not exist.\n\n";
    return false;
}

string ReadClientAccountNumberToTransactions(vector <stClientData>& vClients, stClientData& Client)
{
    string AccountNumber;

    do {
        cout << "Please Enter Account Number ? ";
        cin >> AccountNumber;
        cout << "\n";
    } while (!CheckAccountNumberForTransaction(AccountNumber, vClients, Client));
    return AccountNumber;
}

void DepositTransaction(vector <stClientData> vFileContent, double DepositAmount, double& NewBalance)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClientData Line : vFileContent)
        {
            if (Line.MarkForWithdraw == false)
            {

                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
            else
            {
                Line.AccountBalance += DepositAmount;
                NewBalance = Line.AccountBalance;
                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
        }
        MyFile.close();
    }
}

void Deposit_Choice()
{
    DepositHeader();
    stClientData Client;
    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);
    string AccountNumber = ReadClientAccountNumberToTransactions(vClients, Client);
    double NewBalance = 0, DepositAmount = 0;
    char Check = 'n';

    PrintClientCard(Client);

    cout << "Please Enter Deposit Amount ? ";
    cin >> DepositAmount;

    cout << "\nAre You sure you want perform this Transaction ? y/n ? ";
    cin >> Check;

    if (toupper(Check) == 'Y')
    {
        DepositTransaction(vClients, DepositAmount, NewBalance);

        cout << "\n\nDone Successfully\n\n";
        cout << "New Balance = " << NewBalance;
        system("pause>0");
    }
    else
    {
        cout << "\n\nThe Transaction has been cancelled.";
        Sleep(3000);
    }

}

void WithdrawalHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Withdrawal Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "\n\n";
}

void WithdrawalTransaction(vector <stClientData> vFileContent, double WithdrawalAmount, double& NewBalance)
{
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClientData Line : vFileContent)
        {
            if (Line.MarkForWithdraw == false)
            {

                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
            else
            {
                Line.AccountBalance -= WithdrawalAmount;
                NewBalance = Line.AccountBalance;
                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
        }
        MyFile.close();
    }
}

void Withdrawal_Choice()
{
    WithdrawalHeader();
    stClientData Client;
    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);
    string AccountNumber = ReadClientAccountNumberToTransactions(vClients, Client);
    double NewBalance = 0, WithdrawalAmount = 0;
    char Check = 'n';

    PrintClientCard(Client);

    cout << "Please Enter Withdrawal Amount ? ";
    cin >> WithdrawalAmount;
    while (WithdrawalAmount > Client.AccountBalance)
    {
        cout << "Amount Exceeds The Balance, You can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please Enter another amount ? ";
        cin >> WithdrawalAmount;
    }

    cout << "\nAre You sure you want perform this Transaction ? y/n ? ";
    cin >> Check;

    if (toupper(Check) == 'Y')
    {
        WithdrawalTransaction(vClients, WithdrawalAmount, NewBalance);

        cout << "\n\nDone Successfully\n\n";
        cout << "New Balance = " << NewBalance;
        system("pause>0");
    }
    else
    {
        cout << "\n\nThe Transaction has been cancelled.";
        Sleep(3000);
    }

}

void PrintTotalBalancesHeader()
{
    system("cls");
    cout << left;
    cout << "\t\t\t\tClients List \n\n";
    cout << Under_Scores(85) << "\n\n";
    cout << "| " << setw(25) << "Account Number   "
        << "| " << setw(35) << "Client Name           "
        << "| " << setw(12) << "Balance    \n\n";
    cout << Under_Scores(85) << "\n\n";
}

void PrintBalancesData(stClientData Client)
{
    cout << "| " << setw(25) << left << Client.AccountNumber;

    cout << "| " << setw(35) << left << Client.Name;

    cout << "| " << setw(12) << left << Client.AccountBalance;

    cout << endl;
}

void Total_Balances_Choice()
{
    PrintTotalBalancesHeader();
    double TotalBalances = 0;
    vector <stClientData> vClient = FillLinesFromFileToVectorForClients(ClientsFileName);
    for (stClientData& Data : vClient)
    {
        PrintBalancesData(Data);
        TotalBalances += Data.AccountBalance;
    }
    cout << Under_Scores(85) << "\n\n";
    cout << "\n\t\t\t\tTotal Balances = " << TotalBalances << endl << endl;
    BackToMainMenu();
}

enTransactionsChoice WhatisTheTransactionsChoice()
{
    enTransactionsChoice Choice = ReadTransactionsChoice();

    switch (Choice)
    {
    case enTransactionsChoice::Deposit:
        Deposit_Choice();
        return enTransactionsChoice::Deposit;
    case enTransactionsChoice::Withdraw:
        Withdrawal_Choice();
        return enTransactionsChoice::Withdraw;
    case enTransactionsChoice::TotalBalance:
        Total_Balances_Choice();
        return enTransactionsChoice::TotalBalance;
    case enTransactionsChoice::MainMenu:
        return enTransactionsChoice::MainMenu;
    }
}

void Transactions_Choice()
{
    if (!CheckAccessPermission(enPermissions::P_Transactions))
    {
        ShowAccessDeniedMessage();
        return;
    }
    enTransactionsChoice Choice;
    do {
        ShowTransactionsScreen();
        Choice = WhatisTheTransactionsChoice();
    } while (Choice != enTransactionsChoice::MainMenu);
}

void ManageUsersMainMenu()
{
    system("cls");
    cout << "========================================\n";
    cout << "\t     Manage Users Menue Screen\n";
    cout << "========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Updata User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "========================================\n";
}

enManageUsers ReadManageUsersChoice()
{
    short Number = 0;
    do
    {
        cout << "Choose What do you want to do? [1,6]?" << endl;
        cin >> Number;
    } while (Number < 1 || Number > 6);
    return (enManageUsers)Number;
}

void PrintListUsersHeader()
{
    system("cls");
    cout << left;
    cout << "\t\t\t\tUsers List \n\n";
    cout << Under_Scores(85) << "\n\n";
    cout << "| " << setw(15) << "User Name   "
        << "| " << setw(15) << "Password   "
        << "| " << setw(22) << "Permissions           \n\n";
    cout << Under_Scores(85) << "\n\n";
}

void PrintUserData(stUser User)
{
    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(15) << left << User.Password;
    cout << "| " << setw(22) << left << User.Permissions;
    cout << endl;
}

void List_Users()
{
    PrintListUsersHeader();
    vector <stUser> vUser = FillLinesFromFileToVectorForUsers(UsersFileName);
    for (stUser& user : vUser)
    {
        PrintUserData(user);
    }
    BackToManageUsersMenu();
}

void AddNewUserScreenHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Add New User Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Adding New User :\n\n";
}

bool IsUsernameExist(string un)
{
    vector <stUser> vusers = FillLinesFromFileToVectorForUsers(UsersFileName);
    for (stUser& user : vusers)
    {
        if (un == user.UserName)
        {
            return true;
        }
    }
    return false;
}

short ReadPermissions()
{
    char check = ' ';
    short Permissions = 0;
    cout << "\nDo you want to give full access? y/n? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        return -1;
    }
    cout << "\nDo you want to give access to : \n";
    cout << "\nShow Client List? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_ShowClientsList;
    }
    cout << "\nAdd New Clients? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_AddNewClient;
    }
    cout << "\nDelete Clients? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_DeleteClient;
    }
    cout << "\nUpdate Clients? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_UpdateClientInfo;
    }
    cout << "\nFind Clients? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_FindClient;
    }
    cout << "\nTransactions? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_Transactions;
    }
    cout << "\nManage Users? ";
    cin >> check;
    if (toupper(check) == 'Y')
    {
        Permissions += enPermissions::P_ManageUsers;
    }
    return Permissions;
}

stUser ReadNewUser()
{
    stUser UserData;
    cout << "Enter Username ? ";
    getline(cin >> ws, UserData.UserName);
    if (IsUsernameExist(UserData.UserName))
    {
        do
        {
            cout << "User with [" << UserData.UserName << "] already Exists, "
                << "Enter another Username? ";
            getline(cin >> ws, UserData.UserName);
        } while (IsUsernameExist(UserData.UserName));
    }
    cout << "Enter Password? ";
    cin >> UserData.Password;

    UserData.Permissions = ReadPermissions();
    return UserData;
}

string ConvertRecordToLineForUsers(stUser User, string Seperator = "#//#")
{
    string UserRecord = "";
    UserRecord += User.UserName + Seperator;
    UserRecord += User.Password + Seperator;
    UserRecord += to_string(User.Permissions);
    return UserRecord;
}

void AddUser()
{
    stUser User = ReadNewUser();
    AddDataLineToFile(ConvertRecordToLineForUsers(User), UsersFileName);
}

void Add_New_User()
{
    char Check;
    do
    {
        AddNewUserScreenHeader();
        AddUser();
        cout << "\nUser Added Successfully , Do you want add new User ? y/n ? ";
        cin >> Check;

    } while (toupper(Check) == 'Y');

}

void SaveUsersToFileWithDelete(vector <stUser> vFileContent)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser Line : vFileContent)
        {
            if (Line.eMarkForDelete == false)
            {

                MyFile << ConvertRecordToLineForUsers(Line) << endl;
            }
        }
        MyFile.close();
    }
}

void PrintUserCard(stUser UserData)
{
    cout << "The Following are the user details : \n";
    cout << Under_Scores(35) << endl;
    cout << "Username        : " << UserData.UserName << endl;
    cout << "Password        : " << UserData.Password << endl;
    cout << "Permissions     : " << UserData.Permissions << endl;
    cout << Under_Scores(35) << endl << endl;
}

bool CheckUsernameForDelete(string username, vector <stUser>& vUser, stUser& User)
{

    for (stUser& Data : vUser)
    {
        if (Data.UserName == username)
        {
            Data.eMarkForDelete = true;
            User = Data;
            return true;
        }
    }
    return false;
}

string ReadUsername()
{
    string user;
    cout << "Please Enter Username ? ";
    cin >> user;
    cout << "\n";
    return user;
}

void DeleteUserHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Delete Users Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Delete User :\n\n";
}

void Delete_User()
{
    DeleteUserHeader();

    string username = ReadUsername();

    vector <stUser> vUser = FillLinesFromFileToVectorForUsers(UsersFileName);

    stUser User;
    char Check = 'n';


    if (CheckUsernameForDelete(username, vUser, User) == false)
    {
        cout << "User With Username (" << username << ") Not Found!" << endl;
    }
    else
    {
        PrintUserCard(User);

        cout << "Are You sure you want delete this user ? y/n ? ";
        cin >> Check;

        if (toupper(Check) == 'Y')
        {
            SaveUsersToFileWithDelete(vUser);

            cout << "\n\nUser Deleted Successfully\n";
        }
    }
    BackToManageUsersMenu();
}

stUser FillUpdateUserData(string username)
{
    stUser UserData;

    UserData.UserName = username;
    cout << "Enter Password? ";
    cin >> UserData.Password;

    UserData.Permissions = ReadPermissions();
    return UserData;
}

void SaveUsersToFileWithUpdate(vector <stUser> vFileContent, stUser UpdatedData)
{
    fstream MyFile;
    MyFile.open(UsersFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stUser Line : vFileContent)
        {
            if (Line.UserName == UpdatedData.UserName)
            {
                MyFile << ConvertRecordToLineForUsers(UpdatedData) << endl;
            }
            else
            {
                MyFile << ConvertRecordToLineForUsers(Line) << endl;
            }
        }
        MyFile.close();
    }
}

bool CheckUsernameForUpdate(string username, vector <stUser> vUser, stUser& User)
{

    for (stUser& Data : vUser)
    {
        if (Data.UserName == username)
        {
            User = Data;
            return true;
        }
    }
    return false;
}

void UpdateUserInfoHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Update User Info Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Update User Info :\n\n";
}

void Update_User()
{
    UpdateUserInfoHeader();

    string Username = ReadUsername();
    stUser UpdatedData;

    vector <stUser> vUser = FillLinesFromFileToVectorForUsers(UsersFileName);

    stUser User;
    char Check = 'n';


    if (CheckUsernameForUpdate(Username, vUser, User) == false)
    {
        cout << "User With Username (" << Username << ") Not Found!" << endl;
    }
    else
    {
        PrintUserCard(User);

        cout << "Are You sure you want Update this user ? y/n ? ";
        cin >> Check;
        if (toupper(Check) == 'Y')
        {
            UpdatedData = FillUpdateUserData(Username);
            SaveUsersToFileWithUpdate(vUser, UpdatedData);

            cout << "\n\nUser Updated Successfully\n";
        }
        else
        {
            cout << "\n\nThe Transaction has been cancelled.";
            Sleep(3000);
        }
    }
    BackToManageUsersMenu();
}

void FindUserHeader()
{
    system("cls");
    cout << Under_Scores(35) << endl;
    cout << "       Find User Screen\n";
    cout << Under_Scores(35) << endl;
    cout << "Find User :\n\n";
}

void Find_User()
{

    FindUserHeader();

    string Username = ReadUsername();

    vector <stUser> vUser = FillLinesFromFileToVectorForUsers(UsersFileName);

    stUser User;
    char Check = 'n';


    if (CheckUsernameForUpdate(Username, vUser, User) == false)
    {
        cout << "User With Username (" << Username << ") Not Found!" << endl;
    }
    else
    {
        PrintUserCard(User);
    }
    BackToManageUsersMenu();
}

void ManageUsersChoice(enManageUsers& Choice)
{
    Choice = ReadManageUsersChoice();

    switch (Choice)
    {
    case enManageUsers::eListUsers:
        List_Users();
        break;
    case enManageUsers::eAddNewUser:
        Add_New_User();
        break;
    case enManageUsers::eDeleteUser:
        Delete_User();
        break;
    case enManageUsers::eUpdateUser:
        Update_User();
        break;
    case enManageUsers::eFindUser:
        Find_User();
        break;
    default:
        break;
    }
}

void Manage_Users()
{
    if (!CheckAccessPermission(enPermissions::P_ManageUsers))
    {
        ShowAccessDeniedMessage();
        return;
    }
    enManageUsers Choice;
    do
    {
        ManageUsersMainMenu();
        ManageUsersChoice(Choice);

    } while (Choice != enManageUsers::eMainMenu);
}

void GoToChoice(enChoiceResulte Choice)
{
    switch (Choice)
    {
    case enChoiceResulte::ShowClientsList:
        Show_Clients_List();
        break;
    case enChoiceResulte::AddNewClient:
        Add_New_Clients();
        break;
    case enChoiceResulte::DeleteClient:
        Delete_Client();
        break;
    case enChoiceResulte::UpdateClientInfo:
        Update_Client_Info();
        break;
    case enChoiceResulte::FindClient:
        Find_Client();
        break;
    case enChoiceResulte::Transactions:
        Transactions_Choice();
        break;
    case enChoiceResulte::ManageUsers:
        Manage_Users();
        break;

    }
}

short ReadChoice()
{
    short Number = 0;
    do
    {
        cout << "Choose What do you want to do? [1,8]?" << endl;
        cin >> Number;
    } while (Number < 1 || Number > 8);
    return Number;
}

bool Choice()
{
    enChoiceResulte choice = (enChoiceResulte)ReadChoice();
    GoToChoice(choice);
    if (choice == enChoiceResulte::Logout)
        return true;
    else
        return false;
}

void ShowMainMenue()
{
    system("cls");
    cout << "========================================\n";
    cout << "\t   Main Menue Screen\n";
    cout << "========================================\n";
    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add New Clients.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "========================================\n";
}

stUser ConvertLineToRecordForUsers(string str, string Seperator)
{
    stUser User;
    vector <string> vstr;
    vstr = SplitString(str, Seperator);
    User.UserName = vstr[0];
    User.Password = vstr[1];
    User.Permissions = stoi(vstr[2]);
    return User;
}

vector <stUser> FillLinesFromFileToVectorForUsers(string FileName)
{
    fstream MyFile;
    vector <stUser> vUser;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vUser.push_back(ConvertLineToRecordForUsers(Line));
        }
        MyFile.close();
    }
    return vUser;
}

void LoginScreenHeader()
{
    system("cls");
    cout << Under_Scores(36) << endl;
    cout << "            Login Screen\n";
    cout << Under_Scores(36) << endl << endl;
}

stUser ReadLoginData()
{
    stUser LoginData;
    cout << "Enter UserName? ";
    cin >> LoginData.UserName;
    cout << "Enter Password? ";
    cin >> LoginData.Password;
    return LoginData;
}

bool IsLoginDataExist(stUser LoginData)
{
    vector <stUser> vusers = FillLinesFromFileToVectorForUsers(UsersFileName);
    for (stUser& user : vusers)
    {
        if (LoginData.UserName == user.UserName && LoginData.Password == user.Password)
        {
            CurrentUser = user;
            return true;
        }
    }
    return false;
}

void Login()
{
    stUser LoginData;
    bool b = false;
    do
    {
        system("cls");
        LoginScreenHeader();
        if (b)
            cout << "Invalid Username/password!\n";
        LoginData = ReadLoginData();
        b = true;
    } while (!IsLoginDataExist(LoginData));

}

void StartProgram()
{
    bool Logout = true;
    do
    {
        system("cls");
        if (Logout)
            Login();
        ShowMainMenue();
        Logout = Choice();
    } while (1);
}

int main()
{

    StartProgram();


    cout << "\n\n\n";
    system("pause>0");
    return 0;
}

