#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#define USER_FILE "users.txt"
#define LOG_FILE "log.txt"
#define MAX_LEN 50

void writeLog(const char *user, const char *operation)
{
  FILE *fp = fopen(LOG_FILE, "a");
  if (fp == NULL)
  {
    return;
  }

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d %s %s\n",
          t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
          t->tm_hour, t->tm_min, t->tm_sec,
          user, operation);

  fclose(fp);
}

void readPassword(char *dest)
{
  int i = 0;
  char ch;

  while (1)
  {
    ch = getch();

    if (ch == '\r' || ch == '\n')
    {
      break;
    }
    else if (ch == '\b' || ch == 127)
    {
      if (i > 0)
      {
        i--;
        printf("\b \b");
      }
    }
    else if (i < MAX_LEN - 1)
    {
      dest[i++] = ch;
      printf("*");
    }
  }

  dest[i] = '\0';
  printf("\n");
}

void clearUI()
{
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void header()
{
  clearUI();
  printf("\n\n");
  printf("==============================\n");
  printf("|                            |\n");
  printf("|         CALCULATOR         |\n");
  printf("|                            |\n");
  printf("==============================\n");
}

void loginUI()
{
  header();
  printf("|                            |\n");
  printf("|   1. ADMIN LOGIN           |\n");
  printf("|   2. USER LOGIN            |\n");
  printf("|   3. Exit                  |\n");
  printf("|                            |\n");
  printf("|   0. DEVELOPER INFO        |\n");
  printf("|                            |\n");
  printf("------------------------------\n");
  printf("\n\n");
  printf("Select an option (0-3): ");
}

void calcUI()
{
  header();
  printf("|                            |\n");
  printf("|   1. Addition (+)          |\n");
  printf("|   2. Subtraction (-)       |\n");
  printf("|   3. Multiplication (*)    |\n");
  printf("|   4. Division (/)          |\n");
  printf("|   5. Log Out               |\n");
  printf("|                            |\n");
  printf("------------------------------\n");
  printf("\n\n");
  printf("Select an option (1-5): ");
}

void adminUI()
{
  header();
  printf("|        ADMIN PANEL         |\n");
  printf("|----------------------------|\n");
  printf("|                            |\n");
  printf("|   1. Create New User       |\n");
  printf("|   2. Delete Existing User  |\n");
  printf("|   3. View All Users        |\n");
  printf("|   4. View Logs             |\n");
  printf("|   5. Log Out               |\n");
  printf("|                            |\n");
  printf("------------------------------\n");
  printf("\n\n");
  printf("Select an option (1-5): ");
}

int adminLogin()
{
  char username[MAX_LEN], password[MAX_LEN];

  header();
  printf("\n--- ADMIN LOGIN ---\n\n");

  printf("Enter Admin Username: ");
  scanf("%s", username);
  printf("Enter Admin Password: ");
  readPassword(password);

  if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
  {
    writeLog("admin", "Logged in (Admin)");

    printf("\n>> Admin login successful!\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return 1;
  }
  else
  {
    printf("\n>> ERROR: Invalid admin credentials!\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar();
    getchar();
    return 0;
  }
}

int userLogin(char *loggedInUser)
{
  char username[MAX_LEN], password[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int found = 0;

  header();
  printf("\n--- USER LOGIN ---\n\n");

  printf("Enter Username: ");
  scanf("%s", username);
  printf("Enter Password: ");
  readPassword(password);

  FILE *fp = fopen(USER_FILE, "r");

  if (fp == NULL)
  {
    printf("\n>> ERROR: No users exist yet. Ask an Admin to create your account.\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar();
    getchar();
    return 0;
  }

  while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
  {
    if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0)
    {
      found = 1;
      break;
    }
  }
  fclose(fp);

  if (found)
  {
    strcpy(loggedInUser, username);
    writeLog(username, "Logged in (User)");

    printf("\n>> Login successful! Welcome, %s.\n", username);
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return 1;
  }
  else
  {
    printf("\n>> ERROR: Invalid username or password!\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar();
    getchar();
    return 0;
  }
}

void createUser()
{
  char username[MAX_LEN], password[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int duplicate = 0;

  header();
  printf("\n--- CREATE NEW USER ---\n\n");

  printf("Enter new Username: ");
  scanf("%s", username);

  if (strcmp(username, "admin") == 0)
  {
    printf("\n>> ERROR: 'admin' is a reserved username. Choose a different one.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  FILE *fp = fopen(USER_FILE, "r");
  if (fp != NULL)
  {
    while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
    {
      if (strcmp(username, fileUser) == 0)
      {
        duplicate = 1;
        break;
      }
    }
    fclose(fp);
  }

  if (duplicate)
  {
    printf("\n>> ERROR: Username '%s' already exists!\n", username);
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  printf("Enter Password for '%s': ", username);
  readPassword(password);

  fp = fopen(USER_FILE, "a");
  if (fp == NULL)
  {
    printf("\n>> ERROR: Could not open the user file for writing.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  fprintf(fp, "%s %s\n", username, password);
  fclose(fp);

  char logMsg[MAX_LEN + 20];
  sprintf(logMsg, "Created user: %s", username);
  writeLog("admin", logMsg);

  printf("\n>> User '%s' created successfully!\n", username);
  printf("\nPress ENTER to continue...");
  getchar();
  getchar();
}

void deleteUser()
{
  char username[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int found = 0;

  header();
  printf("\n--- DELETE USER ---\n\n");

  printf("Enter the Username to delete: ");
  scanf("%s", username);

  FILE *fp = fopen(USER_FILE, "r");
  if (fp == NULL)
  {
    printf("\n>> ERROR: No users exist yet. Nothing to delete.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  FILE *temp = fopen("temp_users.txt", "w");
  if (temp == NULL)
  {
    printf("\n>> ERROR: Could not create temporary file.\n");
    fclose(fp);
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
  {
    if (strcmp(username, fileUser) == 0)
    {
      found = 1;
    }
    else
    {
      fprintf(temp, "%s %s\n", fileUser, filePass);
    }
  }

  fclose(fp);
  fclose(temp);

  remove(USER_FILE);
  rename("temp_users.txt", USER_FILE);

  if (found)
  {
    char logMsg[MAX_LEN + 20];
    sprintf(logMsg, "Deleted user: %s", username);
    writeLog("admin", logMsg);

    printf("\n>> User '%s' deleted successfully!\n", username);
  }
  else
  {
    printf("\n>> ERROR: User '%s' not found!\n", username);
  }

  printf("\nPress ENTER to continue...");
  getchar();
  getchar();
}

void viewAllUsers()
{
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int count = 0;

  header();
  printf("\n--- REGISTERED USERS ---\n\n");

  FILE *fp = fopen(USER_FILE, "r");
  if (fp == NULL)
  {
    printf("   No users have been created yet.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
  {
    count++;
    printf("   %d. %s\n", count, fileUser);
  }
  fclose(fp);

  if (count == 0)
  {
    printf("   No users have been created yet.\n");
  }
  else
  {
    printf("\n   Total Users: %d\n", count);
  }

  printf("\nPress ENTER to continue...");
  getchar();
  getchar();
}

void viewLogs()
{
  char line[256];
  int count = 0;

  header();
  printf("\n--- ACTIVITY LOG ---\n\n");

  FILE *fp = fopen(LOG_FILE, "r");
  if (fp == NULL)
  {
    printf("   No log entries found.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  while (fgets(line, sizeof(line), fp) != NULL)
  {
    count++;
    printf("   %d. %s", count, line);
  }
  fclose(fp);

  if (count == 0)
  {
    printf("   No log entries found.\n");
  }
  else
  {
    printf("\n   Total Log Entries: %d\n", count);
  }

  printf("\nPress ENTER to continue...");
  getchar();
  getchar();
}

void adminPanel()
{
  int choice;

  for (;;)
  {
    adminUI();
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      createUser();
      break;

    case 2:
      deleteUser();
      break;

    case 3:
      viewAllUsers();
      break;

    case 4:
      viewLogs();
      break;

    case 5:
      writeLog("admin", "Logged out");
      clearUI();
      return;

    default:
      printf("\n>> Enter a valid option (1-5) and try again.\n");
      printf("\nPress ENTER to continue...");
      getchar();
      getchar();
      break;
    }
  }
}

void calculatorPanel(const char *username)
{
  int choice;

  for (;;)
  {
    calcUI();
    float num1, num2, result;

    scanf("%d", &choice);
    if (choice == 5)
    {
      writeLog(username, "Logged out");
      clearUI();
      break;
    }

    char logMsg[120];

    switch (choice)
    {
    case 1:
      clearUI();
      printf("\n");
      printf("========================\n");
      printf("|       ADDITION       |\n");
      printf("========================\n");
      printf("\n");

      printf("Enter your first number: ");
      scanf("%f", &num1);
      printf("Enter the second number: ");
      scanf("%f", &num2);

      result = num1 + num2;

      sprintf(logMsg, "Addition: %.3f + %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar();
      getchar();

      break;

    case 2:
      clearUI();
      printf("\n");
      printf("===========================\n");
      printf("|       SUBTRACTION       |\n");
      printf("===========================\n");
      printf("\n");

      printf("Enter your first number: ");
      scanf("%f", &num1);
      printf("Enter the second number: ");
      scanf("%f", &num2);

      result = num1 - num2;

      sprintf(logMsg, "Subtraction: %.3f - %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar();
      getchar();

      break;

    case 3:
      clearUI();
      printf("\n");
      printf("==============================\n");
      printf("|       MULTIPLICATION       |\n");
      printf("==============================\n");
      printf("\n");

      printf("Enter your first number: ");
      scanf("%f", &num1);
      printf("Enter the second number: ");
      scanf("%f", &num2);

      result = num1 * num2;

      sprintf(logMsg, "Multiplication: %.3f * %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar();
      getchar();

      break;

    case 4:
      clearUI();
      printf("\n");
      printf("========================\n");
      printf("|       DIVISION       |\n");
      printf("========================\n");
      printf("\n");

      printf("Enter your first number: ");
      scanf("%f", &num1);
      printf("Enter the second number: ");
      scanf("%f", &num2);

      if (num2 == 0)
      {
        printf("\n>> ERROR: Division by zero is not allowed!\n");

        sprintf(logMsg, "Division: %.3f / %.3f = ERROR (div by zero)", num1, num2);
        writeLog(username, logMsg);
      }
      else
      {
        result = num1 / num2;

        sprintf(logMsg, "Division: %.3f / %.3f = %.3f", num1, num2, result);
        writeLog(username, logMsg);

        printf("\nResult: %.3f\n", result);
      }

      printf("\nPress ENTER key to return to the main menu...");
      getchar();
      getchar();

      break;

    default:
      printf("Enter a correct number (1-5) and try again.");
      break;
    }
  }
}

int main()
{
  int choice;
  char loggedInUser[MAX_LEN];

  for (;;)
  {
    loginUI();
    scanf("%d", &choice);

    switch (choice)
    {
    case 0:
      header();
      printf("|                            |\n");
      printf("|     Shahriar Hasan Siam    |\n");
      printf("|                            |\n");
      printf("------------------------------\n");

      printf("\n\n");
      printf("Press ENTER to continue...");
      getchar();
      getchar();
      break;

    case 1:
      if (adminLogin())
      {
        adminPanel();
      }
      break;

    case 2:
      if (userLogin(loggedInUser))
      {
        calculatorPanel(loggedInUser);
      }
      break;

    case 3:
      clearUI();
      printf("\n>> Thank you for using the Calculator. Goodbye!\n\n");
      return 0;

    default:
      printf("\n>> Enter a valid option (0-3) and try again.\n");
      printf("\nPress ENTER to continue...");
      getchar();
      getchar();
      break;
    }
  }
}
