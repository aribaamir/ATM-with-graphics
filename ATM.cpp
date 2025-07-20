#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;

string currentusername = "";
const int MAIN_SCREEN = 0;
const int ATM_SCREEN = 1;
int currentScreen = MAIN_SCREEN;

class ATMInterface {
protected:
    int SKY_BLUE = COLOR(0, 204, 255);
    
public:
    void drawInputBox(int x, int y, int w, int h) {
        setcolor(WHITE);
        rectangle(x, y, x + w, y + h);
        setfillstyle(SOLID_FILL, WHITE);
        floodfill(x + 1, y + 1, WHITE);
    }

    void getInput(int x, int y, char* buffer, int maxLen, bool mask) {
        int len = 0;
        buffer[0] = '\0';
        int boxWidth = 300, boxHeight = 25;

        drawInputBox(x, y, boxWidth, boxHeight);

        while (true) {
            char ch = getch();

            if (ch == 13 && len > 0) break;
            else if (ch == 8 && len > 0) {
                len--;
                buffer[len] = '\0';
            }
            else if (ch >= 32 && ch <= 126 && len < maxLen) {
                buffer[len++] = ch;
                buffer[len] = '\0';
            }

            setfillstyle(SOLID_FILL, WHITE);
            bar(x + 2, y + 2, x + boxWidth - 2, y + boxHeight - 2);

            char display[100];
            if (mask) {
                for (int i = 0; i < len; i++) display[i] = '*';
                display[len] = '\0';
            } else {
                strcpy(display, buffer);
            }

            setbkcolor(WHITE);
            setcolor(BLACK);
            outtextxy(x + 5, y + 5, display);
        }
    }

    bool isInside(int x, int y, int bx, int by, int bw, int bh) {
        return x >= bx && x <= bx + bw && y >= by && y <= by + bh;
    }

    void drawButton(int x, int y, int w, int h, const char* label) {
        setcolor(SKY_BLUE);
        rectangle(x, y, x + w, y + h);
        setfillstyle(SOLID_FILL, SKY_BLUE);
        floodfill(x + 1, y + 1, SKY_BLUE);
        setbkcolor(SKY_BLUE);
        setcolor(WHITE);
        settextstyle(10, 0, 3);
        int textWidth = textwidth((char*)label);
        int textHeight = textheight((char*)label);
        int textX = x + (w - textWidth) / 2;
        int textY = y + (h - textHeight) / 2;
        outtextxy(textX, textY, (char*)label);
    }

    void drawwbutton(int x, int y, int w, int h, const char* label) {
        setcolor(WHITE);
        rectangle(x, y, x + w, y + h);
        setfillstyle(SOLID_FILL, WHITE);
        floodfill(x + 1, y + 1, WHITE);
        setbkcolor(WHITE);
        setcolor(BLACK);
        settextstyle(3, 0, 2);
        int textWidth = textwidth((char*)label);
        int textHeight = textheight((char*)label);
        int textX = x + (w - textWidth) / 2;
        int textY = y + (h - textHeight) / 2;
        outtextxy(textX, textY, (char*)label);
    }
};

class Account {
private:
    int currentBalance;
    char storedPin[5];
    char accountName[50];
    char accountNumber[20];
    bool accountCreated;

public:
    Account() {
        accountName[0] = '\0';
        accountNumber[0] = '\0';
        storedPin[0] = '\0';
        currentBalance = 0;
        accountCreated = false;
    }

    int getBalance() const { return currentBalance; }
    void setBalance(int balance) { currentBalance = balance; }
    
    const char* getPin() const { return storedPin; }
    void setPin(const char* pin) { strncpy(storedPin, pin, 4); storedPin[4] = '\0'; }
    
    const char* getName() const { return accountName; }
    void setName(const char* name) { strncpy(accountName, name, 49); accountName[49] = '\0'; }
    
    const char* getAccountNumber() const { return accountNumber; }
    void setAccountNumber(const char* num) { strncpy(accountNumber, num, 19); accountNumber[19] = '\0'; }
    
