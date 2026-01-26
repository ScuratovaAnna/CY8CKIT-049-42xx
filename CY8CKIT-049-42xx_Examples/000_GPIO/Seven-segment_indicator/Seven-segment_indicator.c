/**------------------------------
 \date  26.01.2026
 *
 *  PSoC Creator 4.4
 *
 *  CY8C4125AXI-483
 *     44-TQFP
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |            |
 *  |        P0.6| ---->  ---    Pin_7Seg[6]
 *  |        P0.5| ----> |   |   Pin_7Seg[5]
 *  |        P0.4| ----> |   |   Pin_7Seg[4]
 *  |        P0.3| ---->  ---    Pin_7Seg[3]
 *  |        P0.2| ----> |   |   Pin_7Seg[2]
 *  |        P0.1| ----> |   |   Pin_7Seg[1]
 *  |        P0.0| ---->  ---    Pin_7Seg[0]
 *  |            |
 *  |            |
 *  |        +5V |
 *  |        GND |
 *
 *  7-сегментный светодиодный модуль
 * https://aliexpress.ru/item/1005004029382373.html?spm=a2g2w.orderdetail.0.0.5a414aa67RYgEw&sku_id=12000033882401395&_ga=2.99520606.1798018584.1769334296-958842653.1759160311
 *
 * Configure Pin: Name -> Pin_7Seg, Pins -> Number of pins: 7 , Type--> Digital output, Drive mode --> Strong drive, Initial drive state --> Low(0)
 *
 *\ authors        ScuratovaAnna, PivnevNikolay
 *\ сode debugging ScuratovaAnna
 */
#include "project.h"

uint8_t CW[10] = { 0x3F, 0X6, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x67 };
__IO uint32_t SYSTEM_MS;

uint32_t millis(void);
void ms_delay(uint32_t delay_ms);
void delayMicroseconds(unsigned int us);

void SysTickISRCallback(void) {
  SYSTEM_MS++;
}

int main(void) {
  CyGlobalIntEnable;
  CySysTickStart();
  CyIntSetSysVector(CY_INT_SYSTICK_IRQN, (cyisraddress)SysTickISRCallback);


  for (;;) {
    for (uint8_t i = 0; i < 10; i++) {
      Pin_7Seg_Write(*(CW + i));
      ms_delay(400);
      //delayMicroseconds(100000);
    }
  }
}

uint32_t millis(void) {
  return SYSTEM_MS;
}

void ms_delay(uint32_t delay_ms) {
  uint32_t end = millis() + delay_ms;
  while (millis() != end);
}

/*Аналог функции delayMicroseconds().
  us: количество микросекунд, на которое приостанавливается выполнение программы. (unsigned int)
  Останавливает выполнение программы на заданное в параметре количество микросекунд (1 000 000 микросекунд в 1 секунде).*/
void delayMicroseconds(unsigned int us) {
  __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks : oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}
