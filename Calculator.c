#include <stdio.h>  // Enables Standard Input and Output in C code
#include <stdlib.h> // Essential Header for using `system()` function

void clearUI()
{
  if (_WIN32)
    system("cls");
  else
    system("clear");
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
  printf("|----------------------------|\n");
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
  printf("|----------------------------|\n");
  printf("|                            |\n");
  printf("|   1. Addition (+)          |\n");
  printf("|   2. Subtraction (-)       |\n");
  printf("|   3. Multiplication (*)    |\n");
  printf("|   4. Division (/)          |\n");
  printf("|   5. Exit                  |\n");
  printf("|                            |\n");
  printf("------------------------------\n");
  printf("\n\n");
  printf("Select an option (1-5): ");
}
int main()
{
  for (;;)
  {
    loginUI(); // Print the Calculator UI
    int choice;
    scanf("%d", &choice);

    if (choice == 0)
    {
      printf("|-----------------------------|\n");
      printf("|                             |\n");
      printf("|     Shahriar Hasan Siam     |\n");
      printf("|                             |\n");
      printf("------------------------------\n");

      printf("\n\n");
      printf("Press any key to continue...");
      scanf("");
    }
    
    for (;;)
    {
      calcUI(); // Print the Calculator UI
      float num1, num2, result;

      scanf("%d", &choice);
      if (choice == 5)
      {
        clearUI();
        break;
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

        result = num1 / num2;

        printf("\nResult: %.3f\n", result);
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
}
