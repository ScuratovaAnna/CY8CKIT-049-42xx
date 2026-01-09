/**------------------------------
 \date  09.01.2026
 *
 *  PSoC Creator 4.4
 *
 *  CY8C4245AXI-483
 *   ------------
 *  |            |
 *  |            |
 *  |            |
 *  |       P0[3]| ----> Pin_Blue
 *  |       P0[2]| ----> Pin_Green
 *  |       P1[6]| ----> Pin_Red
 *  |            |
 *  |            |
 *  |            |
 *  |        +5V |
 *  |        GND |
 *
 * Configure Pin: Type--> Digital output, Drive mode --> Strong drive, Initial drive state --> Low(0)
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna
 */

#include "project.h"
#define LED_ON 1u
#define LED_OFF 0u
#define pause 400

volatile uint8_t condition = 0;
__IO uint32_t SYSTEM_MS;

uint32_t millis(void);
void ms_delay(uint32_t delay_ms);

void SysTickISRCallback(void)
{
  SYSTEM_MS++;
}

int main(void) {
  CyGlobalIntEnable;
  CySysTickStart();
  CyIntSetSysVector(CY_INT_SYSTICK_IRQN, (cyisraddress)SysTickISRCallback);
  for (;;) {
    switch (condition) {
      case 0:
        Pin_Blue_Write(LED_ON);
        ms_delay(pause);
        //CyDelay(pause);
        Pin_Blue_Write(LED_OFF);
        ms_delay(pause);
        //CyDelay(pause);
        condition++;
        break;
      case 1:
        Pin_Green_Write(LED_ON);
        ms_delay(pause);
        //CyDelay(pause);
        Pin_Green_Write(LED_OFF);
        ms_delay(pause);
        //CyDelay(pause);
        condition++;
        break;
      case 2:
        Pin_Red_Write(LED_ON);
        ms_delay(pause);
        //CyDelay(pause);
        Pin_Red_Write(LED_OFF);
        ms_delay(pause);
        //CyDelay(pause);
        condition = 0;
        break;
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
