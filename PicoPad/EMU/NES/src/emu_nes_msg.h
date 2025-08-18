
// ****************************************************************************
//
//                              NES Emulator - Message
//
// ****************************************************************************
// PicoLibSDK - Alternative SDK library for Raspberry Pico and RP2040
// Copyright (c) 2023 Miroslav Nemecek, Panda38@seznam.cz, hardyplotter2@gmail.com
// 	https://github.com/Panda381/PicoLibSDK
//	https://www.breatharian.eu/hw/picolibsdk/index_en.html
//	https://github.com/pajenicko/picopad
//	https://picopad.eu/en/
// License:
//	This source code is freely available for any purpose, including commercial.
//	It is possible to take and modify the code or parts of it, without restriction.

// limit text rendering to 240x240 even if global WIDTH/HEIGHT are larger
#if WIDTH > 240
#define NES_LCD_WIDTH   240
#else
#define NES_LCD_WIDTH   WIDTH
#endif

#if HEIGHT > 240
#define NES_LCD_HEIGHT  240
#else
#define NES_LCD_HEIGHT  HEIGHT
#endif
#define NES_MSG_WIDTH   (NES_LCD_WIDTH*21/240) // message text width (21 columns)
#define NES_TEXT_SCALE  100                     // horizontal text scale in percent (100 = original size, 80 = 20% smaller)
#define NES_MSG_HEIGHT  11                     // message text height of window, 4 rows height 32, 7 rows height 16
#define NES_MSG_BTMLINE (4*32)                 // bottom line to start 16-line rows

// text screen buffer (only characters; 176 bytes)
extern u8 NES_TextFrame[NES_MSG_WIDTH*NES_MSG_HEIGHT];

// colors of rows
extern u16 NES_TextColor[NES_MSG_HEIGHT];
extern u16 NES_TextBgColor;

// text coordinates
extern u8 NES_TextX, NES_TextY;

// next row color
extern u16 NES_TextNextCol;

// clear text screen
void NES_TextClear();

// set text coordinates
void NES_TextSetPos(int x, int y);

// set text color
INLINE void NES_TextSetCol(u16 col) { NES_TextNextCol = col; }

// next line
void NES_TextPrintNL(int num);

// print character (incl. CR/LF) at current position, with current color (entire row has the same colour)
void NES_TextPrintCh(char ch);

// print spaces
void NES_TextPrintSpc(int num);

// print ASCIIZ text (incl. CR/LF) at current position, with current color (entire row has the same colour)
void NES_TextPrint(const char* txt);

// print text centred
void NES_TextPrintCenter(const char* txt);

// print HEX byte
void NES_TextPrintHexByte(u8 n);

// update text screen on display
void NES_TextUpdate();

// error message, repeat or exit (return True = repeat (or ignore), False = ignore or exit)
//Bool NES_ErrorMsg(const char* txt, const char* txt2, Bool repeat, Bool update);

// debug - display memory
//void DB_HexMsg(int addr, Bool update);
