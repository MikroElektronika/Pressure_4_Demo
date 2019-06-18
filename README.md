![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# Pressure 4 click demo application

---

- **Verison**     : 1.0.0
- **Date**        : Jun 2018.
- **Libstock**    : https://libstock.mikroe.com/projects/view/2462/remote-pressure-and-temperature-station

---

#### Description

**Hardware**

- Mikromedia Plus for STM32F7
  + MikroBUS 1 - Pressure 4 click
  + MikroBUS 2 - RTC 6 click
  + MikroBus 3 - GSM 2 click
  + MikroBus 4 - Button R click
  + Memory card formatted on FAT32 

**Packages**

- [Pressure 4 Click](https://libstock.mikroe.com/projects/view/2380/pressure-4-click)
- [RTC 6 Click](https://libstock.mikroe.com/projects/view/2459/rtc-6-click)
- [GSM 2 Click](https://libstock.mikroe.com/projects/view/535/gsm-2-click)
- [FAT32 library](https://libstock.mikroe.com/projects/view/108/fat32-library)
  + New mikromedia boardDef

**Example**

Pressure 4 click measures pressure and temperature and outputs the measurements on USB-UART and display.
RTC6 calculates current time, sets the alarm, and then checks if the alarm is activated.
If the alarm has been activated, the last measured data is taken into a calculation of the pressure drop.

Date and time data along with temperature and pressure data get logged on the memory card.
In order to stop logging, it is necessary to press the button on Button R click.
It is also necessary to stop the logging before taking out the memory card.
If the pressure has been dropping for the past hour - which is calculated using this formula: 
(x(seconds) * 21(measurement)), GSM 2 click sends an SMS message which says that the pressure has dropped in the past hour.

The user can monitor the changes in temperature and pressure on graphs which are updated on the display.




 



