#include <stdio.h>
#include <stdbool.h>
#include <time.h> // Include the time library for getting the current date

// Function to check if a year is a leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Function to get the number of days in a given month of a year
int getDaysInMonth(int year, int month) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

// Function to calculate the day of the week for the 1st day of a given month and year
int calculateFirstDayOfMonth(int year, int month) {
    int day = 1;
    int y = year - (14 - month) / 12;
    int m = month + 12 * ((14 - month) / 12) - 2;
    int firstDay = (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;
    return firstDay; // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
}

// Function to display the calendar for a given month and year
void displayCalendar(int year, int month) {
    const char *months[] = {"January", "February", "March", "April", "May", "June",
                            "July", "August", "September", "October", "November", "December"};
    int daysInMonth = getDaysInMonth(year, month);
    int startDay = calculateFirstDayOfMonth(year, month);

    printf("\n   %s %d\n", months[month - 1], year);
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");

    // Print initial spaces for the first day of the month
    for (int i = 0; i < startDay; i++) {
        printf("    ");
    }

    // Print all days of the month
    for (int day = 1; day <= daysInMonth; day++) {
        printf("%4d", day);
        if ((startDay + day) % 7 == 0) {
            printf("\n"); // New line after Saturday
        }
    }
    printf("\n");
}

// Main function to display the current month's calendar
int main() {
    // Get current date
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);
    
    int year = currentTime->tm_year + 1900; // tm_year is years since 1900
    int month = currentTime->tm_mon + 1;     // tm_mon is 0-11

    displayCalendar(year, month); // Display the calendar for the current month and year

    return 0; // Exit the program successfully
}
