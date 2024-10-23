#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <windows.h>
#include <regex>
#include <conio.h>

using namespace std;

const int MAX_PIN_LENGTH = 6;
const int MIN_PIN_LENGTH = 4;
const int MIN_BALANCE = 5000;

struct Account {   //Details na kailangan i-fill ng users
    string accountNumber;
    string accountName;
    string birthday;
    string contactNumber;
    string pinCode;
    double balance;
};

class ATMList {
private: //List of Accounts = confidential.
    vector<Account> accounts;

public: //Methods na ginagamit para mapanatiling organized ang program. Can be accessed on the whole program.
    void insertAccount(const Account &newAccount) { //will insert new accounts.
        accounts.push_back(newAccount);
    }

    Account* findAccount(const string &accountNumber) { //will find the Account in the accounts list based on the AccNo.
        for (auto &account : accounts) {
            if (account.accountNumber == accountNumber) {
                return &account;
            }
        }
        return NULL;
    }

    void updateAccountInList(const Account &updatedAccount) { //nag-uupdate ng account sa accounts list.
        for (auto &account : accounts) {
            if (account.accountNumber == updatedAccount.accountNumber) {
                account = updatedAccount;
                break;
            }
        }
    }

    const vector<Account>& getAccounts() const { //used to get the accounts list.
        return accounts;
    }

    bool accountNumberExists(const string &accountNumber) const { //to ensure na hindi madu-duplicate ang AccNo.
        for (const auto &account : accounts) {
            if (account.accountNumber == accountNumber) {
                return true;
            }
        }
        return false;
    }
};

ATMList atmList;
HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE); //HANDLE ay ginagamit para sa pag-manage ng console output.

void gotoxy(int x, int y) { //ito ang nagse-set ng position ng cursor sa console coordinates (x, y).
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); //pwede natin ilipat ang cursor sa kahit anong posisyon sa console kapag ginamit ito.
}

