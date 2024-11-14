#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "GUI.h"
#include <stdbool.h>
#include <time.h>
char currentUsername[20];

// Define a structure for time with hours and minutes
typedef struct
{
    int time_HH;
    int time_MM;
} Time;

// Define a structure for schedule with day, time, subject, and lecturer
typedef struct
{
    int day;           // Day of the week (1-5 for Monday to Friday)
    Time startTime;    // Start time
    Time endTime;      // End time
    char subject[30];  // Subject name
    char lecturer[30]; // Lecturer name
} Schedule;

// Clear screen based on the operating system
void clearScreen()
{

    system("clear");
}

// Function to check if a year is a leap year
bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Function to get the number of days in a given month of a year
int getDaysInMonth(int year, int month)
{
    if (month == 2)
    {
        return isLeapYear(year) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else
    {
        return 31;
    }
}

// Function to calculate the day of the week for the 1st day of a given month and year
int calculateFirstDayOfMonth(int year, int month)
{
    int day = 1;
    int y = year - (14 - month) / 12;
    int m = month + 12 * ((14 - month) / 12) - 2;
    int firstDay = (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;
    return firstDay; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
}

// Function to display the calendar for a given month and year
void displayCalendar(int year, int month)
{
    const char *months[] = {"January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December"};
    int daysInMonth = getDaysInMonth(year, month);
    int startDay = calculateFirstDayOfMonth(year, month);

    // Get current date to highlight it
    time_t t = time(NULL);
    struct tm *currentDate = localtime(&t);
    int currentDay = currentDate->tm_mday;
    int currentMonth = currentDate->tm_mon + 1;
    int currentYear = currentDate->tm_year + 1900;

    printf("\n   %s %d\n", months[month - 1], year);
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");

    // Print initial spaces for the first day of the month
    for (int i = 0; i < startDay; i++)
    {
        printf("    ");
    }

    // Print all days of the month
    for (int day = 1; day <= daysInMonth; day++)
    {
        // Check if the day is the current date and highlight it
        if (day == currentDay && month == currentMonth && year == currentYear)
        {
            printf(" [%2d]", day); // Highlight current day with brackets
        }
        else
        {
            printf("%4d", day);
        }

        if ((startDay + day) % 7 == 0)
        {
            printf("\n"); // New line after Saturday
        }
    }
    printf("\n");
}

// Select a day (1-5) with validation
int selectDay()
{
    int day;
    while (1)
    {
        printf("1. Monday\n2. Tuesday\n3. Wednesday\n4. Thursday\n5. Friday\nEnter the day (1-5): ");
        scanf("%d", &day);
        if (day >= 1 && day <= 5)
        {
            return day;
        }
        printf("Invalid day! Please try again.\n");
    }
}

// Prompt user for time input (hours and minutes) with validationvoid
void promptTime(Time *time, const char *timeLabel)
{
    while (1)
    {
        printf("Enter %s hour (0-23): ", timeLabel);
        scanf("%d", &time->time_HH);
        if (time->time_HH < 0 || time->time_HH > 23)
        {
            printf("Invalid hour! Please enter an hour between 0 and 23.\n");
            continue;
        }

        printf("Enter %s minutes (0-59): ", timeLabel);
        scanf("%d", &time->time_MM);
        if (time->time_MM < 0 || time->time_MM >= 60)
        {
            printf("Invalid minutes! Please enter minutes between 0 and 59.\n");
            continue;
        }
        break; // Valid time input; exit the loop
    }
}

// Function to create a new account// Function to create a new account with duplicate username check
void createAccount()
{
    char username[20], password[20];

    while (1)
    { // Start an infinite loop
        printf("Enter a new username: ");
        scanf("%19s", username);

        // Open the file in read mode to check for existing usernames
        FILE *fp = fopen("account.txt", "r");
        if (!fp)
        {
            perror("Error opening account file");
            return;
        }

        char file_username[20], file_password[20];
        // Check for duplicate usernames
        int usernameExists = 0; // Flag to track if username exists
        while (fscanf(fp, "%19s %19s", file_username, file_password) == 2)
        {
            if (strcmp(username, file_username) == 0)
            {
                printf("Error: Username already exists. Please try a different username.\n");
                usernameExists = 1; // Set flag if username exists
                break;              // Exit the loop if a duplicate is found
            }
        }
        fclose(fp);

        if (!usernameExists)
        {          // If no duplicate was found
            break; // Exit the loop to proceed with password input
        }
    }

    // Prompt for password
    printf("Enter a new password: ");
    inputPassword(password);

    // Open the file in append mode and add the new account
    FILE *fp = fopen("account.txt", "a");
    if (!fp)
    {
        perror("\nError opening account file");
        return;
    }

    fprintf(fp, "%s %s\n", username, password);
    printf("\nAccount created successfully!\n");

    fclose(fp);
}
int login()
{
    char username[20], password[20];
    int authenticated = 0;

    while (1)
    {
        printf("Enter username: ");
        scanf("%19s", username);
        printf("Enter password: ");
        inputPassword(password);

        FILE *fp = fopen("account.txt", "r");
        if (!fp)
        {
            perror("\nError opening account file");
            return 0; // Return 0 if there's an error opening the file
        }

        char file_username[20], file_password[20];
        while (fscanf(fp, "%19s %19s", file_username, file_password) == 2)
        {
            if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0)
            {
                authenticated = 1;
                strcpy(currentUsername, username); // Store the logged-in username
                break;
            }
        }
        fclose(fp);

        if (authenticated)
        {
            printf("\nLogin successful!\n");
            return 1; // Return 1 if login is successful
        }
        else
        {
            printf("\nInvalid username or password. Please try again.\n");
        }
    }
}

// Main function to display login or sign up options
void welcome()
{
    int choice;

    // Display options to the user
    printf("Welcome to the Schedule Management System!!! \nPlease select an option:\n");
    printf("1. Create Account\n");
    printf("2. Login\n");
    printf("Enter your choice (1 or 2): ");

    scanf("%d", &choice);
    system("cls");
    if (choice == 1)
    {
        createAccount();
    }
    else if (choice == 2)
    {
        while (!login())
        {
            printf("Login failed. Would you like to try again? (y/n): ");
            char retry;
            scanf(" %c", &retry);
            if (retry != 'y' && retry != 'Y')
            {
                printf("Exiting program...\n");
                exit(0);
            }
        }
    }
    else
    {
        printf("Invalid choice. Exiting program...\n");
        exit(1);
    }
}
void addSchedule()
{
    Schedule schedule;
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername); // Create a filename for the user

    FILE *fp = fopen(filename, "a+"); // Open in append mode but also allow reading for checking duplicates
    if (!fp)
    {
        perror("Error opening file");
        return;
    }

    schedule.day = selectDay();
    printf("Enter subject: ");
    scanf("%29s", schedule.subject);
    printf("Enter lecturer: ");
    scanf("%29s", schedule.lecturer);
    printf("Enter start time:\n");
    promptTime(&schedule.startTime, "start");
    printf("Enter end time:\n");
    promptTime(&schedule.endTime, "end");

    // Check for duplicates before adding the new schedule
    Schedule existingSchedule;
    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &existingSchedule.day,
                  &existingSchedule.startTime.time_HH,
                  &existingSchedule.startTime.time_MM,
                  &existingSchedule.endTime.time_HH,
                  &existingSchedule.endTime.time_MM,
                  existingSchedule.subject,
                  existingSchedule.lecturer) == 7)
    {
        if (existingSchedule.day == schedule.day &&
            strcmp(existingSchedule.subject, schedule.subject) == 0 &&
            existingSchedule.startTime.time_HH == schedule.startTime.time_HH &&
            existingSchedule.startTime.time_MM == schedule.startTime.time_MM)
        {
            printf("Error: A schedule with the same day, subject, and start time already exists.\n");
            fclose(fp);
            return; // Exit the function without adding the schedule
        }
    }

    // Write the new schedule if no duplicates were found
    fprintf(fp, "%d %d %d %d %d %s %s\n", schedule.day,
            schedule.startTime.time_HH,
            schedule.startTime.time_MM,
            schedule.endTime.time_HH,
            schedule.endTime.time_MM,
            schedule.subject,
            schedule.lecturer);
    printf("Schedule added successfully!\n");

    fclose(fp);
}
void displaySchedule()
{
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file");
        return;
    }

    Schedule schedule;
    const char *dayNames[] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

    printf("\nSchedules:\n");
    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &schedule.day,
                  &schedule.startTime.time_HH,
                  &schedule.startTime.time_MM,
                  &schedule.endTime.time_HH,
                  &schedule.endTime.time_MM,
                  schedule.subject,
                  schedule.lecturer) == 7)
    {
        printf("Day: %s | Time: %02d:%02d to %02d:%02d | Subject: %s | Lecturer: %s\n",
               dayNames[schedule.day],
               schedule.startTime.time_HH,
               schedule.startTime.time_MM,
               schedule.endTime.time_HH,
               schedule.endTime.time_MM,
               schedule.subject,
               schedule.lecturer);
    }
    fclose(fp);
}
void deleteSchedule()
{
    int day, startHour, startMin;
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername); // Use the user-specific filename

    printf("Enter the day to delete (1 for Monday, 2 for Tuesday, ...): ");
    scanf("%d", &day);
    printf("Enter the start time of the schedule to delete (HH MM): ");
    scanf("%d %d", &startHour, &startMin);

    FILE *fp = fopen(filename, "r");
    FILE *tempFile = fopen("temp_schedule.txt", "w"); // Temporary file for storing data after deletion
    if (!fp || !tempFile)
    {
        perror("Error opening file");
        return;
    }

    Schedule schedule;
    int found = 0;
    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &schedule.day,
                  &schedule.startTime.time_HH,
                  &schedule.startTime.time_MM,
                  &schedule.endTime.time_HH,
                  &schedule.endTime.time_MM,
                  schedule.subject,
                  schedule.lecturer) == 7)
    {
        if (schedule.day == day &&
            schedule.startTime.time_HH == startHour &&
            schedule.startTime.time_MM == startMin)
        {
            found = 1; // Found the schedule to delete
        }
        else
        {
            fprintf(tempFile, "%d %d %d %d %d %s %s\n", schedule.day,
                    schedule.startTime.time_HH,
                    schedule.startTime.time_MM,
                    schedule.endTime.time_HH,
                    schedule.endTime.time_MM,
                    schedule.subject,
                    schedule.lecturer);
        }
    }
    fclose(fp);
    fclose(tempFile);

    if (found)
    {
        remove(filename);                      // Delete the original file
        rename("temp_schedule.txt", filename); // Rename temp file as the new schedule file
        printf("Schedule deleted successfully.\n");
    }
    else
    {
        printf("No matching schedule found.\n");
        remove("temp_schedule.txt"); // Clean up temp file if no schedule was deleted
    }
}

