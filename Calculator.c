#include <stdio.h>   // Enables Standard Input and Output in C code
#include <stdlib.h>  // Essential Header for using `system()` function
#include <string.h>  // Required for string operations (strcmp, strcpy, etc.)

// ===== File path for storing user credentials =====
// Users are stored as "username password" per line in this file.
// The admin account is hardcoded and NOT stored in this file.
#define USER_FILE "users.txt"

// ===== Maximum length for username and password strings =====
#define MAX_LEN 50

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
  printf("|   4. Log Out               |\n");
  printf("|                            |\n");
  printf("------------------------------\n");
  printf("\n\n");
  printf("Select an option (1-4): ");
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
  scanf("%s", password);

  // Compare entered credentials with hardcoded admin credentials
  if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0)
  {
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
// RETURNS:  1 if login is successful, 0 otherwise.
// ============================================================
int userLogin()
{
  char username[MAX_LEN], password[MAX_LEN];
  char fileUser[MAX_LEN], filePass[MAX_LEN];
  int found = 0; // Flag to track if credentials matched

  header();
  printf("\n--- USER LOGIN ---\n\n");

  printf("Enter Username: ");
  scanf("%s", username);
  printf("Enter Password: ");
  scanf("%s", password);

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
  scanf("%s", password);

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
// FUNCTION: adminPanel
// PURPOSE:  The admin dashboard loop. Admin can create users,
//           delete users, view all users, or log out.
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
      clearUI();
      return; // Log out — return to main login menu

    default:
      printf("\n>> Enter a valid option (1-4) and try again.\n");
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
// ============================================================
void calculatorPanel()
{
  int choice;

  for (;;)
  {
    calcUI(); // Display the calculator menu
    float num1, num2, result;

    scanf("%d", &choice);
    if (choice == 5)
    {
      clearUI();
      break; // Log out — return to main login menu
    }

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
      }
      else
      {
        result = num1 / num2;
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
      printf("|     Tawsif Rahman          |\n");
      printf("|     Mohammad Tanim         |\n");
      printf("|     MD Sabbir Hossain      |\n");
      printf("|     Eastiak Remon          |\n");
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
      if (userLogin())
      {
        calculatorPanel(); // Enter calculator on successful login
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
