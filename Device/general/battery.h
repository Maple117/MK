#ifndef _battery_H_
#define _battery_H_

void BatteryCheckClose(void);
void BatteryCheckOpen(void);
void PVDRespond(void);
void LowVoltageConfirmation(void);

void PWR_Init(void);
void PWR_Init1(void);
void PWR_Init2(void);
void PWR_Init3(void);
void PWRDeInit(void);

#endif

