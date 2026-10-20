/*******************************************************************************
 * 按键算法 PC 仿真（第 1 课实验）
 *
 * 直接编译仓库里的 src/my_misc.c，用一条模拟的“左键按压曲线”喂给
 * ProcessRecvSampleRawData()，每 1ms 调用一次，和固件里一样。
 * 打印 ADC 原始值、滤波后的值、上/下基线、段数、按键状态。
 *
 * 用法: ./key_sim [AP] [RT按下] [RT松开] [是否模拟振动冻结 0/1]
 *       默认       10    30       20        1
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "my_misc.h"
#include "my_electromagnet.h"
#include "my_protocol_mouse.h"

/* 定义在 src/my_misc.c，头文件里没有导出 */
extern uint16_t gwTopBaseline[ANA_TOTAL_COUNT];
extern uint16_t gwBottomBaseline[ANA_TOTAL_COUNT];

/*---------------- 替身函数：固件里由其他模块提供 ----------------*/
static uint32_t gTickMs = 0;
uint32_t MyTick_GetTickMs(void) { return gTickMs; }

const unsigned char bit_error[256] = {0}; /* IRPT 板不用 Hall 校验表 */

static uint32_t gVibrateUntil[2] = {0, 0}; /* [0]=左 [1]=右，振动(含8ms消抖)结束时刻 */
static int gVibFreezeEnable = 1;
uint8_t Ele_IsVibrating(Ele_Position_te pos)
{
  return gVibFreezeEnable && (gTickMs < gVibrateUntil[pos]);
}

static uint8_t gSeg[2];        /* 0=右 1=左，与固件 index 一致 */
void Ele_UpadteSegment(uint8_t virtualKey, uint8_t seg)
{
  gSeg[virtualKey == VK_LBUTTON ? 1 : 0] = seg;
}

static uint16_t gFiltered[2];
void Hall_SyncParam(uint8_t index, HallRawInfo_ts *pxRawInfo, HallSegmentInfo_ts *pxSegInfo)
{
  (void)pxSegInfo;
  gFiltered[index] = pxRawInfo->wCurrentRaw;
}

/*---------------- 模拟的物理世界 ----------------*/
/* 左键按压深度 0.0(松开)~1.0(按到底) 随时间变化的分段曲线 */
typedef struct { uint32_t t; double depth; const char *note; } KeyFrame;
static const KeyFrame curve[] = {
  {   0, 0.00, "上电，手不碰按键（基线初始化）"},
  { 400, 0.00, "第1次按下：50ms 按到底"},
  { 450, 1.00, "按住"},
  { 600, 1.00, "松开：50ms 回到顶"},
  { 650, 0.00, "空闲"},
  { 800, 0.00, "第2次按下"},
  { 850, 1.00, "按住"},
  { 900, 1.00, "只抬起 40%（测 RT 松开）"},
  { 920, 0.60, "停在 60% 深度"},
  { 960, 0.60, "再往下压 35%（测 RT 按下）"},
  { 980, 0.95, "按住"},
  {1050, 0.95, "完全松开"},
  {1100, 0.00, "结束"},
  {1200, 0.00, NULL},
};

static double depth_at(uint32_t t, const char **note)
{
  size_t n = sizeof(curve) / sizeof(curve[0]);
  for (size_t i = 0; i + 1 < n; i++) {
    if (t >= curve[i].t && t < curve[i + 1].t) {
      if (note) *note = (t == curve[i].t) ? curve[i].note : NULL;
      double k = (double)(t - curve[i].t) / (curve[i + 1].t - curve[i].t);
      return curve[i].depth + k * (curve[i + 1].depth - curve[i].depth);
    }
  }
  if (note) *note = NULL;
  return 0;
}

/* IRPT_V4：按下时 ADC 变小。松开约 820，按到底约 320，带 ±3 的噪声 */
static uint16_t adc_model(double depth)
{
  static uint32_t seed = 12345;
  seed = seed * 1103515245u + 12345u;
  int noise = (int)((seed >> 16) % 7) - 3;
  return (uint16_t)(820 - depth * 500 + noise);
}

int main(int argc, char **argv)
{
  RapidTriggerConfig_ts cfg = {0};
  cfg.bmEnableMask = 0x80;
  cfg.wSegmentActuationPoint      = argc > 1 ? atoi(argv[1]) : DEFAULT_AP;
  cfg.wSegmentRapidTriggerPress   = argc > 2 ? atoi(argv[2]) : DEFAULT_RTP;
  cfg.wSegmentRapidTriggerRelease = argc > 3 ? atoi(argv[3]) : DEFAULT_RTR;
  gVibFreezeEnable                = argc > 4 ? atoi(argv[4]) : 1;
  Hall_UpdateParam(VK_LBUTTON, &cfg);

  printf("参数: AP=%u  RT按下=%u  RT松开=%u  振动冻结=%s\n\n",
         cfg.wSegmentActuationPoint, cfg.wSegmentRapidTriggerPress,
         cfg.wSegmentRapidTriggerRelease, gVibFreezeEnable ? "开" : "关");
  printf("  时间ms  深度%%   ADC  反转滤波后  top基线  bottom基线  段数  状态\n");

  uint8_t lastState = 0;
  for (gTickMs = 0; gTickMs < 1200; gTickMs++) {
    const char *note = NULL;
    double d = depth_at(gTickMs, &note);
    uint16_t left = adc_model(d), right = adc_model(0);

    /* 与 app/button.c LR_GetState() 完全相同的打包方式 */
    uint8_t rx[4] = { right >> 8, right & 0xFF, left >> 8, left & 0xFF };
    uint8_t ret = ProcessRecvSampleRawData(rx, 4);

    uint8_t state = ret & 0x01; /* bit0 = 左键 */
    const char *evt = "";
    if (state != lastState) {
      evt = state ? "  <<< 判定按下，触发振动" : "  >>> 判定松开，触发振动";
      gVibrateUntil[ELE_POS_LEFT] = gTickMs + 1 + 8; /* Ton≈0.5ms + 8ms 消抖 */
      lastState = state;
    }
    if (note) printf("---- %s\n", note);
    if (gTickMs % 10 == 0 || *evt)
      printf("%7u  %5.0f  %5u  %9u  %7u  %9u  %5u  %s%s\n", gTickMs, d * 100, left,
             gFiltered[1], gwTopBaseline[1], gwBottomBaseline[1], gSeg[1],
             state ? "按下" : "松开", evt);
  }
  return 0;
}