    bool isCreated() const { return accountCreated; }
    void setCreated(bool created) { accountCreated = created; }
    
    void generateAccountNumber() {
        
        static int accountCounter = 1000;
        sprintf(accountNumber, "ACCT%08d", accountCounter++);
    }
};

class BankScreen : public ATMInterface {
public:
    int show() {
        initwindow(700, 550);
        setbkcolor(BLUE);
        cleardevice();
        drawButton(200, 200, 300, 50, "Create Account");
        drawButton(200, 280, 300, 50, "Use ATM");
        
        while (true) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);
                if (isInside(mx, my, 200, 200, 300, 50)) return 2;
                if (isInside(mx, my, 200, 280, 300, 50)) return 1;
            }
        }
    }
};

class ATMScreen : public ATMInterface {
private:
    Account& account;

public:
    ATMScreen(Account& acc) : account(acc) {}

    int show() {
        initwindow(700, 550);
        setbkcolor(BLUE);
        cleardevice();
        setcolor(WHITE);
        settextstyle(10, 0, 3);
        outtextxy(30, 30, "ATM");

        setcolor(SKY_BLUE);
        settextstyle(10, 0, 2);
        outtextxy(30, 120, "Welcome");

        setcolor(WHITE);
        settextstyle(6, 0, 1);
        outtextxy(30, 150, (char*)account.getName());

        setcolor(SKY_BLUE);
        settextstyle(10, 0, 1);
        outtextxy(30, 200, "Account no: ");
        setcolor(WHITE);
        settextstyle(6, 0, 1);
        outtextxy(30, 230, (char*)account.getAccountNumber());

        setcolor(SKY_BLUE);
        settextstyle(10, 0, 2);
        outtextxy(30, 270, "Current Balance:");
        setcolor(WHITE);
        settextstyle(6, 0, 2);
        char balanceStr[20];
        sprintf(balanceStr, "%d/-", account.getBalance());
        outtextxy(30, 310, balanceStr);

        drawButton(250, 140, 230, 60, "Withdraw");
        drawButton(250, 220, 230, 60, "Deposit");
        drawButton(250, 300, 230, 60, "Exit");

        while (true) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);
                if (isInside(mx, my, 250, 140, 230, 60)) return 3;
                if (isInside(mx, my, 250, 220, 230, 60)) return 4;
                if (isInside(mx, my, 250, 300, 230, 60)) return 5;
            }
        }
    }
};

class WithdrawScreen : public ATMInterface {
private:
    Account& account;

public:
    WithdrawScreen(Account& acc) : account(acc) {}

    void show() {
        char input[20] = "";
        int amount = 0;
        char balanceStr[20];

        setbkcolor(BLUE);
        cleardevice();

        setcolor(WHITE);
        settextstyle(10, 0, 3);
        outtextxy(30, 30, "Withdraw");

        setcolor(SKY_BLUE);
        settextstyle(10, 0, 3);
        outtextxy(30, 120, "Balance");

        sprintf(balanceStr, "%d/-", account.getBalance());
        setcolor(WHITE);
        settextstyle(6, 0, 4);
        outtextxy(30, 150, balanceStr);

        drawwbutton(250, 120, 330, 60, "Enter amount");
        drawButton(250, 230, 150, 50, "Rs.500");
        drawButton(430, 230, 150, 50, "Rs.1000");
        drawButton(250, 310, 150, 50, "Rs.2000");
        drawButton(430, 310, 150, 50, "Rs.3000");
        drawButton(400, 430, 100, 50, "Exit");
        drawButton(520, 430, 100, 50, "Enter");
        drawButton(120, 450, 100, 30, "Delete");

        int inputX = 250, inputY = 120, inputW = 330, inputH = 60;

        // Keypad
        int keyX = 50, keyY = 290, keyW = 50, keyH = 40, spacing = 10;
        const char* keys[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        int keypad[10][4], k = 0;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                if (k >= 10) break;
                int x = keyX + col * (keyW + spacing);
                int y = keyY + row * (keyH + spacing);
                drawwbutton(x, y, keyW, keyH, keys[k]);
                keypad[k][0] = x; keypad[k][1] = y;
                keypad[k][2] = keyW; keypad[k][3] = keyH;
                k++;
            }
        }

