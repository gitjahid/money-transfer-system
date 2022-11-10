#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Account
{
    char userName[64];
    char firstName[32];
    char lastName[32];
    char password[50];
};

struct Transaction
{
    char sourceUserName[64];
    char destinationUserName[64];
    long int amount;
};

void signin(void);
void signup(void);
void signout(void);
void show_main_menu();
void transfer_money();
void check_balance(char userName[]);
void list_of_transactions(char userName[]);
void show_account_menu(char userName[]);
void display_account_info(struct Account user);

const char USER_LIST_FILE_PATH[] = "./db/users.txt";
const char TRANSACTION_LOG_FILE_PATH[] = "./db/transaction-log.txt";

int main()
{
    show_main_menu();
    return 0;
}

void show_main_menu()
{
    int choice;

    system("cls");
    printf("**************************************\n");
    printf("   WELCOME TO MONEY TRANSFER SYSTEM   \n");
    printf("**************************************\n\n");

    printf("1. CREATE NEW ACCOUNT\n");
    printf("2. LOGIN TO EXISTING ACCOUNT\n");
    printf("3. Exit\n");

    printf("\nENTER YOUR CHOICE (1-3): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        signup();
        break;

    case 2:
        signin();
        break;

    case 3:
        exit(0);
        break;
    }
}

void show_account_menu(char userName[])
{
    int choice;

    system("cls");
    printf("**************************************\n");
    printf("   WELCOME TO MONEY TRANSFER SYSTEM   \n");
    printf("**************************************\n\n");

    printf("1. CHECK BALANCE\n");
    printf("2. TRANSFER MONEY\n");
    printf("3. LOG OUT\n");
    printf("4. EXIT\n");

    printf("\nENTER YOUR CHOICE (1-4): ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        // check balance
        check_balance(userName);
        break;

    case 2:
        // transfer money
        transfer_money();
        break;

    case 3:
        signout();
        signin();
        break;

    case 4:
        exit(0);
        break;
    }
}

void signin()
{
    struct Account user;

    char userName[64];
    char password[256];
    bool isFoundUser = false;

    FILE *usersFile;

    usersFile = fopen(USER_LIST_FILE_PATH, "rb");
    if (usersFile == NULL)
    {
        printf("ERROR:: UNABLE TO OPEN %s FILE.\n", USER_LIST_FILE_PATH);
    }

    system("cls");
    printf("**********************************\n");
    printf("   ENTER YOUR LOGIN CREDENTIALS   \n");
    printf("**********************************\n\n");
    printf("USERNAME: ");
    scanf("%s", &userName);

    printf("PASWORD: ");
    scanf("%s", &password);

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(userName, user.userName) == 0)
        {
            isFoundUser = true;
            if (strcmp(password, user.password) != 0)
            {
                printf("\nERROR:: INVALID LOGIN CREDENTIALS");
                printf("\n\n--> PRESS ENTER TO GO MAIN MENU <--");

                char input = getch();
                if (input == 13)
                {
                    show_main_menu();
                }

                break;
            }

            display_account_info(user);
            break;
        }

        isFoundUser = false;
    }

    if (isFoundUser != true)
    {
        printf("\nERROR:: USER DOESN'T EXISTS!.");
        printf("\n\n--> PRESS ENTER TO GO MAIN MENU <--");

        char input = getch();
        if (input == 13)
        {
            show_main_menu();
        }
    }

    fclose(usersFile);
}

void signup()
{
    struct Account user;
    FILE *usersFile;
    char password[20];

    usersFile = fopen(USER_LIST_FILE_PATH, "ab");

    system("cls");
    printf("**************************\n");
    printf("   CREATE A NEW ACCOUNT   \n");
    printf("**************************\n\n");

    printf("FIRST NAME: ");
    scanf("%s", &user.firstName);

    printf("LAST NAME: ");
    scanf("%s", &user.lastName);

    printf("USERNAME: ");
    scanf("%s", &user.userName);

    printf("PASSWORD: ");
    scanf("%s", &user.password);

    fwrite(&user, sizeof(user), 1, usersFile);
    fclose(usersFile);

    system("cls");
    printf("*****************\n");
    printf("   NEW ACCOUNT   \n");
    printf("*****************\n\n");
    printf("NEW ACCOUNT HAS BEEN CREATED FOR -> %s %s\n", user.firstName, user.lastName);
    printf("USERNAME: %s\nPASSWORD: %s", user.userName, user.password);

    printf("\n\n--> PRESS ENTER TO GO MAIN MENU <--");

    char input = getch();
    if (input == 13)
    {
        show_main_menu();
    }
}

