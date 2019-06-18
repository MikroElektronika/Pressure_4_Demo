
/**
 * @brief Initializes RTC 6 click
 */
void demoInit_RTC();

/**
 * @brief Functions for read RTC GMT time and data
 *
 * @param[out] demoTime         Buffer in which Time will be storage
 * @param[out] demoPressure     Buffer in which Date will be storage
 */
void rtc_readTime(char *demoTime, char *demoDate);

/**
 * @brief Functions for logs Time and Date on TFT display and USBUART
 */
void rtc_logTime();

/**
 * @brief Check if the alarm is activated
 *
 * @retval 0 - if not active alarm or 1 - if active alarm
 */
uint8_t rtc_checkAlarm();