void searchScheduleBySubject()
{
    char searchSubject[30];
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername); // Use the user-specific filename

    printf("Enter the subject to search for: ");
    scanf("%29s", searchSubject);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file");
        return;
    }

    Schedule schedule;
    const char *dayNames[] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    int found = 0;

    printf("\nSchedules for subject '%s':\n", searchSubject);
    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &schedule.day,
                  &schedule.startTime.time_HH,
                  &schedule.startTime.time_MM,
                  &schedule.endTime.time_HH,
                  &schedule.endTime.time_MM,
                  schedule.subject,
                  schedule.lecturer) == 7)
    {
        if (strcmp(schedule.subject, searchSubject) == 0)
        {
            printf("Day: %s | Time: %02d:%02d to %02d:%02d | Lecturer: %s\n",
                   dayNames[schedule.day],
                   schedule.startTime.time_HH,
                   schedule.startTime.time_MM,
                   schedule.endTime.time_HH,
                   schedule.endTime.time_MM,
                   schedule.lecturer);
            found = 1;
        }
    }
    if (!found)
    {
        printf("No schedules found for the subject '%s'.\n", searchSubject);
    }
    fclose(fp);
}
void searchScheduleByLecturer()
{
    char searchLecturer[30];
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername); // Use the user-specific filename

    printf("Enter the lecturer's name to search for: ");
    scanf("%29s", searchLecturer);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Error opening file");
        return;
    }

    Schedule schedule;
    const char *dayNames[] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    int found = 0;

    printf("\nSchedules for lecturer '%s':\n", searchLecturer);
    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &schedule.day,
                  &schedule.startTime.time_HH,
                  &schedule.startTime.time_MM,
                  &schedule.endTime.time_HH,
                  &schedule.endTime.time_MM,
                  schedule.subject,
                  schedule.lecturer) == 7)
    {
        if (strcmp(schedule.lecturer, searchLecturer) == 0)
        {
            printf("Day: %s | Subject: %s | Time: %02d:%02d to %02d:%02d\n",
                   dayNames[schedule.day],
                   schedule.subject,
                   schedule.startTime.time_HH,
                   schedule.startTime.time_MM,
                   schedule.endTime.time_HH,
                   schedule.endTime.time_MM);
            found = 1;
        }
    }
    if (!found)
    {
        printf("No schedules found for the lecturer '%s'.\n", searchLecturer);
    }
    fclose(fp);
}

