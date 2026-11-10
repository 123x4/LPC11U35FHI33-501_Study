/*******************************************************************************
 * @file    my_charger.c
 * @brief   Charger IC BQ21128 (U2) driver, step 1: read only.
 *          Init: read PART_INFO (0x38), expect 0x13.
 *          Polling (every 1 s): read status 0x1D / 0x1E / 0x1F and print them.
*******************************************************************************/
#include "my_charger.h"
#include "my_i2c.h"
#include "my_tick.h"
#include "my_debug.h"

int32_t Charger_Read(uint8_t reg, uint8_t *pData)
{
  /* soft_i2c_read_fast returns 0 on success, 1 on NACK */
  if(soft_i2c_read_fast(&handleI2c1, BQ21128_ADDR, reg, pData, 1))
  {
    return -1;
  }
  return 0;
}

int32_t Charger_Init(void)
{
  uint8_t val = 0;

  soft_i2c_init(&handleI2c1);

  if(Charger_Read(BQ21128_REG_PART_INFO, &val))
  {
    InfoLog("read 0x38 fail (no ACK)");
    return -1;
  }
  InfoLog("part info 0x%02X (expect 0x13)", val);
  return (val == 0x13) ? 0 : -2;
}

void Charger_PollingTask(void)
{
  uint8_t st0 = 0, st1 = 0, fault = 0;

  POLLING_INTERVAL_MS(1000);

  if(Charger_Read(BQ21128_REG_CHG_STATUS0, &st0) ||
     Charger_Read(BQ21128_REG_CHG_STATUS1, &st1) ||
     Charger_Read(BQ21128_REG_FAULT_STATUS0, &fault))
  {
    InfoLog("read status fail");
    return;
  }

  /* 0x1E bit4~3 CHG_STAT: 0 not charging/done, 1 CC, 2 CV, 3 top-off
   * 0x1E bit2~0 VBUS_STAT: 0 no input, 4 adapter present
   * 0x1F bit2~0 TS_STAT: 0 normal 1 cold 2 hot 3 cool 4 warm 5 precool 6 prewarm
   * 0x1D bit0   WD_STAT: 1 = default mode (watchdog expired) */
  InfoLog("1D=%02X 1E=%02X 1F=%02X | CHG=%u VBUS=%u TS=%u WD=%u",
          st0, st1, fault,
          (st1 >> 3) & 0x03, st1 & 0x07, fault & 0x07, st0 & 0x01);
}
