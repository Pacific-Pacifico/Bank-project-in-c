#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *get_date()
{
    // time_t is arithmetic time type
	time_t now;

	// Obtain current time
	// time() returns the current time of the system as a time_t value
	time(&now);
    return ctime(&now);
}

// Print current date and time in C
int get_month()
{
	int month;
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	month = local->tm_mon + 1;   	// get month of year (0 to 11)
	// print current date
//	printf("Date is : %d %d\n",  month, year);
	return month;
}

int get_year()
{
	int year;
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
    year = local->tm_year + 1900;	// get year since 1900
	// print current date
//	printf("Date is : %d %d\n",  month, year);
	return year;
}

//int main()
//{
//    printf("Date is : %d %d\n",  get_month(), get_year());
//}
