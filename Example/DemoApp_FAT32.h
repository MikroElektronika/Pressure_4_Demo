#include "demo_types.h"
#include "demo_config.h"

/**
 * @brief Initializes FAT32 - MMC module
 */
void demoInit_FAT32();

/**
 * @brief Function for opening the log file
 */
int demoMMC_openFile();

/**
 * @brief Function for close file
 */
void demoMMC_closeFile();

/**
 * @brief Functions for logs Time to FAT32
 *
 * @param[in] demoTime   Time to be logged on to the memory card
 * @param[in] demoDate   Date to be logged on to the memory card
 */
void logTime_toFAT32(char *demoTime, char *demoDate);

/**
 * @brief Functions for logs Pressure data and Temperature data to FAT32
 *
 * @param[in] pressure   Pressure data which be logged on to the memory card
 * @param[in] temp       Temperature data which be logged on to the memory card
 */
void logData_toFAT32(float pressure, float temp);