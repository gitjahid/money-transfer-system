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

void goto_coords(int x, int y);
void enter_to_continue(void callback(), char userName[]);
void signin(void);
void signup(void);
void signout(void);
void show_main_menu();
void show_account_menu(char userName[]);
void list_of_transactions(char userName[]);
int get_available_balance(char userName[]);
void transfer_money(char sourceUserName[]);
void check_balance(char userName[]);
void display_account_info(struct Account user);

const char ADMIN_USERNAME[] = "jahid";
const char USER_LIST_FILE_PATH[] = "./db/users.txt";
const char TRANSACTION_LOG_FILE_PATH[] = "./db/transaction-log.txt";

int main()
{
    show_main_menu();
    return 0;
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

void signin()
{
    struct Account user;
    FILE *usersFile;

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

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(userName, user.userName) == 0)
        {
            isFoundUser = true;
            if (strcmp(password, user.password) != 0)
            {
                printf("\nERROR:: INVALID LOGIN CREDENTIALS");

                enter_to_continue(show_main_menu, NULL);
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
        enter_to_continue(show_main_menu, NULL);
    }

    fclose(usersFile);
}

void signup()
{
    struct Account user;
    FILE *usersFile;

    bool isUserExists = false;
    char password[20];
    char userName[64];

    usersFile = fopen(USER_LIST_FILE_PATH, "ab+");

    system("cls");
    printf("**************************\n");
    printf("   CREATE A NEW ACCOUNT   \n");
    printf("**************************\n\n");

    printf("FIRST NAME: ");
    scanf("%s", &user.firstName);

    printf("LAST NAME: ");
    scanf("%s", &user.lastName);

    printf("USERNAME: ");
    scanf("%s", &userName);

    printf("PASSWORD: ");
    scanf("%s", &user.password);

    while (fread(&user, sizeof(user), 1, usersFile))
    {
        if (strcmp(userName, user.userName) == 0)
        {
            isUserExists = true;
            break;
        }
    }

    if (isUserExists == true)
    {
        printf("\nERROR:: USER ALREADY EXISTS");
    }
    else
    {
        strcpy(user.userName, userName);

        system("cls");
        printf("*****************\n");
        printf("   NEW ACCOUNT   \n");
        printf("*****************\n\n");
        printf("NEW ACCOUNT HAS BEEN CREATED FOR -> %s %s\n", user.firstName, user.lastName);
        printf("USERNAME: %s\nPASSWORD: %s", user.userName, user.password);

        fwrite(&user, sizeof(user), 1, usersFile);
    }

    fclose(usersFile);
    enter_to_continue(show_main_menu, NULL);
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
    enter_to_continue(show_main_menu, NULL);
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

void list_of_transactions(char userName[])
{
    struct Transaction transaction;
    FILE *transactionsFile;

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

int get_available_balance(char userName[])
{
    struct Transaction transaction;
    FILE *transactionsFile;

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

void transfer_money(char sourceUserName[])
{
    struct Account user;
    struct Transaction transaction;
    FILE *usersFile, *transactionsFile;

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

        int balance = get_available_balance(sourceUserName);

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
    enter_to_continue(show_account_menu, sourceUserName);
}

void check_balance(char userName[])
{
    system("cls");
    printf("***********************\n");
    printf("   BALANCE DASHBOARD   \n");
    printf("***********************\n\n");

    int balance = get_available_balance(userName);
    printf("TOTAL BALANCE: %d\n\n", balance);

    list_of_transactions(userName);
    enter_to_continue(show_account_menu, userName);
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