        bool running = true;
        while (running) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);

                // Keypad
                for (int i = 0; i < 10; i++) {
                    if (isInside(mx, my, keypad[i][0], keypad[i][1], keypad[i][2], keypad[i][3])) {
                        if (strlen(input) < 6) { // Limit amount to 6 digits
                            strcat(input, keys[i]);
                            amount = atoi(input);
                        }
                    }
                }

                // Presets
                if (isInside(mx, my, 250, 230, 150, 50)) strcpy(input, "500");
                else if (isInside(mx, my, 430, 230, 150, 50)) strcpy(input, "1000");
                else if (isInside(mx, my, 250, 310, 150, 50)) strcpy(input, "2000");
                else if (isInside(mx, my, 430, 310, 150, 50)) strcpy(input, "3000");

                // Delete
                else if (isInside(mx, my, 120, 450, 100, 30)) {
                    int len = strlen(input);
                    if (len > 0) input[len - 1] = '\0';
                }

                // Enter
                else if (isInside(mx, my, 520, 430, 100, 50)) {
                    amount = atoi(input);
                    if (amount > 0 && amount <= account.getBalance()) {
                        account.setBalance(account.getBalance() - amount);
                        sprintf(balanceStr, "%d/-", account.getBalance());
                        setfillstyle(SOLID_FILL, BLUE);
                        bar(30, 150, 200, 170);
                        setbkcolor(BLUE);
                        setcolor(WHITE);
                        settextstyle(6, 0, 4);
                        outtextxy(30, 150, balanceStr);
                        strcpy(input, "");
                    }
                }
                else if (isInside(mx, my, 400, 430, 100, 50)) {
                    running = false;
                }

                // Update input box
                setfillstyle(SOLID_FILL, WHITE);
                bar(inputX + 1, inputY + 1, inputX + inputW - 1, inputY + inputH - 1);
                setbkcolor(WHITE);
                setcolor(BLACK);
                outtextxy(inputX + 10, inputY + 20, input);
            }
        }
    }
};

class DepositScreen : public ATMInterface {
private:
    Account& account;

public:
    DepositScreen(Account& acc) : account(acc) {}

    void show() {
        char input[20] = "";
        int amount = 0;
        char balanceStr[20];

        setbkcolor(BLUE);
        cleardevice();

        setcolor(WHITE);
        settextstyle(10, 0, 3);
        outtextxy(30, 30, "Deposit");

        setcolor(SKY_BLUE);
        settextstyle(10, 0, 4);
        outtextxy(30, 120, "Balance");

        sprintf(balanceStr, "%d/-", account.getBalance());
        setcolor(WHITE);
        settextstyle(6, 0, 4);
        outtextxy(30, 150, balanceStr);

        drawwbutton(250, 120, 330, 60, "Enter amount");
        drawButton(400, 430, 100, 50, "Exit");
        drawButton(520, 430, 100, 50, "Enter");
        drawButton(120, 450, 100, 30, "Delete");

        int inputX = 250, inputY = 120, inputW = 330, inputH = 60;

        // Keypad
        int keyX = 50, keyY = 290, keyW = 50, keyH = 40, spacing = 10;
        const char* keys[10] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        int keypad[10][4], k = 0;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                if (k >= 10) break;
                int x = keyX + col * (keyW + spacing);
                int y = keyY + row * (keyH + spacing);
                drawwbutton(x, y, keyW, keyH, keys[k]);
                keypad[k][0] = x; keypad[k][1] = y;
                keypad[k][2] = keyW; keypad[k][3] = keyH;
                k++;
            }
        }

        bool running = true;
        while (running) {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);

                // Keypad
                for (int i = 0; i < 10; i++) {
                    if (isInside(mx, my, keypad[i][0], keypad[i][1], keypad[i][2], keypad[i][3])) {
                        if (strlen(input) < 6) { // Limit amount to 6 digits
                            strcat(input, keys[i]);
                            amount = atoi(input);
                        }
                    }
                }

                // Delete
                if (isInside(mx, my, 120, 450, 100, 30)) {
                    int len = strlen(input);
                    if (len > 0) input[len - 1] = '\0';
                }

                // Enter
                else if (isInside(mx, my, 520, 430, 100, 50)) {
                    amount = atoi(input);
                    if (amount > 0) {
                        account.setBalance(account.getBalance() + amount);
                        sprintf(balanceStr, "%d/-", account.getBalance());
                        setfillstyle(SOLID_FILL, BLUE);
                        bar(30, 150, 200, 170);
                        setbkcolor(BLUE);
                        setcolor(WHITE);
                        settextstyle(6, 0, 4);
                        outtextxy(30, 150, balanceStr);
                        strcpy(input, "");
                    }
                }

                // Exit
                else if (isInside(mx, my, 400, 430, 100, 50)) {
                    running = false;
                }

                // Update input box
                setfillstyle(SOLID_FILL, WHITE);
                bar(inputX + 1, inputY + 1, inputX + inputW - 1, inputY + inputH - 1);
                setbkcolor(WHITE);
                setcolor(BLACK);
                outtextxy(inputX + 10, inputY + 20, input);
            }
        }
    }
};

