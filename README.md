# STM32F769 NFC Application

Hardware used
* Development board STM32F769-DISCO
* X-NUCLEO-NFC05A1*

*In case you don't have the Nucleo board, the project will still run.

Additional Software: 
* LittlevGL
* FreeRTOS
* Rfal
* LwIP

# Known issues
* After getting back to constant polling mode(after 2 times pressing the user button), the NFC reader stops working and applcation stops responding.
* High network delay(ping ~1s)
