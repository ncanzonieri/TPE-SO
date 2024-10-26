#include <commands.h>
#include <time.h>


void Actualtime(){
    char * localDate = getDay();
    printf("Today's date is: %s\n", localDate);
}