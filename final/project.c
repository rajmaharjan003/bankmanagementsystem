#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <conio.h> 

struct Account
{
    int accountNumber;
    char name[100];
    char accountType[20];
    float balance;
    char address[200];
    char contactNumber[15];
    char fatherName[100];
    char motherName[100];
    char password[20]; // Added password field
    int pin; // Added PIN field
};

void createAccount(FILE *file);
int login(FILE *file);
void deposit(FILE **file, int accountNumber);
void withdraw(FILE **file, int accountNumber);
void checkBalance(FILE *file, int accountNumber);
void listAccounts(FILE *file);
void transfer(FILE **file, int fromAccount, int toAccount, float amount);
int generateAccountNumber(FILE *file);

int main()
{
    int choice, accountNumber, fromAccount, toAccount;
    float amount;
    FILE *file = fopen("accounts.csv", "r+");

    if (file == NULL)
    {
        file = fopen("accounts.csv", "w+");
        if (file == NULL)
        {
            printf("Error opening file.\n");
            return 1;
        }
    }

    while (1)
    {
        printf("\nBank Management System\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount(file);
            break;
        case 2:
            if (login(file))
            {
                while (1)
                {
                    printf("\nBank Management System\n");
                    printf("1. Deposit Money\n");
                    printf("2. Withdraw Money\n");
                    printf("3. Check Balance\n");
                    printf("4. List Accounts\n");
                    printf("5. Transfer Money\n");
                    printf("6. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice)
                    {
                    case 1:
                        printf("Enter account number: ");
                        scanf("%d", &accountNumber);
                        deposit(&file, accountNumber);
                        break;
                    case 2:
                        printf("Enter account number: ");
                        scanf("%d", &accountNumber);
                        withdraw(&file, accountNumber);
                        break;
                    case 3:
                        printf("Enter account number: ");
                        scanf("%d", &accountNumber);
                        checkBalance(file, accountNumber);
                        break;
                    case 4:
                        listAccounts(file);
                        break;
                    case 5:
                        printf("Enter source account number: ");
                        scanf("%d", &fromAccount);
                        printf("Enter destination account number: ");
                        scanf("%d", &toAccount);
                        printf("Enter amount to transfer: ");
                        scanf("%f", &amount);
                        transfer(&file, fromAccount, toAccount, amount);
                        break;
                    case 6:
                        fclose(file);
                        return 0;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    }
                }
            }
            break;
        case 3:
            fclose(file);
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void createAccount(FILE *file)
{
    struct Account newAccount;
    newAccount.accountNumber = generateAccountNumber(file);
    printf("Generated Account Number: %d\n", newAccount.accountNumber);
    printf("Enter name: ");
    scanf(" %[^\n]", newAccount.name); // To read strings with spaces
    printf("Enter account type (Savings/Checking): ");
    scanf("%s", newAccount.accountType);
    printf("Enter address: ");
    scanf(" %[^\n]", newAccount.address);
    printf("Enter contact number: ");
    scanf("%s", newAccount.contactNumber);
    printf("Enter father's name: ");
    scanf(" %[^\n]", newAccount.fatherName);
    printf("Enter mother's name: ");
    scanf(" %[^\n]", newAccount.motherName);
    printf("Set a password: ");
    scanf("%s", newAccount.password); // Set password
    printf("Set a 4-digit PIN: ");
    scanf("%d", &newAccount.pin); // Set PIN
    newAccount.balance = 0; // Initial balance is 0

    fseek(file, 0, SEEK_END);
    fprintf(file, "%d,%s,%s,%.2f,%s,%s,%s,%s,%s,%d\n", newAccount.accountNumber, newAccount.name, newAccount.accountType, newAccount.balance, newAccount.address, newAccount.contactNumber, newAccount.fatherName, newAccount.motherName, newAccount.password, newAccount.pin);
    printf("Account created successfully! Your account number is %d\n", newAccount.accountNumber);
}

int login(FILE *file)
{
    int accountNumber;
    char password[20];
    struct Account acc;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter password: ");
    scanf("%s", password);

    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber == accountNumber && strcmp(acc.password, password) == 0)
        {
            printf("Login successful!\n");
            return 1;
        }
    }
    printf("Invalid account number or password.\n");
    return 0;
}

int generateAccountNumber(FILE *file)
{
    struct Account acc;
    int maxAccountNumber = 1000; // Start account numbers from 1001

    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber > maxAccountNumber)
        {
            maxAccountNumber = acc.accountNumber;
        }
    }

    return maxAccountNumber + 1;
}