void setColor(int color) { //nagse-set ng kulay ng text.
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void showLoadingBar1(int duration) { //loading bar display
    int bar1 = 176, bar2 = 178;

    SetConsoleTextAttribute(console_color, 7);
    gotoxy(65, 26); cout << "            Loading ATM...";

    SetConsoleTextAttribute(console_color, 8);
    cout << "\n\n\t\t\t\t\t\t\t";

    for (int i = 0; i < 50; i++) { //ginagamit ang loop na ito para gumalaw ang loading bar display.
        cout << (char)bar2;
        Sleep(duration);
    }
    cout << endl;
}

void showLoadingBar2(int duration) { //loading bar display
    int bar1 = 176, bar2 = 178;

    SetConsoleTextAttribute(console_color, 7);
    gotoxy(70, 20); cout << "Processing your request...";

    SetConsoleTextAttribute(console_color, 8);
    cout << "\n\n\t\t\t\t\t\t\t";

    for (int i = 0; i < 50; i++) { //ginagamit ang loop na ito para gumalaw ang loading bar display.
        cout << (char)bar2;
        Sleep(duration);
    }
    cout << endl;
}

void ATM_Title() { //welcome screen - logo/title
    system("cls");
    SetConsoleTextAttribute(console_color, 9);
    gotoxy(15, 8); cout << "        .o.       ooooooooooooo ooo        ooooo       .oooooo..o oooooo   oooo  .oooooo..o ooooooooooooo oooooooooooo ooo        ooooo " << endl;
    gotoxy(15, 9); cout << "       .888.      8'   888   `8 `88.       .888'      d8P'    `Y8  `888.   .8'  d8P'    `Y8 8'   888   `8 `888'     `8 `88.       .888' " << endl;
    gotoxy(15, 10); cout <<"      .8'888.          888       888b     d'888       Y88bo.        `888. .8'   Y88bo.           888       888          888b     d'888  " << endl;
    gotoxy(15, 11); cout <<"     .8' `888.         888       8 Y88. .P  888        `'Y8888o.     `888.8'     `'Y8888o.       888       888oooo8     8 Y88. .P  888  " << endl;
    gotoxy(15, 12); cout <<"    .88ooo8888.        888       8  `888'   888            `'Y88b     `888'          `'Y88b      888       888    '     8  `888'   888  " << endl;
    gotoxy(15, 13); cout <<"   .8'     `888.       888       8    Y     888       oo     .d8P      888      oo     .d8P      888       888       o  8    Y     888  " << endl;
    gotoxy(15, 14); cout <<"  o88o     o8888o     o888o     o8o        o888o      8''88888P'      o888o     8''88888P'      o888o     o888ooooood8 o8o        o888o " << endl;
    SetConsoleTextAttribute(console_color, 8);
}

string findRemovableDrive() { //
    DWORD drives = GetLogicalDrives(); //kumukuha ng bitmask ng lahat ng available drives sa system.
    char driveLetter = 'A';

    for (int i = 0; i < 26; i++) { //will check ang A hanggang Z files - 26 letters.
        if (drives & (1 << i)) {
            string drive = string(1, driveLetter) + ":\\"; //gumagawa ng string

            if (GetDriveType(drive.c_str()) == DRIVE_REMOVABLE) { //will check kung removable ba ang drive
                return drive;
            }
        }
        driveLetter++;
    }
    return "";
}

bool isCardInserted() {
    string removableDrive = findRemovableDrive(); //will find the removable drive.

    if (removableDrive.empty()) {
        system("cls"); //clear screen
        SetConsoleTextAttribute(console_color, 4); //display if di naka-insert ang flashdrive.
        gotoxy(58, 11); cout << "========================================================" << endl;
        gotoxy(79, 12); cout << "Warning";
        gotoxy(58, 13); cout << "========================================================" << endl;
        gotoxy(59, 15); cout << "Please insert your ATM card (flash drive) to proceed.";
        gotoxy(58, 17); cout << "========================================================" << endl;
        gotoxy(59, 19); cout << "Press any key to return to the main menu...";
        cin.ignore();
        cin.get();
        return false; //walang na-detect na flashdrive.
    }

    string cardFilePath = removableDrive + "ATM_CARD.txt"; //will create a PATH to the text file name, inside the flashdrive.

    ifstream cardFile(cardFilePath); //read the file.

    if (!cardFile.is_open()) { //if there is no file, it will create new file.
        ofstream createFile(cardFilePath);

        if (createFile.is_open()) {
            cout << "ATM_CARD.txt not found. A new file was created at: " << cardFilePath << endl;
            createFile.close();
        } else {
            cout << "Failed to access or create ATM_CARD.txt on the flash drive." << endl;
            return false;
        }
    }
    cardFile.close();
    return true;
}

bool readDataFromCard() { //halos same explanation sa isCardInserted, pero ito ay taga-basa lang.
    if (!isCardInserted()) return false;

    string removableDrive = findRemovableDrive();
    string cardFilePath = removableDrive + "ATM_CARD.txt";

    ifstream cardFile(cardFilePath);

    if (!cardFile.is_open()) {
        cout << "\nError: Could not access ATM card file on the flash drive." << endl;
        return false;
    }

    string line;

    while (getline(cardFile, line)) {
        stringstream ss(line);
        Account account;
        getline(ss, account.accountName, ',');
        getline(ss, account.accountNumber, ',');
        getline(ss, account.pinCode, ',');
        ss >> account.balance;

        atmList.insertAccount(account);
    }
    cardFile.close();
    return true;
}

string accountNumber_generator() { //will create unique account numbers
    const int ACCOUNT_NUMBER_LENGTH = 5;
    srand(static_cast<unsigned int>(time(0))); //used para gawing "random" ang bawat number na i-ggenerate.
    string accountNumber;

    do { //to ensure that the AccNo. are really unique
        accountNumber = to_string(rand() % 100000);
        accountNumber.insert(0, ACCOUNT_NUMBER_LENGTH - accountNumber.length(), '0');
    } while (atmList.accountNumberExists(accountNumber));

    return accountNumber;
}

bool isValidBirthday(const string& birthday) {
    const regex pattern(R"(\d{2}/\d{2}/\d{4})");
    return regex_match(birthday, pattern); //will check if the pattern is correct
}

string encryptPIN(const string &pin) {
    string encrypted = pin;
    char key = 'X';

    for (char &ch : encrypted) {
        ch ^= key; //XOR operation
    }
    return encrypted;
}

void writeDataToCard(const Account &newAccount, bool appendMode = true) {
    string removableDrive = findRemovableDrive();

    if (removableDrive.empty()) {
        cout << "\nError: No flash drive detected to save account data." << endl;
        return;
    }

    string cardFilePath = removableDrive + "ATM_CARD.txt";
    ofstream cardFile(cardFilePath, appendMode ? ios::app : ios::trunc);

    if (!cardFile.is_open()) {
        cout << "\nError: Could not access ATM card file on the flash drive." << endl;
        return;
    }

    cardFile << newAccount.accountName << ","
             << newAccount.accountNumber << ","
             << newAccount.pinCode << ","
             << newAccount.balance << endl;
    cardFile.close();
}

void rewriteAllAccountsToCard() {
    string removableDrive = findRemovableDrive();

    if (removableDrive.empty()) {
        cout << "\nError: No flash drive detected to save account data." << endl;
        return;
    }

    string cardFilePath = removableDrive + "ATM_CARD.txt";
    ofstream cardFile(cardFilePath, ios::trunc);

    if (!cardFile.is_open()) {
        cout << "\nError: Could not access ATM card file on the flash drive." << endl;
        return;
    }

    for (const auto &account : atmList.getAccounts()) {
        cardFile << account.accountName << ","
                 << account.accountNumber << ","
                 << account.pinCode << ","
                 << account.balance << endl;
    }
    cardFile.close();
}

// Registration Module
void registerAccount() { //Tatawagin na ang mga functions.
    system("cls");

    if (!isCardInserted()) return;

    Account newAccount;

    system("cls");
    SetConsoleTextAttribute(console_color, 7);
    gotoxy(58, 11); cout << "========================================================" << endl;
    gotoxy(75, 12); cout << "Register a New Account";
    gotoxy(58, 13); cout << "========================================================" << endl;
    gotoxy(58, 15); cout << "Enter the following details to create";
    gotoxy(58, 16); cout << "a new account:";
    gotoxy(58, 18); cout << "--------------------------------------------------------" << endl;

    setColor(10);
    newAccount.accountNumber = accountNumber_generator();
    gotoxy(58, 20); cout << "> Generated Account Number: " << newAccount.accountNumber << endl;
    gotoxy(58, 22); cout << "> Account Name: ";
    cin.ignore();
    getline(cin, newAccount.accountName);
    gotoxy(58, 24); cout << "> Birthday (DD/MM/YYYY): ";
    cin >> newAccount.birthday;

    while (!isValidBirthday(newAccount.birthday)) {
        gotoxy(58, 24); cout << "> Invalid date format! Please enter in DD/MM/YYYY format: ";
        cin >> newAccount.birthday;
    }

    gotoxy(58, 26); cout << "> Contact Number: ";
    cin >> newAccount.contactNumber;

    gotoxy(58, 28); cout << "> Initial Deposit (Minimum 5000): PHP ";
    cin >> newAccount.balance;

    while (newAccount.balance < MIN_BALANCE) {
        gotoxy(59, 28); cout << " Initial deposit must be at least PHP 5000. ";
        gotoxy(59, 29); cout << " Enter a valid amount: PHP ";
        cin >> newAccount.balance;
    }

    string pin;
    char ch;
    int count = 0;

    do {
        gotoxy(58, 31); cout << "--------------------------------------------------------" << endl;
        setColor(4);
        gotoxy(58, 32); cout << "Enter a 4 to 6-digit PIN: ";

        pin.clear();
        count = 0;

        while (count < MAX_PIN_LENGTH) {
            ch = _getch();

            if (ch == 13) {
                if (count >= MIN_PIN_LENGTH) {
                    break;
                } else {
                    cout << "\nPlease enter a valid 4-digit PIN." << endl;

                    pin.clear();
                    count = 0;

                    gotoxy(58, 32); cout << "Enter a 4 to 6-digit PIN: ";
                    continue;
                }
            }

            if (ch == 8) {
                if (count > 0) {
                    cout << "\b \b";
                    pin.pop_back();
                    count--;
                }
                continue;
            }

            if (isdigit(ch)) {
                pin += ch;

                cout << '*'; // will show asterisk kapag tinype ang pincode
                count++;

                if (count == MAX_PIN_LENGTH) {
                    break;
                }
            }
        }
    } while (pin.length() < MIN_PIN_LENGTH || pin.length() > MAX_PIN_LENGTH);

    newAccount.pinCode = encryptPIN(pin);
    writeDataToCard(newAccount);
    atmList.insertAccount(newAccount);

    system("cls");
    showLoadingBar2(15);
    system("cls");

    SetConsoleTextAttribute(console_color, 7);
    gotoxy(58, 11); cout << "========================================================" << endl;
    gotoxy(71, 12); cout << "Account successfully registered!";
    gotoxy(58, 13); cout << "--------------------------------------------------------" << endl;
    gotoxy(58, 15); cout << "PIN: " << pin << endl;
    gotoxy(58, 16); cout << "Account Number: " << newAccount.accountNumber << endl;
    gotoxy(58, 17); cout << "Account Name: " << newAccount.accountName << endl;
    gotoxy(58, 18); cout << "Initial Balance: PHP " << fixed << setprecision(2) << newAccount.balance << endl;
    gotoxy(58, 20); cout << "========================================================" << endl;
    gotoxy(58, 21); cout << "Press any key to return to the main menu...";
    cin.ignore();
    cin.get();
}

// Transaction Module
void transactionModule() { //will handle every transactions of the program.
    system("cls");
    SetConsoleTextAttribute(console_color, 7);

    if (!isCardInserted()) return;

    string accountNumber;
    string pin;

    gotoxy(58, 11); cout << "========================================================" << endl;
    gotoxy(78, 12); cout << "Transaction Menu";
    gotoxy(58, 13); cout << "========================================================" << endl;
    gotoxy(59, 15); cout << "> Enter Account Number: ";
    cin >> accountNumber;

    Account* account = atmList.findAccount(accountNumber);

    if (account == NULL) {
        system("cls");
        SetConsoleTextAttribute(console_color, 4);
        gotoxy(58, 11); cout << "========================================================" << endl;
        gotoxy(79, 12); cout << "Error";
        gotoxy(58, 13); cout << "========================================================" << endl;
        gotoxy(59, 15); cout << "Account not found! Please register first.";
        gotoxy(58, 17); cout << "========================================================" << endl;
        gotoxy(59, 19); cout << "Press any key to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    gotoxy(59, 16); cout << "> Enter your PIN: ";
    cin >> pin;

    if (encryptPIN(pin) != account->pinCode) {
        system("cls");
        SetConsoleTextAttribute(console_color, 4);
        gotoxy(58, 11); cout << "========================================================" << endl;
        gotoxy(79, 12); cout << "Error";
        gotoxy(58, 13); cout << "========================================================" << endl;
        gotoxy(59, 15); cout << "Incorrect PIN! Please try again.";
        gotoxy(58, 17); cout << "========================================================" << endl;
        gotoxy(59, 19); cout << "Press any key to return to the main menu...";
        cin.ignore();
        cin.get();
        return;
    }

    int choice;

    do {
        system("cls");
        setColor(7);
        gotoxy(65, 11); cout << "==========================================" << endl;
        gotoxy(77, 12); cout << "Transaction Menu";
        gotoxy(65, 13); cout << "==========================================" << endl;

        setColor(10);
        gotoxy(65, 15); cout << "Choose a transaction option:";
        gotoxy(77, 17); cout << "[1] Balance Inquiry";
        gotoxy(77, 18); cout << "[2] Withdraw";
        gotoxy(77, 19); cout << "[3] Deposit";
        gotoxy(77, 20); cout << "[4] Fund Transfer";
        gotoxy(77, 21); cout << "[5] Change PIN";
        gotoxy(77, 22); cout << "[6] Exit";

        setColor(7);
        gotoxy(65, 24); cout << "-----------------------------------------" << endl;
        gotoxy(65, 25); cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: //Balance Inquiry - ang kasalukuyang laman na pera ng account.
            system("cls");
            SetConsoleTextAttribute(console_color, 7);
            gotoxy(58, 11); cout << "========================================================" << endl;
            gotoxy(79, 12); cout << "Balance Inquiry";
            gotoxy(58, 13); cout << "========================================================" << endl;
            gotoxy(58, 15); cout << "Your Current Balance: PHP " << fixed << setprecision(2) << account->balance << endl;
            gotoxy(58, 17); cout << "--------------------------------------------------------" << endl;
            break;

        case 2: //Withdraw - paglabas/pagkuha ng pera sa account.
            {
                double amount;

                system("cls");
                SetConsoleTextAttribute(console_color, 7);
                gotoxy(58, 11); cout << "========================================================" << endl;
                gotoxy(79, 12); cout << "Withdraw Funds";
                gotoxy(58, 13); cout << "========================================================" << endl;
                gotoxy(58, 15); cout << "Enter amount to withdraw: PHP ";
                cin >> amount;

                if (amount > account->balance) {
                    system("cls");
                    SetConsoleTextAttribute(console_color, 4);
                    gotoxy(58, 11); cout << "========================================================" << endl;
                    gotoxy(79, 12); cout << "Error";
                    gotoxy(58, 13); cout << "========================================================" << endl;
                    gotoxy(59, 15); cout << "Insufficient balance for this transaction.";
                    gotoxy(59, 16); cout << "Please check your balance and try again.";
                    gotoxy(58, 18); cout << "========================================================" << endl;
                    gotoxy(59, 19); cout << "Press any key to return to the main menu...";
                    cin.ignore();
                    cin.get();
                } else {
                    system("cls");
                    showLoadingBar2(15);
                    account->balance -= amount;

                    system("cls");
                    SetConsoleTextAttribute(console_color, 7);
                    gotoxy(58, 11); cout << "========================================================" << endl;
                    gotoxy(75, 12); cout << "Withdrawal successful!";
                    gotoxy(58, 13); cout << "========================================================" << endl;
                }
                break;
            }

        case 3: //Deposit - paglalagay ng pera sa account.
            {
                double amount;

                system("cls");
                SetConsoleTextAttribute(console_color, 7);
                gotoxy(58, 11); cout << "========================================================" << endl;
                gotoxy(79, 12); cout << "Deposit Funds";
                gotoxy(58, 13); cout << "========================================================" << endl;
                gotoxy(58, 15); cout << "Enter the amount to deposit: PHP ";
                cin >> amount;

                system("cls");
                showLoadingBar2(15);
                account->balance += amount;

                system("cls");
                SetConsoleTextAttribute(console_color, 7);
                gotoxy(58, 11); cout << "========================================================" << endl;
                gotoxy(75, 12); cout << "Deposit successful!";
                gotoxy(58, 13); cout << "========================================================" << endl;
                break;
            }

        case 4: //Fund Transfer - pagtransfer ng pera sa ibang account user.
            {
                string targetAccountNumber;
                double transferAmount;

                system("cls");
                SetConsoleTextAttribute(console_color, 7);
                gotoxy(58, 11); cout << "========================================================" << endl;
                gotoxy(79, 12); cout << "Fund Transfer";
                gotoxy(58, 13); cout << "========================================================" << endl;
                gotoxy(58, 15); cout << "> Enter target account number: ";
                cin >> targetAccountNumber;

                Account* targetAccount = atmList.findAccount(targetAccountNumber);

                if (targetAccount == NULL) {
                    system("cls");
                    SetConsoleTextAttribute(console_color, 4);
                    gotoxy(58, 11); cout << "========================================================" << endl;
                    gotoxy(81, 12); cout << "Error";
                    gotoxy(58, 13); cout << "========================================================" << endl;
                    gotoxy(59, 15); cout << "Target account not found.";
                    gotoxy(58, 17); cout << "========================================================" << endl;
                } else {
                    gotoxy(58, 16); cout << "> Enter amount to transfer: PHP ";
                    cin >> transferAmount;

                    if (transferAmount > account->balance) {
                        system("cls");
                        SetConsoleTextAttribute(console_color, 4);
                        gotoxy(58, 11); cout << "========================================================" << endl;
                        gotoxy(79, 12); cout << "Error";
                        gotoxy(58, 13); cout << "========================================================" << endl;
                        gotoxy(59, 15); cout << "Insufficient balance!";
                        gotoxy(58, 17); cout << "========================================================" << endl;
                    } else {
                        system("cls");
                        showLoadingBar2(15);
                        account->balance -= transferAmount;
                        targetAccount->balance += transferAmount;

                        system("cls");
                        SetConsoleTextAttribute(console_color, 7);
                        gotoxy(58, 11); cout << "========================================================" << endl;
                        gotoxy(75, 12); cout << "Transfer successful!";
                        gotoxy(58, 13); cout << "========================================================" << endl;
                    }
                }
                break;
            }

        case 5: //Change PIN
            {
                string newPin;

                do {
                    system("cls");
                    SetConsoleTextAttribute(console_color, 7);
                    gotoxy(58, 11); cout << "========================================================" << endl;
                    gotoxy(79, 12); cout << "Change Pin";
                    gotoxy(58, 13); cout << "========================================================" << endl;
                    gotoxy(59, 15); cout << "Enter a new 4 to 6-digit PIN: ";
                    cin >> newPin;

                    system("cls");
                    showLoadingBar2(15);
                } while (newPin.length() < MIN_PIN_LENGTH || newPin.length() > MAX_PIN_LENGTH);

                account->pinCode = encryptPIN(newPin);

                system("cls");
                SetConsoleTextAttribute(console_color, 7);
                gotoxy(58, 11); cout << "========================================================" << endl;
                gotoxy(73, 12); cout << "PIN successfully changed!";
                gotoxy(58, 13); cout << "========================================================" << endl;
                gotoxy(59, 15); cout << "Press any key to return to the main menu...";
                cin.ignore();
                cin.get();
                break;
            }

        case 6: //Exit
            system("cls");
            SetConsoleTextAttribute(console_color, 7);
            gotoxy(58, 11); cout << "========================================================" << endl;
            gotoxy(76, 12); cout << "Exiting transaction module...";
            gotoxy(58, 13); cout << "========================================================" << endl;
            break;

        default:
            gotoxy(66, 26); cout << "Invalid choice! Please try again.";
            break;
        }

        rewriteAllAccountsToCard();

        gotoxy(65, 25); cout << "Press any key to continue...";
        cin.ignore();
        cin.get();
    } while (choice != 6);
}

// Main Menu
void mainMenu() {
    int choice;

    do {
        system("cls");
        setColor(7);
        gotoxy(65, 11); cout << "==========================================" << endl;
        gotoxy(77, 12); cout << "A T M  S Y S T E M";
        gotoxy(65, 13); cout << "==========================================" << endl;

        setColor(10);
        gotoxy(65, 15); cout << "Welcome to the ATM! Please select";
        gotoxy(65, 16); cout << "from the following options: ";
        gotoxy(77, 18); cout << "[1] Register a New Account";
        gotoxy(77, 19); cout << "[2] Transaction Module";
        gotoxy(77, 20); cout << "[3] Exit";

        setColor(7);
        gotoxy(65, 22); cout << "-----------------------------------------" << endl;
        gotoxy(65, 23); cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerAccount(); //para mag-register ng bagong account.
            break;
        case 2:
            transactionModule(); //para pumasok sa transaction section.
            break;
        case 3:
            system("cls");
            SetConsoleTextAttribute(console_color, 7);
            gotoxy(58, 11); cout << "========================================================" << endl;
            gotoxy(79, 12); cout << "Thank You !";
            gotoxy(58, 13); cout << "========================================================" << endl;
            gotoxy(59, 15); cout << "Thank you for using the ATM system.";
            gotoxy(59, 16); cout << "Have a great day!";
            gotoxy(58, 18); cout << "========================================================" << endl;
            break;

        default: //invalid choice
            gotoxy(66, 26); cout << "Invalid choice! Please try again.";
            break;
        }
    } while (choice != 3);
}

int main() {
    ATM_Title();
    showLoadingBar1(15);
    readDataFromCard();
    mainMenu();
    return 0;
}
