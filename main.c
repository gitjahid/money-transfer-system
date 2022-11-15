#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

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

void main_menu();
void loader(void);
void signin(void);
void signup(void);
void signout(void);
void goto_coords(int x, int y);
int get_balance(char userName[]);
bool user_exists(char userName[]);
void account_menu(char userName[]);
void check_balance(char userName[]);
void user_account(struct Account user);
void print_transactions(char userName[]);
void transfer_money(char sourceUserName[]);
void enter_to_continue(void callback(), char userName[]);

const char ADMIN_USERNAME[] = "jahid";
const char USER_LIST_FILE_PATH[] = "./db/users.txt";
const char TRANSACTION_LOG_FILE_PATH[] = "./db/transaction-log.txt";

int main()
{
    main_menu();
    return 0;
}

void loader(void)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 50000000; j++)
        {
            i++;
            i--;
        }

        printf(".");
    }
}

void goto_coords(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void enter_to_continue(void callback(), char userName[])
{
    printf("\n\n--> PRESS ENTER TO GO %s MENU <--", userName != NULL ? "MAIN" : "ACCOUNT");

    char input = getch();
    if (input == 13)
    {
        callback(userName);
    }
}

bool user_exists(char userName[])
{
    FILE *usersFile;
    struct Account user;

    bool userExists = false;

    usersFile = fopen(USER_LIST_FILE_PATH, "rb");

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(userName, user.userName) == 0)
        {
            userExists = true;
            break;
        }
    }

    fclose(usersFile);

    return userExists;
}

void signin(void)
{
    FILE *usersFile;
    struct Account user;

    bool isFoundUser = false;
    char userName[64];
    char password[256];

    usersFile = fopen(USER_LIST_FILE_PATH, "rb");
    if (usersFile == NULL)
    {
        printf("ERROR:: UNABLE TO OPEN %s FILE.\n", USER_LIST_FILE_PATH);
        exit(0);
    }

    system("cls");
    printf("**********************************\n");
    printf("   ENTER YOUR LOGIN CREDENTIALS   \n");
    printf("**********************************\n\n");
    printf("USERNAME: ");
    scanf("%s", &userName);

    printf("PASWORD: ");
    scanf("%s", &password);

    printf("\nLOGGING INTO YOUR ACCOUNT");
    printf("\nPLEASE WAIT");
    loader();

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(userName, user.userName) == 0)
        {
            isFoundUser = true;
            if (strcmp(password, user.password) != 0)
            {
                printf("\nERROR:: INVALID LOGIN CREDENTIALS. PLEASE TRY AGAIN!");
                enter_to_continue(main_menu, NULL);
                break;
            }

            user_account(user);
            break;
        }

        isFoundUser = false;
    }

    if (isFoundUser != true)
    {
        printf("\n\nERROR:: USER DOESN'T EXISTS!.");
        enter_to_continue(main_menu, NULL);
    }

    fclose(usersFile);
}

void signup(void)
{
    FILE *usersFile;
    struct Account user;

    char password[256];

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

    if (user_exists(user.userName))
    {
        printf("\nERROR:: USER ALREADY EXISTS");
    }
    else
    {
        system("cls");
        printf("WE ARE CREATING YOUR ACCOUNT");
        printf("\nPLEASE WAIT");
        loader();

        system("cls");
        printf("************************\n");
        printf("   NEW ACCOUNT DETAILS  \n");
        printf("************************\n\n");
        printf("NEW ACCOUNT HAS BEEN CREATED FOR -> %s %s\n", user.firstName, user.lastName);
        printf("USERNAME: %s\nPASSWORD: %s", user.userName, user.password);

        fwrite(&user, sizeof(user), 1, usersFile);
    }

    fclose(usersFile);

    enter_to_continue(main_menu, NULL);
}

void signout(void)
{
    system("cls");
    printf("\nPLEASE WAIT, LOGGING OUT");

    loader();

    printf("\nSUCCESSFULLY SIGNED OUT");
    enter_to_continue(main_menu, NULL);
}

