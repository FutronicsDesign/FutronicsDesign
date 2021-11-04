/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */
#include "sys_common.h"
#include "system.h"
#include <math.h>
#include <stdbool.h>
#include "sci.h"
#include "spi.h"
#include "rti.h"
#include "uartstdio.h"
#include "sdc-hercules.h"
/* USER CODE BEGIN (1) */
#include <string.h>
#include <stdio.h>
#include "ff.h"
/* USER CODE END */
#include "diskio.h"
FATFS Fatfs;
FIL fil;
FRESULT res;


/////*code for set RTC timer //////

FRESULT set_timestamp (
    char *obj,     /* Pointer to the file name */
    int year,
    int month,
    int mday,
    int hour,
    int min,
    int sec
)
{
    FILINFO fno;

    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);

    return f_utime(obj, &fno);
}



#if _USE_LFN
static char lfn[_MAX_LFN + 1];
#endif


#if _USE_LFN
fno.lfname = lfn;
fno.lfsize = sizeof(lfn);
#endif
/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/
//void showdata4(struct cell c,float vPack,float iPack,float zAvg,float TempPack, float PCap);
//void showdata4(struct cell c[],float vPack,float iPack,float zAvg,float TempPack, float PCap);
//*Number of cells connected in Series & Parallel and Temp. Sensors
#define noSCell 8
#define noPCell 7
#define noTemp 8

struct cell{
    float voltage;
    float current;
    float z;
    float temp;
    float Capacity;
    float timestamp;
};
struct cell c[8];

float zAvg= 1.0 ;
float vPack=2.0;
float iPack=3.0;
float iref;
float TempPack=4.0;
float PCap;
float zMin;
int i,m;

#pragma SET_DATA_SECTION("SD_RDATA_SECTION")
int SD_Test(void);
/* USER CODE BEGIN (2) */
/* USER CODE END */

/****** function to change the timestamp ***********************/



void showdata4(struct cell c[],float vPack, float iPack, float zAvg, float TempPack, float PCap){
                      int i;

                  FRESULT timer = set_timestamp("", 2018, 9, 15, 1, 1, 1);


             //     printf("%f\t%f\t%f\t%f\t%f\t",zAvg*100,vPack,iPack,TempPack,PCap);
                   f_printf(&fil,"%f\t%f\t%f\t%f\t%f\t",(int)timer,zAvg*100,vPack,iPack,TempPack);
                //   printf("\t");
                  for(i = 0; i < noSCell; i++){
                       f_printf(&fil,"%f\t",c[i].voltage);// c is structure voltage data
                       }
                //   printf("\t\t");
                   for(i = 0; i < noSCell; i++){
                     f_printf(&fil,"%f\t",c[i].z*100);
                     }
                //   printf("\t\t");
                    for(i = 0; i < noTemp; i++){
                        f_printf(&fil,"%f\t ",c[i].temp);
                   }
              }


