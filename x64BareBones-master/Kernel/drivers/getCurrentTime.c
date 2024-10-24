#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
void getCurrentTime(){
    time_t t = time(NULL);
        if (t == (time_t)(-1)) {
            printf("There was an error\n");
            return -1;
        }
    struct tm date = *localtime(&t);
    printf("Current date is %d-%02d-%02d %02d:%02d\n", date.tm_year + 1900, date.tm_mon+ 1, date.tm_mday, date.tm_hour, date.tm_min);
    // printf("Timezone is: %s\n", date.tm_zone);


}
*/
char * getCurrentTime(){
    int day, month, year;

    // time_t is arithmetic time type
    time_t now;

    // Obtain current time
    time(&now);

    // Convert to local time format
    struct tm *local = localtime(&now);

    day = local->tm_mday;        // get day of month (1 to 31)
    month = local->tm_mon + 1;   // get month of year (0 to 11)
    year = local->tm_year + 1900; // get year since 1900

    // Allocate memory for the date string
    static char date[20];

    // Format the date as "dd/mm/yyyy"
    sprintf(date, "%02d/%02d/%04d", day, month, year);

    // Return the formatted date
    return date;
}
/* --------- Testeo --------
// Print current date in C
int main()
{
    printf("Welcome Steve\n");
    char *local = getDay();
    printf("Today's date is: %s\n", local);

    return 0;
}
*/