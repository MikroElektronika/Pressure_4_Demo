#ifndef _DEMO_PRESSURE_
#define _DEMO_PRESSURE_

/**
 * @brief Functions for logs pressure data on TFT display and USBUART
 *
 * @param[in] newData   New pressure data
 */
void pressure_logData(float newData);

/**
 * @brief Functions for logs temperature data on TFT display and USBUART
 *
 * @param[in] newData   New temperature data
 */
void temperature_logData(float newData);

/**
 * @brief Functions for read Temperature and Pressure data
 *
 * @param[out] demoTemp         Buffer in which temperature data will be storage
 * @param[out] demoPressure     Buffer in which pressure data will be storage
 */
void demoMeasurement(float *demoTemp, float *demoPressure);

/**
 * @brief Initializes Pressure 4 click
 */
void demoInit_Pressure();

/**
 * @brief Functions for report pressure calculate
 *
 * @param[in] pressure        Pressure data that will be taken into account 
                              during the calculation of pressure drop
 */
void pressure_reportCalculation(float pressure);

#endif