void updateSchedule()
{
    int day, startHour, startMin;
    char filename[40];
    sprintf(filename, "%s_schedule.txt", currentUsername); // Use the user-specific filename

    printf("Enter the day of the schedule to update (1 for Monday, 2 for Tuesday, ...): ");
    scanf("%d", &day);
    printf("Enter the start time of the schedule to update (HH MM): ");
    scanf("%d %d", &startHour, &startMin);

    FILE *fp = fopen(filename, "r");
    FILE *tempFile = fopen("temp_schedule.txt", "w");
    if (!fp || !tempFile)
    {
        perror("Error opening file");
        return;
    }

    Schedule schedule;
    int found = 0;

    while (fscanf(fp, "%d %d %d %d %d %29s %29s",
                  &schedule.day,
                  &schedule.startTime.time_HH,
                  &schedule.startTime.time_MM,
                  &schedule.endTime.time_HH,
                  &schedule.endTime.time_MM,
                  schedule.subject,
                  schedule.lecturer) == 7)
    {
        if (schedule.day == day &&
            schedule.startTime.time_HH == startHour &&
            schedule.startTime.time_MM == startMin)
        {
            printf("Enter new subject: ");
            scanf("%29s", schedule.subject);
            printf("Enter new lecturer: ");
            scanf("%29s", schedule.lecturer);
            printf("Enter new start time (HH MM): ");
            scanf("%d %d", &schedule.startTime.time_HH, &schedule.startTime.time_MM);
            printf("Enter new end time (HH MM): ");
            scanf("%d %d", &schedule.endTime.time_HH, &schedule.endTime.time_MM);
            found = 1;
        }
        fprintf(tempFile, "%d %d %d %d %d %s %s\n", schedule.day,
                schedule.startTime.time_HH,
                schedule.startTime.time_MM,
                schedule.endTime.time_HH,
                schedule.endTime.time_MM,
                schedule.subject,
                schedule.lecturer);
    }
    fclose(fp);
    fclose(tempFile);

    if (found)
    {
        remove(filename);                      // Delete the original file
        rename("temp_schedule.txt", filename); // Rename temp file as the new schedule file
        printf("Schedule updated successfully.\n");
    }
    else
    {
        printf("No matching schedule found.\n");
        remove("temp_schedule.txt"); // Clean up temp file if no schedule was updated
    }
}