class CreateAccountScreen : public ATMInterface {
private:
    Account& account;

public:
    CreateAccountScreen(Account& acc) : account(acc) {}

    void show() {
        char name[50];
        char pin[5];
        char balance[20];
        setbkcolor(BLUE);
        cleardevice();
        
        setcolor(WHITE);
        settextstyle(10, 0, 3);
        outtextxy(200, 50, "Create New Account");
        settextstyle(10, 0, 2);
        outtextxy(70, 100, "Name:");
        settextstyle(10, 0, 2);
        outtextxy(70, 150, "PIN (4-digit):");
        settextstyle(10, 0, 2);
        outtextxy(70, 200, "Initial Balance:");

        getInput(250, 100, name, 49, false);
        getInput(250, 150, pin, 4, true);
        getInput(250, 200, balance, 19, false);

        // Validate inputs
        if (strlen(name) == 0 || strlen(pin) != 4 || atoi(balance) < 0) {
            setbkcolor(BLUE);
            cleardevice();
            settextstyle(10, 0, 3);
            setcolor(WHITE);
            outtextxy(100, 200, "Invalid input! Try again.");
            delay(2000);
            return;
        }

        account.setName(name);
        account.setPin(pin);
        account.setBalance(atoi(balance));
        account.generateAccountNumber();
        account.setCreated(true);

        // Save to file
        ofstream file("accounts.txt", ios::app);
        if (file.is_open()) {
            file << name << " " << pin << " " << account.getAccountNumber() << " " << account.getBalance() << endl;
            file.close();
        }

        setbkcolor(BLUE);
        cleardevice();
        settextstyle(10, 0, 3);
        setcolor(WHITE);
        outtextxy(100, 200, "Account Created Successfully!");
        delay(2000);
    }
};

class PinVerificationScreen : public ATMInterface {
private:
    Account& account;

public:
    PinVerificationScreen(Account& acc) : account(acc) {}

