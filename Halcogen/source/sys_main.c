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

FATFS Fatfs;
FIL fil;

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/
#pragma SET_DATA_SECTION("SD_RDATA_SECTION")
int SD_Test(void);
/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{

UINT bw;
FRESULT fr;


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

      mmcSelectSpi(spiPORT1, spiREG1);  // SD card is on the SPI1

   //   SD_Test();

      f_mount(&Fatfs, "", 3);     /* Give a work area to the default drive */

      fr = f_open(&fil, "xyz.txt", FA_WRITE | FA_CREATE_ALWAYS);  /* Create a file */

      if (fr == FR_OK) {
            f_write(&fil, "It works!\r\n", 11, &bw);    /* Write data to the file */
            fr = f_close(&fil);                         /* Close the file */

            }

      while(1);

}



/* USER CODE BEGIN (4) */
/* USER CODE END */
