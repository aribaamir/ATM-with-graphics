# 💳 ATM Simulation in C++ (graphics.h)

A simple graphical ATM simulation using C++ and `graphics.h`. Features include:

- Create account with name, 4-digit PIN, and initial balance
- Login with name and PIN
- Withdraw and deposit money via mouse GUI
- On-screen keypad and balance display
- Data saved in `accounts.txt`

## 🛠 Requirements
- Turbo C++ / DOSBox  
- Or: g++ with WinBGIm graphics setup

## 🧪 Run (Modern Setup)
```bash
g++ main.cpp -o atm -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
./atm
