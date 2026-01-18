/**------------------------------
 \date  18.01.2026
 *
 *  PSoC Creator 4.4
 *
 *  CY8C4125PVI-482
 *     28-SSOP
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |       P1[0]| ----> LED
 *  |            |
 *  |            |
 *  |       P4[1]| ----> UART Tx
 *  |       P4[0]| ----> UART Rx
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 * Configure Pin: Type--> Digital output, Drive mode --> Strong drive, Initial drive state --> Low(0)
 * Configure UART Basic: Name --> UART_1, mode --> Standart, Direction --> Tx+Rx, Baud rate --> 9600, Data bits --> 8 bits, Parity --> None,
                         Stop bits --> 1 Bit.
 * UART Advanced:  Interrupt --> Internal, Interrupt sources --> RX FIFO not empty.
 *
 *\ authors        ScuratovaAnna, PivnevNikolay
 *\ сode debugging ScuratovaAnna
 */
#include "project.h"
#include <string.h>

#define BUFFER_SIZE 32

// Глобальные переменные для работы с буфером в прерывании 
char rxBuffer[BUFFER_SIZE];
uint8 rxIndex = 0;
uint8 packetReady = 0;
uint8_t RXi;

void clear_RXBuffer(void);

// Прототип функции обработчика прерывания 
CY_ISR(My_UART_ISR)
{
  // Проверяем источник прерывания (RX FIFO не пуст) 
  while (UART_1_SpiUartGetRxBufferSize() > 0)
  {
    char ch = (char)UART_1_UartGetChar();
    // Если пакет еще не обработан основным циклом, принимаем данные 
    if (!packetReady)
    {
      if (ch == 13 || ch == 10)
      {
        if (rxIndex > 0) // Если что-то ввели
        {
          packetReady = 1;          // Флаг готовности строки
        }
      }
      else if (rxIndex < (BUFFER_SIZE - 1))
      {
        rxBuffer[rxIndex++] = ch; // Добавляем символ в буфер
      }
    }
  }

  UART_1_ClearRxInterruptSource(UART_1_INTR_RX_NOT_EMPTY);
}

int main(void)
{
  CyGlobalIntEnable;

  // Настройка прерывания 
  UART_1_SetCustomInterruptHandler(My_UART_ISR);

  UART_1_Start();
  UART_1_UartPutString("UART LED Controller Ready!\r\n");
  UART_1_UartPutString("Commands: Led ON, Led OFF\r\n");

  for (;;)
  {
    if (packetReady)
    {
      UART_1_UartPutString(rxBuffer);
      // Сравниваем полученную строку с командами
      if (strcmp(rxBuffer, "Led ON") == 0)
      {
        Pin_1_Write(1); // Включить
        UART_1_UartPutString("Status: LED is ON\r\n");
      }
      else if (strcmp(rxBuffer, "Led OFF") == 0)
      {
        Pin_1_Write(0); // Выключить
        UART_1_UartPutString("Status: LED is OFF\r\n");
      }
      else
      {
        UART_1_UartPutString("Error: Unknown Command\r\n");
      }
      // Сброс буфера для новой команды 
      clear_RXBuffer();
      rxIndex = 0;
      packetReady = 0;
    }
  }
}

void clear_RXBuffer(void) {
  for (RXi = 0; RXi < BUFFER_SIZE; RXi++) {
    rxBuffer[RXi] = 0u;
  }
}