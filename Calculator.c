#include <stdio.h>   // Enables Standard Input and Output in C code
#include <stdlib.h>  // Essential Header for using `system()` function
#include <string.h>  // Required for string operations (strcmp, strcpy, etc.)
#include <time.h>    // Required for date/time stamps in log entries
#include <conio.h>   // Required for getch() — password masking on Windows

// ===== File path for storing user credentials =====
// Users are stored as "username password" per line in this file.
// The admin account is hardcoded and NOT stored in this file.
#define USER_FILE "users.txt"

// ===== File path for the activity log =====
// Logs are stored as "YYYY-MM-DD HH:MM:SS <user> <operation>" per line.
#define LOG_FILE "log.txt"

// ===== Maximum length for username and password strings =====
#define MAX_LEN 50

// ============================================================
// FUNCTION: writeLog
// PURPOSE:  Appends a timestamped log entry to LOG_FILE.
//           Format: "YYYY-MM-DD HH:MM:SS <user> <operation>"
// PARAMS:   user      - the username performing the action
//           operation - description of what happened
// ============================================================
void writeLog(const char *user, const char *operation)
{
  FILE *fp = fopen(LOG_FILE, "a");
  if (fp == NULL)
  {
    // Silently fail — logging should not block the program
    return;
  }

  // Get the current date and time
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  // Write log entry in format: YYYY-MM-DD HH:MM:SS <user> <operation>
  fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d %s %s\n",
          t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
          t->tm_hour, t->tm_min, t->tm_sec,
          user, operation);

  fclose(fp);
}

// ============================================================
// FUNCTION: readPassword
// PURPOSE:  Reads a password from the user with masking.
//           Each typed character is replaced with '*' on screen.
//           Supports backspace to delete characters.
//           The password is null-terminated and stored in `dest`.
// PARAMS:   dest - buffer to store the password (must be MAX_LEN)
// ============================================================
void readPassword(char *dest)
{
  int i = 0;  // Current position in the password buffer
  char ch;    // Holds each character as it is typed

  // Read characters one by one until Enter (\r) is pressed
  while (1)
  {
    ch = getch(); // Read a single character without echoing it

    if (ch == '\r' || ch == '\n') // Enter key pressed — done
    {
      break;
    }
    else if (ch == '\b' || ch == 127) // Backspace key pressed
    {
      if (i > 0)
      {
        i--;
        // Move cursor back, overwrite the '*' with space, move back again
        printf("\b \b");
      }
    }
    else if (i < MAX_LEN - 1) // Regular character (within buffer limit)
    {
      dest[i++] = ch;
      printf("*"); // Show '*' instead of the actual character
    }
  }

  dest[i] = '\0'; // Null-terminate the password string
  printf("\n");   // Move to the next line after password entry
}