void signout()
{
    system("cls");
    printf("\nPLEASE WAIT, LOGGING OUT");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 25000000; j++)
        {
            i++;
            i--;
        }

        printf(".");
    }

    printf("\nSUCCESSFULLY SIGNED OUT");
    printf("\n\n--> PRESS ENTER TO GO MAIN MENU <--");

    char input = getch();
    if (input == 13)
    {
        show_main_menu();
    }
}

void transfer_money()
{
    struct Account user;
    struct Transaction transaction;
    FILE *usersFile, *transactionsFile;

    char sourceUserName[64];
    char destinationUserName[64];

    usersFile = fopen(USER_LIST_FILE_PATH, "rb");
    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "ab");

    printf("ENTER SOURCE USERNAME: ");
    scanf("%s", &sourceUserName);

    printf("ENTER DESTINATION USERNAME: ");
    scanf("%s", &destinationUserName);

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(destinationUserName, user.userName) == 0)
        {
            strcpy(transaction.sourceUserName, user.userName);
            strcpy(transaction.destinationUserName, sourceUserName);
        }
    }

    printf("ENTER AMOUNT TO BE TRANSFERRED: ");
    scanf("%d", &transaction.amount);

    fwrite(&transaction, sizeof(transaction), 1, transactionsFile);

    printf("AMOUNT SUCCESSFULLY TRANSFERRED....");

    fclose(usersFile);
    fclose(transactionsFile);
}

void list_of_transactions(char userName[])
{
    struct Transaction transaction;
    FILE *transactionsFile;
    int serialNoCounter = 1;

    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "rb");

    printf("LIST OF TRANSACTIONS\n");
    printf("**********************\n");
    printf("SL NO.\t");
    printf("STATUS.\t");
    printf("TRANSACTION ID\t\t");
    printf("AMOUNT\n");

    while (fread(&transaction, sizeof(transaction), 1, transactionsFile))
    {
        if (strcmp(userName, transaction.sourceUserName) == 0 || strcmp(userName, transaction.destinationUserName) == 0)
        {
            printf("%d \t", serialNoCounter);
            serialNoCounter++;

            printf("%s \t", strcmp(userName, transaction.sourceUserName) == 0 ? "Received" : "Sent");

            printf("%s  \t", transaction.destinationUserName);

            printf("%d\n", transaction.amount);
        }
    }

    fclose(transactionsFile);
}

void check_balance(char userName[])
{
    struct Transaction transaction;
    FILE *transactionsFile;

    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "rb");

    system("cls");
    printf("***********************\n");
    printf("   BALANCE DASHBOARD   \n");
    printf("***********************\n\n");

    int balance = 0;

    while (fread(&transaction, sizeof(transaction), 1, transactionsFile))
    {
        if (strcmp(userName, transaction.sourceUserName) == 0)
        {
            balance += transaction.amount;
        }
    }

    fclose(transactionsFile);

    printf("TOTAL BALANCE: %d\n\n", balance);

    list_of_transactions(userName);

    printf("\n--> PRESS ENTER TO GO ACCOUNT MENU <--");

    char input = getch();
    if (input == 13)
    {
        show_account_menu(userName);
    }
}

void display_account_info(struct Account user)
{
    system("cls");
    printf("**************************************\n");
    printf("   WELCOME TO MONEY TRANSFER SYSTEM   \n");
    printf("**************************************\n\n");
    printf("WELCOME, %s\n\n", user.firstName);

    show_account_menu(user.userName);
}