    bool verify() {
        char enteredName[50];
        char enteredPin[5];

        cleardevice();
        setbkcolor(BLUE);
        cleardevice();

        // Get Name
        settextstyle(3, 0, 2);
        setcolor(WHITE);
        outtextxy(180, 70, "Enter Your Name:");
        getInput(180, 100, enteredName, 49, false);

        // Get PIN
        setcolor(WHITE);
        outtextxy(180, 150, "Enter your 4-digit PIN");
        
        int xStart = 180;
        for (int i = 0; i < 4; i++) {
            rectangle(xStart + i * 60, 200, xStart + i * 60 + 50, 250);
        }

        int btnY = 300;
        drawButton(150, btnY, 100, 40, "Delete");
        drawButton(270, btnY, 150, 40, "Continue");
        drawButton(430, btnY, 100, 40, "Exit");

        int pinLen = 0;
        bool running = true;

        while (running) {
            if (kbhit()) {
                char ch = getch();
                if (ch >= '0' && ch <= '9' && pinLen < 4) {
                    enteredPin[pinLen++] = ch;
                    outtextxy(xStart + (pinLen - 1) * 60 + 20, 215, "*");
                } else if (ch == 8 && pinLen > 0) {
                    pinLen--;
                    setfillstyle(SOLID_FILL, BLUE);
                    bar(xStart + pinLen * 60 + 1, 201, xStart + pinLen * 60 + 49, 249);
                    rectangle(xStart + pinLen * 60, 200, xStart + pinLen * 60 + 50, 250);
                } else if (ch == 13 && pinLen == 4) {
                    running = false;
                }
            }

            if (ismouseclick(WM_LBUTTONDOWN)) {
                int mx, my;
                getmouseclick(WM_LBUTTONDOWN, mx, my);

                if (isInside(mx, my, 150, btnY, 100, 40)) {
                    if (pinLen > 0) {
                        pinLen--;
                        setfillstyle(SOLID_FILL, BLUE);
                        bar(xStart + pinLen * 60 + 1, 201, xStart + pinLen * 60 + 49, 249);
                        rectangle(xStart + pinLen * 60, 200, xStart + pinLen * 60 + 50, 250);
                    }
                } 
                else if (isInside(mx, my, 270, btnY, 150, 40)) {
                    if (pinLen == 4) {
                        running = false;
                    }
                } 
                else if (isInside(mx, my, 430, btnY, 100, 40)) {
                    return false;
                }
            }
        }

        enteredPin[4] = '\0';

        // Verify against file
        ifstream file("accounts.txt");
        if (!file.is_open()) {
            showError("No accounts found!");
            return false;
        }

        string line;
        bool found = false;
        while (getline(file, line)) {
            istringstream iss(line);
            string name, pin, accNum;
            int balance;
            
            if (iss >> name >> pin >> accNum >> balance) {
                if (name == enteredName && pin == enteredPin) {
                    found = true;
                    account.setName(name.c_str());
                    account.setPin(pin.c_str());
                    account.setAccountNumber(accNum.c_str());
                    account.setBalance(balance);
                    account.setCreated(true);
                    currentusername = name;
                    break;
                }
            }
        }
        file.close();

        if (!found) {
            showError("Invalid name or PIN!");
            return false;
        }

        return true;
    }

    void showError(const char* message) {
        settextstyle(3, 0, 2);
        setcolor(RED);
        outtextxy(200, 260, (char*)message);
        delay(2000);
    }
};

class ATMController {
private:
    Account account;
    BankScreen bankScreen;
    ATMScreen atmScreen;
    WithdrawScreen withdrawScreen;
    DepositScreen depositScreen;
    CreateAccountScreen createAccountScreen;
    PinVerificationScreen pinVerificationScreen;

public:
    ATMController() : 
        atmScreen(account),
        withdrawScreen(account),
        depositScreen(account),
        createAccountScreen(account),
        pinVerificationScreen(account) {}

    void run() {
        while (true) {
            int option = bankScreen.show();

            if (option == 1) { // Use ATM
                if (pinVerificationScreen.verify()) {
                    while (true) {
                        int atmChoice = atmScreen.show();

                        if (atmChoice == 3) {
                            withdrawScreen.show();
                        } 
                        else if (atmChoice == 4) {
                            depositScreen.show();
                        } 
                        else if (atmChoice == 5) { 
                            break;
                        }
                    }
                }
            } 
            else if (option == 2) { // Create Account
                createAccountScreen.show();
            }
        }
        closegraph();
    }
};

int main() {
    initwindow(700, 550);
    ATMController atm;
    atm.run();
    return 0;
}
