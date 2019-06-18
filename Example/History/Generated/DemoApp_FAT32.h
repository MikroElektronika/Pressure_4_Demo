#include "demo_types.h"
#include "demo_config.h"

void demoInit_FAT32();

void demoMMC_writeData(char *demoText, uint8_t nData);

int demoMMC_openFile();

void demoMMC_closeFile();

void logTime_toFAT32(char *demoTime, char *demoDate);
void logData_toFAT32(float pressure, float temp);