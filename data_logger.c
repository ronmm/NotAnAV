#include "data_logger.h"
#define CSV_FILE "/tmp/data.csv"

void log_kernel_data(char * data_from_kernel)
{
	FILE * csv_file = fopen(CSV_FILE, "a");
	if (csv_file == NULL)
	{
		syslog(LOG_ERR, "Cannot open CSV file (%d)", errno);
		return;
	}

	fprintf(csv_file, "%s\n", data_from_kernel);
	fclose(csv_file);
}
