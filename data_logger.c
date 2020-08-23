#include "data_logger.h"
#define CSV_FILE "/tmp/data.csv"

void get_timestamp(char * timestamp)
{
	time_t current_time = time(NULL);
	sprintf(timestamp, "\"time=%s\"", ctime(&current_time));
	
	// Remove newline from end of tiemstamp
	timestamp[strlen(timestamp)-2] = '"';
	timestamp[strlen(timestamp)-1] = '\0';
}

void log_kernel_data(char * data_from_kernel)
{
	FILE * csv_file = fopen(CSV_FILE, "a");
	if (csv_file == NULL)
	{
		syslog(LOG_ERR, "Cannot open CSV file (%d)", errno);
		return;
	}

	char timestamp[64] = {0};
	get_timestamp(timestamp);
        printf("data from kernel: '%s'\n", data_from_kernel);

	fprintf(csv_file, "%s,%s\n", data_from_kernel, timestamp);
	fclose(csv_file);
}
