#ifndef __KEYTBL_H__
#define __KEYTBL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"
#include "Main.h"

#ifdef  __KEYTBL_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif


/*Mouse Button Table define */
#if 0 //no use
#define VK_NULL 0x00/*No Press*/
#define VK_LBUTTON 0x01/*Left mouse button*/
#define VK_RBUTTON 0x02/*Right mouse button*/
#define VK_CANCEL 0x03/*Control-break processing*/
#define VK_MBUTTON 0x04/*Middle mouse button on a three-button mouse*/
#define VK_XBUTTON1 0x05/*Windows 2000/XP: X1 mouse button*/
#define VK_XBUTTON2 0x06/*Windows 2000/XP: X2 mouse button*/
#define VK_UNDEFINED 0x07/*Undefined*/
#define VK_BACK 0x08/*BACKSPACE key*/
#define VK_TAB 0x09/*TAB key*/
#define VK_CLEAR 0x0C/*CLEAR key*/
#define VK_RETURN 0x0D/*ENTER key*/
#define VK_SHIFT 0x10/*SHIFT key*/
#define VK_CONTROL 0x11/*CTRL key*/
#define VK_MENU 0x12/*ALT key*/
#define VK_PAUSE 0x13/*PAUSE key*/
#define VK_CAPITAL 0x14/*CAPS LOCK key*/
#define VK_ESCAPE 0x1B/*ESC key*/
#define VK_CONVERT 0x1C/*IME convert*/
#define VK_SPACE 0x20/*SPACEBAR*/
#define VK_PRIOR 0x21/*PAGE UP key*/
#define VK_NEXT 0x22/*PAGE DOWN key*/
#define VK_END 0x23/*END key*/
#define VK_HOME 0x24/*HOME key*/
#define VK_LEFT 0x25/*LEFT ARROW key*/
#define VK_UP 0x26/*UP ARROW key*/
#define VK_RIGHT 0x27/*RIGHT ARROW key*/
#define VK_DOWN 0x28/*DOWN ARROW key*/
#define VK_SELECT 0x29/*SELECT key*/
#define VK_PRINT 0x2A/*PRINT key*/
#define VK_EXECUTE 0x2B/*EXECUTE key*/
#define VK_SNAPSHOT 0x2C/*PRINT SCREEN key*/
#define VK_INSERT 0x2D/*INS key*/
#define VK_DELETE 0x2E/*DEL key*/
#define VK_HELP 0x2F/*HELP key*/


/*Keyboard Table Define*/
#define NO_EVENT 0x0000
#define RIGHT_ARROW 0x4F
#define LEFT_ARROW 0x50
#define MINS 0x2D
#define PLUG 0x2E
#define LEFT_ALT 0x04/*Left Alt	Normal key*/
#define LEFT_GUI	0x08/*Left GUI	Normal key*/
#define RIGHT_ALT 0x40/*Right Alt Normal key*/
#define RIGHT_GUI 0x80/*Right GUI Normal key*/
#define LEFT_CTRL 0x01/*Left CtrlNormal key*/
#define RIGHT_CTRL 0x10/*Right Ctrl	Normal key*/
#define C_KEY 0x06/*C key*/
#define V_KEY 0x19/*V key*/
#define X_KEY 0x1B/*X key*/
#define Z_KEY 0x1D/*Z key*/
#define Y_KEY 0x1C/*Y key*/
#define S_KEY 0x16/*S key*/

#endif

//Key Matrix
#define MODE_KEYBOARD			0x00
#define MODE_MOUSE				0x02
#define MODE_SYSTEM			0x07
#define MODE_MEDIA				0x04
#define MODE_SPECIAL			0x05
#define MODE_SHORTCUT			0x01
#define MODE_DPI				0x71
#define MODE_PROFILE_SWITCH	0x70
#define MODE_INSTANT_PROFILE	0x73
#define MODE_MACRO				0x53

#define MAX_MACRO_ACTION	40

#define KEY_MATRIX_LEN	0x04
#define KEY_DEBOUNCE    5//ms //0xC0

typedef struct _KEY_SET_
{
	U8 Press;
	U8 Changed;
	U16 Debounce;
	U8 PHY_Status;
	U8 Key_Assignment[KEY_MATRIX_LEN];
} Key_Setting;
#define KEY_MAC_OS_NUM      (14+1)
INTERFACE Key_Setting	g_Key_Matrix[14];
#if 0
typedef struct _KEY_ATTR_
{
	U8	Press;  //Bit15
	U16 Delay;  //Bit 14~Bit0
}KEY_ATTR;
#else

typedef union
{
  struct
  {
    U16 Delay:15;  //Bit 14~Bit0
    U16	Press:1;  //Bit15
  } bit;
  U16 u16Value;

}KEY_ATTR;
#endif

#ifdef M_Macro_Key
typedef struct _MACRO_KEY_
{
	U16	Repeat_Count;
	KEY_ATTR	Key_Attr[40];
	U8	KeyCode[40];
} MACRO_KEY;

INTERFACE MACRO_KEY g_Macro_Key[KEY_MAC_OS_NUM];//14key
#endif

//INTERFACE KEY_MATRIX	g_Key_Matrix;

#undef INTERFACE
#ifdef __cplusplus
}
#endif
#endif /* end __GPIO_H */

