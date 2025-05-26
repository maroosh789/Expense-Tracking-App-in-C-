#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;

struct Expense {
    int id;
    string name;
    float price;
    tm date;
    Expense* next;
};

// Master list node 
struct MasterNode {
    Expense* expense;
    MasterNode* next;
};


Expense* head = nullptr;
MasterNode* masterHead = nullptr;

int currentId = 1;


tm parseDate() {
    tm date = {};
    int d, m, y;
    cout << "Enter date (dd mm yyyy): ";
    cin >> d >> m >> y;
    date.tm_mday = d;
    date.tm_mon = m - 1;
    date.tm_year = y - 1900;
    return date;
}

// Function to add new expense
void addExpense() {
    Expense* newExp = new Expense;
    newExp->id = currentId++;

    cout << "Enter expense name: ";
    cin.ignore();
    getline(cin, newExp->name);

    cout << "Enter expense price: ";
    cin >> newExp->price;

    newExp->date = parseDate();
    newExp->next = nullptr;

    // Add to expense list
    if (!head) {
        head = newExp;
    } else {
        Expense* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newExp;
    }

    // Add to master list
    MasterNode* mNode = new MasterNode;
    mNode->expense = newExp;
    mNode->next = nullptr;

    if (!masterHead) {
        masterHead = mNode;
    } else {
        MasterNode* temp = masterHead;
        while (temp->next)
            temp = temp->next;
        temp->next = mNode;
    }

    cout << "Expense added successfully! ";
}

// Function to display a single expense
void displayExpense(Expense* exp) {
    cout << "ID: " << exp->id << ", Name: " << exp->name
         << ", Price: $" << fixed << setprecision(2) << exp->price
         << ", Date: " << put_time(&exp->date, "%d-%m-%Y") << endl;
}

// View all expenses
void viewAllExpenses() {
    if (!head) {
        cout << "No expenses to show ";
        return;
    }

    Expense* temp = head;
    while (temp) {
        displayExpense(temp);
        temp = temp->next;
    }
}


bool sameMonth(const tm& a, const tm& b) {
    return (a.tm_mon == b.tm_mon && a.tm_year == b.tm_year);
}


bool sameWeek(const tm& a, const tm& b) {
    time_t ta = mktime((tm*)&a);
    time_t tb = mktime((tm*)&b);

    int daysA = ta / (60 * 60 * 24);
    int daysB = tb / (60 * 60 * 24);

    return ((daysA / 7) == (daysB / 7));
}

void reportExpenses(bool monthly) {
    if (!masterHead) {
        cout << "No expenses available ";
        return;
    }

    time_t now = time(0);
    tm* curr = localtime(&now);

    MasterNode* temp = masterHead;
    float total = 0;

    cout << (monthly ? " Monthly Report " : " Weekly Report ");

    while (temp) {
        Expense* exp = temp->expense;
        if ((monthly && sameMonth(*curr, exp->date)) ||
            (!monthly && sameWeek(*curr, exp->date))) {
            displayExpense(exp);
            total += exp->price;
        }
        temp = temp->next;
    }

    cout << "Total " << (monthly ? "monthly" : "weekly") << " expenses: $" << fixed << setprecision(2) << total << "  ";
}

// Main Menu
void menu() {
    int choice;
    while (true) {
        cout << " Expense Tracker ";
        cout << "1. Add Expense ";
        cout << "2. View All Expenses ";
        cout << "3. Monthly Report ";
        cout << "4. Weekly Report ";
        cout << "5. Exit ";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewAllExpenses(); break;
            case 3: reportExpenses(true); break;
            case 4: reportExpenses(false); break;
            case 5: exit(0);
            default: cout << "Invalid option. Try again";
        }
    }
}

int main() {
    menu();
    return 0;
}