// ===== Utility: Clear the terminal screen (cross-platform) =====
void clearUI()
{
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

// ===== UI: Print the application header banner =====
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

// ===== UI: Print the main login menu =====
void loginUI()
{
  header();
  // printf("|----------------------------|\n");
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

// ===== UI: Print the calculator operation menu =====
void calcUI()
{
  header();
  // printf("|----------------------------|\n");
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

// ===== UI: Print the admin panel menu =====
void adminUI()
{
  header();
  // printf("|----------------------------|\n");
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

// ============================================================
// FUNCTION: adminLogin
// PURPOSE:  Validates admin credentials (hardcoded).
//           Admin username = "admin", password = "admin".
// RETURNS:  1 if login is successful, 0 otherwise.
// ============================================================
int adminLogin()
{
  char username[MAX_LEN], password[MAX_LEN];

  header();
  printf("\n--- ADMIN LOGIN ---\n\n");

  printf("Enter Admin Username: ");
  scanf("%s", username);
  printf("Enter Admin Password: ");
  readPassword(password); // Masked input — shows '*' instead of characters

  // Compare entered credentials with hardcoded admin credentials
  if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
  {
    // Log: Admin login
    writeLog("admin", "Logged in (Admin)");

    printf("\n>> Admin login successful!\n");
    printf("\nPress ENTER to continue...");
    getchar(); // Catch newline from scanf
    getchar(); // Wait for user to press ENTER
    return 1;  // Login success
  }
  else
  {
    printf("\n>> ERROR: Invalid admin credentials!\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar(); // Catch newline from scanf
    getchar(); // Wait for user to press ENTER
    return 0;  // Login failed
  }
}

// ============================================================
// FUNCTION: userLogin
// PURPOSE:  Validates user credentials against the users.txt file.
//           Each line in the file has format: "username password"
//           On success, copies the username into loggedInUser so
//           the caller can track who is logged in.
// RETURNS:  1 if login is successful, 0 otherwise.
// ============================================================
int userLogin(char *loggedInUser)
{
  char username[MAX_LEN], password[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int found = 0; // Flag to track if credentials matched

  header();
  printf("\n--- USER LOGIN ---\n\n");

  printf("Enter Username: ");
  scanf("%s", username);
  printf("Enter Password: ");
  readPassword(password); // Masked input — shows '*' instead of characters

  // Open the users file for reading
  FILE *fp = fopen(USER_FILE, "r");

  // If the file doesn't exist, no users have been created yet
  if (fp == NULL)
  {
    printf("\n>> ERROR: No users exist yet. Ask an Admin to create your account.\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar();
    getchar();
    return 0;
  }

  // Read each line and compare username + password
  while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
  {
    if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0)
    {
      found = 1; // Credentials matched
      break;
    }
  }
  fclose(fp);

  if (found)
  {
    // Copy the username out so the caller knows who logged in
    strcpy(loggedInUser, username);

    // Log: User login
    writeLog(username, "Logged in (User)");

    printf("\n>> Login successful! Welcome, %s.\n", username);
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return 1; // Login success
  }
  else
  {
    printf("\n>> ERROR: Invalid username or password!\n");
    printf("\nPress ENTER to return to the main menu...");
    getchar();
    getchar();
    return 0; // Login failed
  }
}

// ============================================================
// FUNCTION: createUser
// PURPOSE:  Allows admin to add a new user to users.txt.
//           Checks for duplicate usernames before creating.
// ============================================================
void createUser()
{
  char username[MAX_LEN], password[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int duplicate = 0; // Flag to track duplicate usernames

  header();
  printf("\n--- CREATE NEW USER ---\n\n");

  printf("Enter new Username: ");
  scanf("%s", username);

  // Prevent creating a user with the reserved admin username
  if (strcmp(username, "admin") == 0)
  {
    printf("\n>> ERROR: 'admin' is a reserved username. Choose a different one.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  // Check if the username already exists in the file
  FILE *fp = fopen(USER_FILE, "r");
  if (fp != NULL)
  {
    while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
    {
      if (strcmp(username, fileUser) == 0)
      {
        duplicate = 1; // Username already taken
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

  // Username is unique — ask for the password
  printf("Enter Password for '%s': ", username);
  readPassword(password); // Masked input — shows '*' instead of characters

  // Append the new user to the file
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

  // Log: Admin created a new user
  // Build a descriptive operation string like "Created user: siam"
  char logMsg[MAX_LEN + 20];
  sprintf(logMsg, "Created user: %s", username);
  writeLog("admin", logMsg);

  printf("\n>> User '%s' created successfully!\n", username);
  printf("\nPress ENTER to continue...");
  getchar();
  getchar();
}

// ============================================================
// FUNCTION: deleteUser
// PURPOSE:  Allows admin to remove a user from users.txt.
//           Rewrites the entire file without the deleted user.
// ============================================================
void deleteUser()
{
  char username[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int found = 0; // Flag to track if the user was found

  header();
  printf("\n--- DELETE USER ---\n\n");

  printf("Enter the Username to delete: ");
  scanf("%s", username);

  // Open the original file for reading
  FILE *fp = fopen(USER_FILE, "r");
  if (fp == NULL)
  {
    printf("\n>> ERROR: No users exist yet. Nothing to delete.\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    return;
  }

  // Open a temporary file for writing (will hold all users except the deleted one)
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

  // Copy all users except the one being deleted
  while (fscanf(fp, "%s %s", fileUser, filePass) == 2)
  {
    if (strcmp(username, fileUser) == 0)
    {
      found = 1; // Mark user as found (skip writing this entry)
    }
    else
    {
      fprintf(temp, "%s %s\n", fileUser, filePass); // Keep this user
    }
  }

  fclose(fp);
  fclose(temp);

  // Replace the original file with the updated temp file
  remove(USER_FILE);
  rename("temp_users.txt", USER_FILE);

  if (found)
  {
    // Log: Admin deleted a user
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

// ============================================================
// FUNCTION: viewAllUsers
// PURPOSE:  Displays all registered usernames from users.txt.
//           Passwords are hidden for security.
// ============================================================
void viewAllUsers()
{
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int count = 0; // Counter for the number of users

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

  // Print each username with a serial number
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

// ============================================================
// FUNCTION: viewLogs
// PURPOSE:  Displays all log entries from log.txt.
//           Each line is printed as-is (timestamped entries).
// ============================================================
void viewLogs()
{
  char line[256]; // Buffer to hold each log line
  int count = 0;  // Counter for log entries

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

  // Read and display each log entry line by line
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

// ============================================================
// FUNCTION: adminPanel
// PURPOSE:  The admin dashboard loop. Admin can create users,
//           delete users, view all users, view logs, or log out.
// ============================================================
void adminPanel()
{
  int choice;

  for (;;)
  {
    adminUI(); // Display the admin menu
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      createUser();  // Create a new user
      break;

    case 2:
      deleteUser();  // Delete an existing user
      break;

    case 3:
      viewAllUsers(); // Show all registered users
      break;

    case 4:
      viewLogs(); // Show activity log entries
      break;

    case 5:
      // Log: Admin logged out
      writeLog("admin", "Logged out");
      clearUI();
      return; // Log out — return to main login menu

    default:
      printf("\n>> Enter a valid option (1-5) and try again.\n");
      printf("\nPress ENTER to continue...");
      getchar();
      getchar();
      break;
    }
  }
}

// ============================================================
// FUNCTION: calculatorPanel
// PURPOSE:  The calculator dashboard loop. User can perform
//           arithmetic operations or log out.
// PARAMS:   username - the name of the currently logged-in user,
//                      used to tag log entries.
// ============================================================
void calculatorPanel(const char *username)
{
  int choice;

  for (;;)
  {
    calcUI(); // Display the calculator menu
    float num1, num2, result;

    scanf("%d", &choice);
    if (choice == 5)
    {
      // Log: User logged out
      writeLog(username, "Logged out");
      clearUI();
      break; // Log out — return to main login menu
    }

    // Temporary buffer for building log messages
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

      // Log: Addition operation with operands and result
      sprintf(logMsg, "Addition: %.3f + %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar(); // Catch the newline from input
      getchar(); // Wait for the user to press ENTER key

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

      // Log: Subtraction operation with operands and result
      sprintf(logMsg, "Subtraction: %.3f - %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar(); // Catch the newline from input
      getchar(); // Wait for the user to press ENTER key

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

      // Log: Multiplication operation with operands and result
      sprintf(logMsg, "Multiplication: %.3f * %.3f = %.3f", num1, num2, result);
      writeLog(username, logMsg);

      printf("\nResult: %.3f\n", result);
      printf("\nPress ENTER key to return to the main menu...");
      getchar(); // Catch the newline from input
      getchar(); // Wait for the user to press ENTER key

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

      // Guard against division by zero
      if (num2 == 0)
      {
        printf("\n>> ERROR: Division by zero is not allowed!\n");

        // Log: Division by zero attempt
        sprintf(logMsg, "Division: %.3f / %.3f = ERROR (div by zero)", num1, num2);
        writeLog(username, logMsg);
      }
      else
      {
        result = num1 / num2;

        // Log: Division operation with operands and result
        sprintf(logMsg, "Division: %.3f / %.3f = %.3f", num1, num2, result);
        writeLog(username, logMsg);

        printf("\nResult: %.3f\n", result);
      }

      printf("\nPress ENTER key to return to the main menu...");
      getchar(); // Catch the newline from input
      getchar(); // Wait for the user to press ENTER key

      break;

    default:
      printf("Enter a correct number (1-5) and try again.");
      break;
    }
  }
}

// ============================================================
// MAIN FUNCTION
// PURPOSE:  Entry point of the program. Displays the login menu
//           and routes to Admin Panel or Calculator based on
//           the user's login selection.
// ============================================================
int main()
{
  int choice;
  char loggedInUser[MAX_LEN]; // Stores the username of the logged-in user

  for (;;)
  {
    loginUI(); // Print the Login UI
    scanf("%d", &choice);

    switch (choice)
    {
    case 0:
      // Developer Info — display team member names
      header();
      printf("|                            |\n");
      printf("|     Shahriar Hasan Siam    |\n");
      printf("|                            |\n");
      printf("------------------------------\n");

      printf("\n\n");
      printf("Press ENTER to continue...");
      getchar(); // Catch newline from scanf
      getchar(); // Wait for user to press ENTER
      break;

    case 1:
      // Admin Login — validate credentials, then open admin panel
      if (adminLogin())
      {
        adminPanel(); // Enter admin dashboard on successful login
      }
      break;

    case 2:
      // User Login — validate against users.txt, then open calculator
      // loggedInUser gets populated inside userLogin() on success
      if (userLogin(loggedInUser))
      {
        calculatorPanel(loggedInUser); // Pass username so logs can identify the user
      }
      break;

    case 3:
      // Exit the application
      clearUI();
      printf("\n>> Thank you for using the Calculator. Goodbye!\n\n");
      return 0; // Exit the program

    default:
      printf("\n>> Enter a valid option (0-3) and try again.\n");
      printf("\nPress ENTER to continue...");
      getchar();
      getchar();
      break;
    }
  }
}