void main_menu(void)
{
    int choice;

    system("cls");
    printf("**************************************\n");
    printf("   WELCOME TO MONEY TRANSFER SYSTEM   \n");
    printf("**************************************\n\n");

    printf("1. CREATE NEW ACCOUNT\n");
    printf("2. LOGIN TO EXISTING ACCOUNT\n");
    printf("3. EXIT\n");

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

void account_menu(char userName[])
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
        check_balance(userName);
        break;

    case 2:
        transfer_money(userName);
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

void transfer_money(char sourceUserName[])
{
    FILE *usersFile, *transactionsFile;
    struct Account user;
    struct Transaction transaction;

    bool isDestinationUserFound = false;
    char destinationUserName[64];

    usersFile = fopen(USER_LIST_FILE_PATH, "rb");
    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "ab");

    system("cls");
    printf("********************\n");
    printf("   TRANSFER MONEY   \n");
    printf("********************\n\n");

    printf("ENTER DESTINATION USERNAME: ");
    scanf("%s", &destinationUserName);

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(destinationUserName, user.userName) == 0)
        {
            isDestinationUserFound = true;
            strcpy(transaction.sourceUserName, user.userName);
            strcpy(transaction.destinationUserName, sourceUserName);
            break;
        }

        isDestinationUserFound = false;
    }

    if (isDestinationUserFound == true)
    {
        printf("ENTER AMOUNT TO BE TRANSFERRED: ");
        scanf("%d", &transaction.amount);

        int balance = get_balance(sourceUserName);

        if (transaction.amount <= balance || strcmp(sourceUserName, ADMIN_USERNAME) == 0)
        {
            fwrite(&transaction, sizeof(transaction), 1, transactionsFile);

            printf("\nAMOUNT SUCCESSFULLY TRANSFERRED....");
        }
        else
        {
            printf("\nERROR:: IN-SUFFICIENT BALANCE!!!");
        }
    }
    else
    {
        printf("\nERROR:: DESTINATION USER NOT FOUND IN OUR DATABASE!!!");
    }

    fclose(usersFile);
    fclose(transactionsFile);

    enter_to_continue(account_menu, sourceUserName);
}

void print_transactions(char userName[])
{
    FILE *transactionsFile;
    struct Transaction transaction;

    int serialNoCounter = 1;
    int serialNoCoordsX = 0, serialNoCoordsY = 10, statusCoordsX = 10, statusCoordsY = 10, transactionCoordsX = 25, transactionCoordsY = 10, amountCoordsX = 50, amountCoordsY = 10;

    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "rb");

    printf("LIST OF TRANSACTIONS\n");
    printf("**********************\n");
    goto_coords(serialNoCoordsX, serialNoCoordsY);
    printf("SL NO.\t");
    goto_coords(statusCoordsX, statusCoordsY);
    printf("STATUS.\t");
    goto_coords(transactionCoordsX, transactionCoordsY);
    printf("TRANSACTION ID\t\t");
    goto_coords(amountCoordsX, amountCoordsY);
    printf("AMOUNT\n");

    while (fread(&transaction, sizeof(transaction), 1, transactionsFile))
    {
        if (strcmp(userName, transaction.sourceUserName) == 0 || strcmp(userName, transaction.destinationUserName) == 0)
        {
            goto_coords(serialNoCoordsX, ++serialNoCoordsY);
            printf("%d \t", serialNoCounter);
            serialNoCounter++;

            goto_coords(statusCoordsX, ++statusCoordsY);
            printf("%s \t", strcmp(userName, transaction.sourceUserName) == 0 ? "Received" : "Sent");

            goto_coords(transactionCoordsX, ++transactionCoordsY);
            printf("%s  \t", strcmp(userName, transaction.sourceUserName) == 0 ? transaction.destinationUserName : transaction.sourceUserName);

            goto_coords(amountCoordsX, ++amountCoordsY);
            printf("%d\n", transaction.amount);
        }
    }

    fclose(transactionsFile);
}

int get_balance(char userName[])
{
    FILE *transactionsFile;
    struct Transaction transaction;

    int balance = 0;

    transactionsFile = fopen(TRANSACTION_LOG_FILE_PATH, "rb");

    while (fread(&transaction, sizeof(transaction), 1, transactionsFile))
    {
        if (strcmp(userName, transaction.sourceUserName) == 0)
        {
            balance += transaction.amount;
        }

        if (strcmp(userName, transaction.destinationUserName) == 0)
        {
            balance -= transaction.amount;
        }
    }

    fclose(transactionsFile);

    return balance;
}

void check_balance(char userName[])
{
    system("cls");
    printf("***********************\n");
    printf("   BALANCE DASHBOARD   \n");
    printf("***********************\n\n");

    int balance = get_balance(userName);
    printf("TOTAL BALANCE: %d\n\n", balance);

    print_transactions(userName);
    enter_to_continue(account_menu, userName);
}

void user_account(struct Account user)
{
    system("cls");
    printf("**************************************\n");
    printf("   WELCOME TO MONEY TRANSFER SYSTEM   \n");
    printf("**************************************\n\n");
    printf("WELCOME, %s\n\n", user.firstName);

    account_menu(user.userName);
}
