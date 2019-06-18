#ifndef _DEMO_PRESSURE_
#define _DEMO_PRESSURE_

void pressure_logData(float newData);
void temperature_logData(float newData);
void demoMeasurement(float *demoTemp, float *demoPressure);
void demoInit_Pressure();
void pressure_reportCalculation(float pressure);

#endif