#pragma once


////////////////////////////////////////////
// KEY INPUT (BASE ON WINDOW VIRTUAL KEY)
#define KI_LBUTTON        0x01
#define KI_RBUTTON        0x02
#define KI_CANCEL         0x03
#define KI_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

/*
* 0x07 : unassigned
*/

#define KI_BACK           0x08
#define KI_TAB            0x09

/*
* 0x0A - 0x0B : reserved
*/

#define KI_CLEAR          0x0C
#define KI_RETURN         0x0D

#define KI_SHIFT          0x10
#define KI_CONTROL        0x11
#define KI_MENU           0x12
#define KI_PAUSE          0x13
#define KI_CAPITAL        0x14

#define KI_KANA           0x15
#define KI_HANGEUL        0x15  /* old name - should be here for compatibility */
#define KI_HANGUL         0x15
#define KI_JUNJA          0x17
#define KI_FINAL          0x18
#define KI_HANJA          0x19
#define KI_KANJI          0x19

#define KI_ESCAPE         0x1B

#define KI_CONVERT        0x1C
#define KI_NONCONVERT     0x1D
#define KI_ACCEPT         0x1E
#define KI_MODECHANGE     0x1F

#define KI_SPACE          0x20
#define KI_PRIOR          0x21
#define KI_NEXT           0x22
#define KI_END            0x23
#define KI_HOME           0x24
#define KI_LEFT           0x25
#define KI_UP             0x26
#define KI_RIGHT          0x27
#define KI_DOWN           0x28
#define KI_SELECT         0x29
#define KI_PRINT          0x2A
#define KI_EXECUTE        0x2B
#define KI_SNAPSHOT       0x2C
#define KI_INSERT         0x2D
#define KI_DELETE         0x2E
#define KI_HELP           0x2F

/*
* KI_0 - KI_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
* 0x40 : unassigned
* KI_A - KI_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
*/

#define KI_LWIN           0x5B
#define KI_RWIN           0x5C
#define KI_APPS           0x5D

/*
* 0x5E : reserved
*/

#define KI_SLEEP          0x5F

#define KI_NUMPAD0        0x60
#define KI_NUMPAD1        0x61
#define KI_NUMPAD2        0x62
#define KI_NUMPAD3        0x63
#define KI_NUMPAD4        0x64
#define KI_NUMPAD5        0x65
#define KI_NUMPAD6        0x66
#define KI_NUMPAD7        0x67
#define KI_NUMPAD8        0x68
#define KI_NUMPAD9        0x69
#define KI_MULTIPLY       0x6A
#define KI_ADD            0x6B
#define KI_SEPARATOR      0x6C
#define KI_SUBTRACT       0x6D
#define KI_DECIMAL        0x6E
#define KI_DIVIDE         0x6F
#define KI_F1             0x70
#define KI_F2             0x71
#define KI_F3             0x72
#define KI_F4             0x73
#define KI_F5             0x74
#define KI_F6             0x75
#define KI_F7             0x76
#define KI_F8             0x77
#define KI_F9             0x78
#define KI_F10            0x79
#define KI_F11            0x7A
#define KI_F12            0x7B
#define KI_F13            0x7C
#define KI_F14            0x7D
#define KI_F15            0x7E
#define KI_F16            0x7F
#define KI_F17            0x80
#define KI_F18            0x81
#define KI_F19            0x82
#define KI_F20            0x83
#define KI_F21            0x84
#define KI_F22            0x85
#define KI_F23            0x86
#define KI_F24            0x87

/*
* 0x88 - 0x8F : unassigned
*/

#define KI_NUMLOCK        0x90
#define KI_SCROLL         0x91

/*
* NEC PC-9800 kbd definitions
*/
#define KI_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
* Fujitsu/OASYS kbd definitions
*/
#define KI_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define KI_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define KI_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define KI_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define KI_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
* 0x97 - 0x9F : unassigned
*/

/*
* KI_L* & KI_R* - left and right Alt, Ctrl and Shift virtual keys.
* Used only as parameters to GetAsyncKeyState() and GetKeyState().
* No other API or message will distinguish left and right keys in this way.
*/
#define KI_LSHIFT         0xA0
#define KI_RSHIFT         0xA1
#define KI_LCONTROL       0xA2
#define KI_RCONTROL       0xA3
#define KI_LMENU          0xA4
#define KI_RMENU          0xA5

#define KI_BROWSER_BACK        0xA6
#define KI_BROWSER_FORWARD     0xA7
#define KI_BROWSER_REFRESH     0xA8
#define KI_BROWSER_STOP        0xA9
#define KI_BROWSER_SEARCH      0xAA
#define KI_BROWSER_FAVORITES   0xAB
#define KI_BROWSER_HOME        0xAC

#define KI_VOLUME_MUTE         0xAD
#define KI_VOLUME_DOWN         0xAE
#define KI_VOLUME_UP           0xAF
#define KI_MEDIA_NEXT_TRACK    0xB0
#define KI_MEDIA_PREV_TRACK    0xB1
#define KI_MEDIA_STOP          0xB2
#define KI_MEDIA_PLAY_PAUSE    0xB3
#define KI_LAUNCH_MAIL         0xB4
#define KI_LAUNCH_MEDIA_SELECT 0xB5
#define KI_LAUNCH_APP1         0xB6
#define KI_LAUNCH_APP2         0xB7

