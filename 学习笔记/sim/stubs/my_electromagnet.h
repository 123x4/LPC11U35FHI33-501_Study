/* 仿真替身：振动状态由 sim_main.c 控制 */
#ifndef __my_electromagnet_h__
#define __my_electromagnet_h__
#include <stdint.h>
#include "my_protocol_common.h"
typedef enum { ELE_POS_LEFT, ELE_POS_RIGHT } Ele_Position_te;
#define USE_HALL_VALUE_DURING_VIBRATION   0
#define PUSH_VIBRATING_HALL_VALUE_TO_GUI  0
uint8_t Ele_IsVibrating(Ele_Position_te pos);
void Ele_UpadteSegment(uint8_t virtualKey, uint8_t seg);
#endif