// Masked password input function
void inputPassword(char *password)
{
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r')
    { // '\r' is carriage return
        if (ch == '\b' && i > 0)
        { // Handle backspace
            printf("\b \b");
            i--;
        }
        else if (ch != '\b')
        {
            password[i++] = ch;
            printf("*"); // Mask with '*'
        }
    }
    password[i] = '\0'; // Null-terminate the password
}
// Display the main menu and handle user choices
void menu()
{
    int choice;
    do
    {
        printf("\nMenu:\n");
        printf("1. Add Schedule\n");
        printf("2. Display Schedule\n");
        printf("3. Delete Schedule\n");
        printf("4. Search Schedule by Subject\n");
        printf("5. Search Schedule by Lecturer\n");
        printf("6. Edit the Schedule\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addSchedule();
            break;
        case 2:
            displaySchedule();
            break;
        case 3:
            deleteSchedule();
            break;
        case 4:
            searchScheduleBySubject();
            break;
        case 5:
            searchScheduleByLecturer();
            break;
        case 6:
            updateSchedule();
            break;
        case 7:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}

// Main function to execute the program
int main()
{
    
    system("cls");
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);

    int year = currentTime->tm_year + 1900; // tm_year is years since 1900
    int month = currentTime->tm_mon + 1;    // tm_mon is 0-11

    displayCalendar(year, month); // Display the calendar for the current month and year
    welcome();
    menu();
    return 0;
}