int main(void)
{

UINT bw;
FRESULT fr;

char filename[16]; // enough space for characters and terminating NUL


      sciInit(); // To Initalize LIN/SCI2 routines to receive commands and transmit data
      //  spiInit(); // Use it  in Fatfs port

      UARTprintf("Futronics Limited");

       rtiInit();
      /* Enable RTI Compare 3 interrupt notification */
      rtiEnableNotification(rtiNOTIFICATION_COMPARE3);
      /* Enable IRQ - Clear I flag in CPS register */


      _enable_IRQ();
      /* Start RTI Counter Block 1 */
      rtiStartCounter(rtiCOUNTER_BLOCK1);

   //   _cacheDisable_();

   //   mmcSelectSpi(spiPORT1, spiREG1);  // SD card is on the SPI1
        mmcSelectSpi(spiPORT2, spiREG2);  // SD card is on the SPI2
   //   SD_Test();

      f_mount(&Fatfs, "", 3);     /* Give a work area to the default drive */


 //     sprintf(fr, "xyz_%d.txt", GetNextIndex("") );

      fr = f_open(&fil,"xyz_%d.txt", FA_WRITE | FA_CREATE_ALWAYS);  /* Create a file */
      fr = open_append(&fil, "xyz_%d.txt");

//      sprintf(fr, "%03d.txt", GetNextIndex("""") );
      if (fr != FR_OK) {
        //    f_write(&fil, "It works!\r\n", 11, &bw);    /* Write data to the file */
        //   fr = f_close(&fil);                         /* Close the file */
              while(1);
            }
      if (! f_size(&fil)) {
                int i;
                fr = f_printf(&fil, "BMS INITIALIZ...........\n");
                fr = f_printf(&fil, "Battery Initial Status:\n");
                fr = f_printf(&fil, "Battery Pack Status\t\t\t\t\t\t\t\tSeries cell's Voltages\t\t\t\t\t\t\t\t\t\t\t\t\t\tSeries cell's SOC\t\t\t\t\t\t\t\t\t\t\t\t\t\tTemperature Sensors data\t\t\t\t\t\t\t\t\t\t\t Balancing Cell ID\n");
                fr = f_printf(&fil,"SoC\t\tTime\t\tVoltage\t\tCurrent\t\tTemperature\t\t");

                for(i = 0; i < noSCell; i++){
                    f_printf(&fil,"%d\t\t",i+1);  //
                     }

               //   printf("Cell_SOC:\t");
                  for(i = 0; i < noSCell; i++){
                      f_printf(&fil,"%d\t\t",i+1);
                     }

               //   printf("Temp_Sensors:\t");
                  for(i = 0; i < noTemp; i++){
                      f_printf(&fil,"%d\t\t",i+1);
                     }

                  for(i = 0; i < noSCell; i++){
                      f_printf(&fil,"%d\t",i+1);


            //    f_printf("BMS INITIALIZ...........\n");
            //    f_printf("Battery Initial Status:\n");
            //    f_printf("Battery Pack Status\t\t\t\t\t\t\t\tSeries cell's Voltages\t\t\t\t\t\t\t\t\t\t\t\t\t\tSeries cell's SOC\t\t\t\t\t\t\t\t\t\t\t\t\t\tTemperature Sensors data\t\t\t\t\t\t\t\t\t\t\tBalancing Cell ID\n");
                  }
              if (fr <0) {
                  /* Error. Cannot write header */
                  while(1);
              }
            }

      while(1){

      //     fr = f_printf(&fil, "%08u;%08u;%08u\n", 1, 2, 3);
          fr = open_append(&fil, "xyz_%d.txt");
           showdata4(c,vPack,iPack,zAvg,TempPack, PCap);

         }
           if (fr < FR_OK) {
               /* Error. Cannot log data */
               while(1);
           }

           /* Close the file */
                fr = f_close(&fil);
                if (fr != FR_OK)
                {
                  /* Error. Cannot close the file */
                  while(1);
                }




     // while(1);

}

/*****
int GetNextIndex(char *path)
{
DIR dir;
FILINFO fno;
int i, index = -1;
if (f_opendir(&dir, path) == FR_OK)
{
while(1)
{
if ((f_readdir(&dir, &fno) != FR_OK) || (fno.fname[0] == 0))
break;
if ((strstr(fno.fname, """.txt") != NULL) && (sscanf(fno.fname, "%d", &i) == 1))
if (i > index) index = i;
}
}
return(index + 1);
}

****?
//void loop() {
  // save new integer every loop and then wait 1s
//    fr = f_open(&fil, "xyz.txt", FA_WRITE | FA_CREATE_ALWAYS);
//    if (fr) {
    // save a different number each loop
//        f_printf(String(millis())+","+String(int_iter));
//    testfile.close();
//    Serial.println("Saving "+String(int_iter));
//  } else {
//    Serial.println("error opening file");
//  }
//  int_iter+=1;
//  delay(1000);
//}

/* USER CODE BEGIN (4) */
/* USER CODE END */
