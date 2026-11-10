/*******************************************************************************
 * @file    my_charger.h
 * @brief   Charger IC BQ21128 (U2) driver. I2C on PIO0_4(SCL) / PIO0_5(SDA).
 * @note    Step 1: read only. Nothing is written to the charger, so it stays in
 *          its default (autonomous) mode and the I2C watchdog does not matter.
*******************************************************************************/
#ifndef __my_charger_h__
#define __my_charger_h__

#include <stdint.h>

#define BQ21128_ADDR                0x6A    /* 7-bit I2C address */

/* register addresses (BQ21128 datasheet 6.6) */
#define BQ21128_REG_CHG_STATUS0     0x1D
#define BQ21128_REG_CHG_STATUS1     0x1E
#define BQ21128_REG_FAULT_STATUS0   0x1F
#define BQ21128_REG_PART_INFO       0x38    /* expect 0x13 */

int32_t Charger_Init(void);
void    Charger_PollingTask(void);
int32_t Charger_Read(uint8_t reg, uint8_t *pData);

#endif /* __my_charger_h__ */