void deposit(FILE **file, int accountNumber)
{
    struct Account acc;
    FILE *tempFile = fopen("temp.csv", "w");
    int found = 0;

    fseek(*file, 0, SEEK_SET);
    while (fscanf(*file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber == accountNumber)
        {
            float depositAmount;
            printf("Enter amount to deposit: ");
            scanf("%f", &depositAmount);
            acc.balance += depositAmount;
            found = 1;
        }
        fprintf(tempFile, "%d,%s,%s,%.2f,%s,%s,%s,%s,%s,%d\n", acc.accountNumber, acc.name, acc.accountType,acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, acc.pin);
    }

    fclose(*file);
    fclose(tempFile);
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");
    *file = fopen("accounts.csv", "r+");

    if (found)
    {
        printf("Deposited successfully!\n");
    }
    else
    {
        printf("Account not found.\n");
    }
}

void withdraw(FILE **file, int accountNumber)
{
    struct Account acc;
    FILE *tempFile = fopen("temp.csv", "w");
    int found = 0;

    fseek(*file, 0, SEEK_SET);
    while (fscanf(*file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber == accountNumber)
        {
            float withdrawAmount;
            printf("Enter amount to withdraw: ");
            scanf("%f", &withdrawAmount);
            if (withdrawAmount > acc.balance)
            {
                printf("Insufficient funds!\n");
            }
            else
            {
                acc.balance -= withdrawAmount;
                found = 1;
            }
        }
        fprintf(tempFile, "%d,%s,%s,%.2f,%s,%s,%s,%s,%s,%d\n", acc.accountNumber, acc.name, acc.accountType, acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, acc.pin);
    }

    fclose(*file);
    fclose(tempFile);
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");
    *file = fopen("accounts.csv", "r+");

    if (found)
    {
        printf("Withdrawal successful!\n");
    }
    else
    {
        printf("Account not found.\n");
    }
}

void checkBalance(FILE *file, int accountNumber)
{
    struct Account acc;
    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber == accountNumber)
        {
            printf("Account balance: %.2f\n", acc.balance);
            return;
        }
    }
    printf("Account not found.\n");
}

void listAccounts(FILE *file)
{
    struct Account acc;
    fseek(file, 0, SEEK_SET);
    printf("\nList of Accounts:\n");
    while (fscanf(file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        printf("Account Number: %d, Name: %s, Type: %s, Balance: %.2f, Address: %s, Contact: %s, Father's Name: %s, Mother's Name: %s\n",
               acc.accountNumber, acc.name, acc.accountType, acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName);
    }
}

void transfer(FILE **file, int fromAccount, int toAccount, float amount)
{
    struct Account acc;
    FILE *tempFile = fopen("temp.csv", "w");
    int fromFound = 0, toFound = 0;
    int enteredPin;

    printf("Enter PIN for source account: ");
    scanf("%d", &enteredPin);

    fseek(*file, 0, SEEK_SET);
    while (fscanf(*file, "%d,%[^,],%[^,],%f,%[^,],%[^,],%[^,],%[^,],%[^,],%d\n", &acc.accountNumber, acc.name, acc.accountType, &acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, &acc.pin) != EOF)
    {
        if (acc.accountNumber == fromAccount)
        {
            if (acc.pin != enteredPin)
            {
                printf("Incorrect PIN.\n");
                fclose(tempFile);
                remove("temp.csv");
                return;
            }
            if (acc.balance >= amount)
            {
                acc.balance -= amount;
                fromFound = 1;
            }
            else
            {
                printf("Insufficient funds in source account.\n");
                fclose(tempFile);
                remove("temp.csv");
                return;
            }
        }
        else if (acc.accountNumber == toAccount)
        {
            acc.balance += amount;
            toFound = 1;
        }
        fprintf(tempFile, "%d,%s,%s,%.2f,%s,%s,%s,%s,%s,%d\n", acc.accountNumber, acc.name, acc.accountType, acc.balance, acc.address, acc.contactNumber, acc.fatherName, acc.motherName, acc.password, acc.pin);
    }

    fclose(*file);
    fclose(tempFile);
    remove("accounts.csv");
    rename("temp.csv", "accounts.csv");
    *file = fopen("accounts.csv", "r+");

    if (fromFound && toFound)
    {
        printf("Transfer successful!\n");
    }
    else
    {
        if (!fromFound)
        {
            printf("Source account not found.\n");
        }
        if (!toFound)
        {
            printf("Destination account not found.\n");
        }
    }
}