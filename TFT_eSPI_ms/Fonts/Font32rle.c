// Font 4
//
// This font has been 8 bit Run Length Encoded to save FLASH space
//
// This font contains 96 ASCII characters

#include <pgmspace.h>


PROGMEM const unsigned char widtbl_f32[96] =         // character width table
{
        5, 8, 8, 19, 14, 21, 17, 6,       // char 32 - 39   x20 !"#$%&'
        8, 8, 12, 10, 7, 8, 7, 8,         // char 40 - 47   X28 ()*+,-./
        14, 14, 14, 14, 14, 14, 14, 14,   // char 48 - 55   x30 01234567
        14, 14, 7, 7, 14, 9, 14, 13,      // char 56 - 63   x38 89:;<=>?
        25, 16, 17, 18, 18, 16, 15, 19,   // char 64 - 71   x40 @ABCDEFG
        18, 6, 13, 17, 13, 21, 18, 19,    // char 72 - 79   x48 HIJKLMNO
        16, 19, 17, 16, 14, 18, 15, 23,   // char 80 - 87   x50 PQRSTUVW
        15, 16, 16, 9, 13, 9, 12, 13,     // char 88 - 95   x58 XYZ[\]^_
        9, 14, 15, 13, 15, 14, 8, 15,     // char 96 - 103  x60 `abcdefg
        15, 6, 6, 12, 6, 22, 15, 15,      // char 104 - 111 x68 hijklmno
        15, 15, 8, 12, 7, 14, 12, 18,     // char 112 - 119 x70 pqrstuvw
//        13, 13, 12, 13, 13, 13, 15, 1     // char 120 - 127 x78 xyz{|}~ 
        13, 13, 12, 13, 13, 13, 15, 21     // char 120 - 127 x78 xyz{|}~ Wifi 
};



// Row format, MSB left

PROGMEM const unsigned char chr_f32_20[] = 
{
0x7F, 0x1
};

PROGMEM const unsigned char chr_f32_21[] = 
{
0x0A, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x15, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x3A
};

PROGMEM const unsigned char chr_f32_22[] = 
{
0x09, 0x80, 0x01, 0x80, 0x03, 0x80, 0x01, 0x80, 
0x03, 0x80, 0x01, 0x80, 0x03, 0x80, 0x01, 0x80, 
0x03, 0x80, 0x01, 0x80, 0x03, 0x80, 0x01, 0x80, 
0x03, 0x80, 0x01, 0x80, 0x7F, 0x11
};

PROGMEM const unsigned char chr_f32_23[] = 
{
0x2E, 0x81, 0x01, 0x81, 0x0C, 0x80, 0x02, 0x80, 
0x0D, 0x80, 0x02, 0x80, 0x0C, 0x81, 0x01, 0x81, 
0x0C, 0x80, 0x02, 0x80, 0x0D, 0x80, 0x02, 0x80, 
0x08, 0x8D, 0x08, 0x80, 0x02, 0x80, 0x0D, 0x80, 
0x02, 0x80, 0x0D, 0x80, 0x02, 0x80, 0x08, 0x8D, 
0x08, 0x80, 0x02, 0x80, 0x0D, 0x80, 0x02, 0x80, 
0x0C, 0x81, 0x01, 0x81, 0x0C, 0x80, 0x02, 0x80, 
0x0D, 0x80, 0x02, 0x80, 0x0C, 0x81, 0x01, 0x81, 
0x7F, 0xD
};

PROGMEM const unsigned char chr_f32_24[] = 
{
0x1F, 0x85, 0x05, 0x89, 0x03, 0x82, 0x03, 0x82, 
0x02, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x0B, 0x82, 0x0B, 0x81, 0x0A, 0x87, 
0x07, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0A, 0x81, 0x0B, 0x86, 0x02, 0x80, 0x01, 0x8B, 
0x01, 0x81, 0x04, 0x83, 0x63
};

PROGMEM const unsigned char chr_f32_25[] = 
{
0x2D, 0x82, 0x07, 0x81, 0x05, 0x86, 0x05, 0x80, 
0x06, 0x81, 0x02, 0x81, 0x04, 0x81, 0x05, 0x81, 
0x04, 0x81, 0x02, 0x81, 0x06, 0x81, 0x04, 0x81, 
0x02, 0x80, 0x07, 0x81, 0x04, 0x81, 0x01, 0x81, 
0x08, 0x81, 0x02, 0x81, 0x02, 0x80, 0x09, 0x86, 
0x01, 0x80, 0x0C, 0x82, 0x02, 0x81, 0x02, 0x82, 
0x0C, 0x80, 0x01, 0x86, 0x09, 0x80, 0x02, 0x81, 
0x02, 0x81, 0x08, 0x81, 0x01, 0x81, 0x04, 0x81, 
0x07, 0x80, 0x02, 0x81, 0x04, 0x81, 0x06, 0x81, 
0x02, 0x81, 0x04, 0x81, 0x05, 0x81, 0x04, 0x81, 
0x02, 0x81, 0x06, 0x80, 0x05, 0x86, 0x05, 0x81, 
0x07, 0x82, 0x07, 0x80, 0x7F, 0xC
};

PROGMEM const unsigned char chr_f32_26[] = 
{
0x15, 0x84, 0x0A, 0x86, 0x08, 0x82, 0x02, 0x82, 
0x07, 0x81, 0x04, 0x81, 0x07, 0x81, 0x04, 0x81, 
0x07, 0x81, 0x03, 0x82, 0x08, 0x81, 0x01, 0x82, 
0x0A, 0x84, 0x0A, 0x83, 0x0B, 0x85, 0x03, 0x81, 
0x03, 0x82, 0x01, 0x82, 0x02, 0x81, 0x02, 0x82, 
0x03, 0x82, 0x01, 0x81, 0x02, 0x81, 0x05, 0x84, 
0x03, 0x81, 0x06, 0x83, 0x03, 0x81, 0x07, 0x81, 
0x04, 0x82, 0x04, 0x85, 0x03, 0x88, 0x01, 0x82, 
0x03, 0x85, 0x04, 0x82, 0x76
};

PROGMEM const unsigned char chr_f32_27[] = 
{
0x06, 0x82, 0x02, 0x82, 0x02, 0x82, 0x04, 0x80, 
0x03, 0x81, 0x02, 0x81, 0x74
};

PROGMEM const unsigned char chr_f32_28[] = 
{
0x0D, 0x81, 0x05, 0x80, 0x05, 0x81, 0x05, 0x80, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x04, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x06, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x06, 0x80, 0x06, 0x81, 0x06, 0x80, 0x06, 0x81, 
0x7
};

PROGMEM const unsigned char chr_f32_29[] = 
{
0x07, 0x81, 0x06, 0x80, 0x06, 0x81, 0x06, 0x80, 
0x06, 0x81, 0x05, 0x81, 0x05, 0x81, 0x06, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x04, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x80, 0x05, 0x81, 0x05, 0x80, 0x05, 0x81, 
0xD
};

PROGMEM const unsigned char chr_f32_2A[] = 
{
0x10, 0x81, 0x09, 0x81, 0x09, 0x81, 0x06, 0x80, 
0x01, 0x81, 0x01, 0x80, 0x02, 0x89, 0x03, 0x85, 
0x06, 0x83, 0x06, 0x81, 0x01, 0x81, 0x04, 0x82, 
0x01, 0x82, 0x04, 0x80, 0x03, 0x80, 0x7F, 0x36
};

PROGMEM const unsigned char chr_f32_2B[] = 
{
0x53, 0x81, 0x07, 0x81, 0x07, 0x81, 0x04, 0x87, 
0x01, 0x87, 0x04, 0x81, 0x07, 0x81, 0x07, 0x81, 
0x67
};

PROGMEM const unsigned char chr_f32_2C[] = 
{
0x71, 0x82, 0x03, 0x82, 0x03, 0x82, 0x05, 0x80, 
0x04, 0x81, 0x04, 0x80, 0x04, 0x81, 0x17
};

PROGMEM const unsigned char chr_f32_2D[] = 
{
0x58, 0x85, 0x01, 0x85, 0x68
};

PROGMEM const unsigned char chr_f32_2E[] = 
{
0x71, 0x82, 0x03, 0x82, 0x03, 0x82, 0x32
};

PROGMEM const unsigned char chr_f32_2F[] = 
{
0x0D, 0x81, 0x05, 0x81, 0x05, 0x80, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x80, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x80, 0x05, 0x81, 0x05, 0x81, 0x05, 0x80, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x80, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x80, 0x05, 0x81, 0x05, 0x81, 
0x2D
};

PROGMEM const unsigned char chr_f32_30[] = 
{
0x20, 0x83, 0x07, 0x87, 0x04, 0x83, 0x01, 0x83, 
0x03, 0x81, 0x05, 0x81, 0x02, 0x82, 0x05, 0x82, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x82, 0x05, 0x82, 
0x02, 0x81, 0x05, 0x81, 0x03, 0x83, 0x01, 0x83, 
0x04, 0x87, 0x07, 0x83, 0x66
};

PROGMEM const unsigned char chr_f32_31[] = 
{
0x22, 0x81, 0x0B, 0x81, 0x0A, 0x82, 0x07, 0x85, 
0x07, 0x85, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x66
};

PROGMEM const unsigned char chr_f32_32[] = 
{
0x1F, 0x85, 0x05, 0x89, 0x03, 0x82, 0x03, 0x82, 
0x02, 0x82, 0x06, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x0B, 0x81, 0x0A, 0x82, 
0x08, 0x83, 0x07, 0x84, 0x06, 0x84, 0x07, 0x83, 
0x08, 0x82, 0x09, 0x82, 0x0A, 0x81, 0x0B, 0x8B, 
0x01, 0x8B, 0x62
};

PROGMEM const unsigned char chr_f32_33[] = 
{
0x1F, 0x85, 0x06, 0x87, 0x04, 0x82, 0x03, 0x82, 
0x03, 0x81, 0x05, 0x81, 0x02, 0x82, 0x05, 0x81, 
0x0B, 0x81, 0x0A, 0x82, 0x07, 0x84, 0x08, 0x85, 
0x0B, 0x82, 0x0B, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x82, 0x05, 0x82, 
0x02, 0x82, 0x03, 0x82, 0x03, 0x89, 0x05, 0x85, 
0x65
};

PROGMEM const unsigned char chr_f32_34[] = 
{
0x24, 0x81, 0x0A, 0x82, 0x09, 0x83, 0x09, 0x83, 
0x08, 0x81, 0x00, 0x81, 0x07, 0x81, 0x01, 0x81, 
0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 0x02, 0x81, 
0x05, 0x81, 0x03, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x03, 0x81, 0x05, 0x81, 0x03, 0x8B, 0x01, 0x8B, 
0x09, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x64
};

PROGMEM const unsigned char chr_f32_35[] = 
{
0x1D, 0x89, 0x03, 0x89, 0x03, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x00, 0x84, 0x05, 0x89, 
0x02, 0x83, 0x03, 0x82, 0x02, 0x81, 0x06, 0x82, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x01, 0x81, 
0x07, 0x81, 0x01, 0x82, 0x05, 0x82, 0x02, 0x82, 
0x03, 0x82, 0x03, 0x89, 0x05, 0x85, 0x65
};

PROGMEM const unsigned char chr_f32_36[] = 
{
0x20, 0x84, 0x06, 0x88, 0x03, 0x82, 0x04, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x01, 0x81, 0x0B, 0x81, 0x01, 0x84, 0x04, 0x8A, 
0x02, 0x83, 0x03, 0x82, 0x02, 0x82, 0x05, 0x82, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x02, 0x81, 0x05, 0x82, 
0x02, 0x82, 0x03, 0x82, 0x04, 0x88, 0x05, 0x85, 
0x65
};

PROGMEM const unsigned char chr_f32_37[] = 
{
0x1C, 0x8B, 0x01, 0x8B, 0x0A, 0x81, 0x0A, 0x82, 
0x09, 0x82, 0x0A, 0x81, 0x0A, 0x82, 0x0A, 0x81, 
0x0A, 0x81, 0x0B, 0x81, 0x0A, 0x82, 0x0A, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0A, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x69
};

PROGMEM const unsigned char chr_f32_38[] = 
{
0x20, 0x83, 0x07, 0x87, 0x05, 0x81, 0x03, 0x81, 
0x04, 0x81, 0x05, 0x81, 0x03, 0x81, 0x05, 0x81, 
0x03, 0x81, 0x05, 0x81, 0x04, 0x81, 0x03, 0x81, 
0x06, 0x85, 0x06, 0x87, 0x04, 0x82, 0x03, 0x82, 
0x02, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x02, 0x82, 0x03, 0x82, 0x03, 0x89, 0x05, 0x85, 
0x65
};

PROGMEM const unsigned char chr_f32_39[] = 
{
0x1F, 0x85, 0x05, 0x88, 0x04, 0x82, 0x03, 0x82, 
0x02, 0x82, 0x05, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x01, 0x81, 0x07, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x01, 0x82, 0x05, 0x82, 0x02, 0x82, 0x03, 0x83, 
0x02, 0x8A, 0x04, 0x84, 0x01, 0x81, 0x0B, 0x81, 
0x01, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x04, 0x82, 0x03, 0x88, 0x05, 0x86, 
0x65
};

PROGMEM const unsigned char chr_f32_3A[] = 
{
0x2B, 0x82, 0x03, 0x82, 0x03, 0x82, 0x34, 0x82, 
0x03, 0x82, 0x03, 0x82, 0x32
};

PROGMEM const unsigned char chr_f32_3B[] = 
{
0x2B, 0x82, 0x03, 0x82, 0x03, 0x82, 0x34, 0x82, 
0x03, 0x82, 0x03, 0x82, 0x05, 0x80, 0x04, 0x81, 
0x04, 0x80, 0x04, 0x81, 0x17
};

PROGMEM const unsigned char chr_f32_3C[] = 
{
0x6B, 0x82, 0x08, 0x82, 0x07, 0x83, 0x07, 0x82, 
0x08, 0x81, 0x0D, 0x82, 0x0D, 0x82, 0x0C, 0x82, 
0x0C, 0x82, 0x7F, 0xC
};

PROGMEM const unsigned char chr_f32_3D[] = 
{
0x51, 0x86, 0x01, 0x86, 0x13, 0x86, 0x01, 0x86, 
0x63
};

PROGMEM const unsigned char chr_f32_3E[] = 
{
0x62, 0x82, 0x0C, 0x82, 0x0C, 0x83, 0x0C, 0x82, 
0x0D, 0x81, 0x08, 0x82, 0x07, 0x83, 0x07, 0x82, 
0x08, 0x82, 0x7F, 0x15
};

PROGMEM const unsigned char chr_f32_3F[] = 
{
0x10, 0x85, 0x04, 0x88, 0x03, 0x82, 0x03, 0x82, 
0x01, 0x82, 0x05, 0x81, 0x01, 0x81, 0x06, 0x81, 
0x01, 0x81, 0x06, 0x81, 0x09, 0x82, 0x08, 0x82, 
0x08, 0x82, 0x08, 0x82, 0x09, 0x81, 0x09, 0x82, 
0x09, 0x82, 0x23, 0x82, 0x09, 0x82, 0x09, 0x82, 
0x5F
};

PROGMEM const unsigned char chr_f32_40[] = 
{
0x21, 0x88, 0x0D, 0x82, 0x06, 0x82, 0x09, 0x82, 
0x0A, 0x81, 0x07, 0x81, 0x0D, 0x81, 0x05, 0x81, 
0x0F, 0x81, 0x04, 0x80, 0x06, 0x83, 0x01, 0x80, 
0x03, 0x81, 0x02, 0x81, 0x04, 0x81, 0x02, 0x83, 
0x03, 0x81, 0x02, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x01, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x01, 0x81, 0x03, 0x81, 0x05, 0x81, 
0x04, 0x81, 0x01, 0x81, 0x03, 0x81, 0x05, 0x81, 
0x04, 0x81, 0x01, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x02, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x02, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x03, 0x81, 0x04, 0x81, 0x03, 0x81, 0x02, 0x82, 
0x02, 0x81, 0x05, 0x81, 0x04, 0x83, 0x01, 0x84, 
0x07, 0x81, 0x17, 0x81, 0x17, 0x81, 0x0B, 0x81, 
0x09, 0x82, 0x05, 0x83, 0x0D, 0x87, 0x6C
};

PROGMEM const unsigned char chr_f32_41[] = 
{
0x15, 0x83, 0x0B, 0x83, 0x0B, 0x83, 0x0A, 0x81, 
0x01, 0x81, 0x09, 0x81, 0x01, 0x81, 0x09, 0x81, 
0x01, 0x81, 0x08, 0x81, 0x03, 0x81, 0x07, 0x81, 
0x03, 0x81, 0x07, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x89, 
0x04, 0x8B, 0x03, 0x81, 0x07, 0x81, 0x03, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x09, 0x81, 0x01, 0x81, 
0x09, 0x81, 0x00, 0x82, 0x09, 0x82, 0x6F
};

PROGMEM const unsigned char chr_f32_42[] = 
{
0x12, 0x8A, 0x05, 0x8C, 0x03, 0x81, 0x07, 0x82, 
0x03, 0x81, 0x08, 0x81, 0x03, 0x81, 0x08, 0x81, 
0x03, 0x81, 0x08, 0x81, 0x03, 0x81, 0x08, 0x81, 
0x03, 0x81, 0x07, 0x82, 0x03, 0x8A, 0x05, 0x8C, 
0x03, 0x81, 0x07, 0x82, 0x03, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x07, 0x82, 
0x03, 0x8C, 0x03, 0x8A, 0x7A
};

PROGMEM const unsigned char chr_f32_43[] = 
{
0x17, 0x86, 0x08, 0x8A, 0x05, 0x83, 0x04, 0x83, 
0x03, 0x82, 0x08, 0x81, 0x03, 0x81, 0x0A, 0x81, 
0x01, 0x82, 0x0A, 0x81, 0x01, 0x81, 0x0F, 0x81, 
0x0F, 0x81, 0x0F, 0x81, 0x0F, 0x81, 0x0F, 0x81, 
0x0B, 0x81, 0x01, 0x82, 0x0A, 0x81, 0x02, 0x81, 
0x09, 0x82, 0x02, 0x82, 0x08, 0x81, 0x04, 0x83, 
0x04, 0x83, 0x05, 0x8A, 0x08, 0x86, 0x7F, 0x2
};

PROGMEM const unsigned char chr_f32_44[] = 
{
0x13, 0x89, 0x07, 0x8B, 0x05, 0x81, 0x06, 0x83, 
0x04, 0x81, 0x08, 0x82, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x82, 0x02, 0x81, 0x0A, 0x81, 
0x02, 0x81, 0x0A, 0x81, 0x02, 0x81, 0x0A, 0x81, 
0x02, 0x81, 0x0A, 0x81, 0x02, 0x81, 0x0A, 0x81, 
0x02, 0x81, 0x0A, 0x81, 0x02, 0x81, 0x09, 0x82, 
0x02, 0x81, 0x09, 0x81, 0x03, 0x81, 0x08, 0x82, 
0x03, 0x81, 0x06, 0x83, 0x04, 0x8B, 0x05, 0x89, 
0x7F, 0x3
};

PROGMEM const unsigned char chr_f32_45[] = 
{
0x11, 0x8C, 0x02, 0x8C, 0x02, 0x81, 0x0D, 0x81, 
0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 
0x0D, 0x8B, 0x03, 0x8B, 0x03, 0x81, 0x0D, 0x81, 
0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 
0x0D, 0x8C, 0x02, 0x8C, 0x70
};

PROGMEM const unsigned char chr_f32_46[] = 
{
0x10, 0x8B, 0x02, 0x8B, 0x02, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x8A, 0x03, 0x8A, 0x03, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x73
};

PROGMEM const unsigned char chr_f32_47[] = 
{
0x18, 0x86, 0x09, 0x8A, 0x06, 0x82, 0x05, 0x83, 
0x04, 0x82, 0x08, 0x81, 0x04, 0x81, 0x09, 0x82, 
0x02, 0x82, 0x0F, 0x81, 0x10, 0x81, 0x10, 0x81, 
0x05, 0x87, 0x02, 0x81, 0x05, 0x87, 0x02, 0x81, 
0x0B, 0x81, 0x02, 0x81, 0x0B, 0x81, 0x02, 0x82, 
0x0A, 0x81, 0x03, 0x81, 0x09, 0x82, 0x03, 0x82, 
0x07, 0x83, 0x04, 0x83, 0x04, 0x84, 0x05, 0x89, 
0x00, 0x81, 0x07, 0x85, 0x03, 0x80, 0x7F, 0x6
};

PROGMEM const unsigned char chr_f32_48[] = 
{
0x13, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x8D, 0x03, 0x8D, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x7F
};

PROGMEM const unsigned char chr_f32_49[] = 
{
0x07, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x2B
};

PROGMEM const unsigned char chr_f32_4A[] = 
{
0x15, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x01, 0x81, 0x06, 0x81, 0x01, 0x81, 0x06, 0x81, 
0x01, 0x81, 0x06, 0x81, 0x01, 0x82, 0x04, 0x82, 
0x02, 0x88, 0x04, 0x86, 0x5E
};

PROGMEM const unsigned char chr_f32_4B[] = 
{
0x12, 0x81, 0x08, 0x82, 0x02, 0x81, 0x07, 0x82, 
0x03, 0x81, 0x06, 0x82, 0x04, 0x81, 0x05, 0x82, 
0x05, 0x81, 0x04, 0x82, 0x06, 0x81, 0x03, 0x82, 
0x07, 0x81, 0x02, 0x82, 0x08, 0x81, 0x01, 0x82, 
0x09, 0x81, 0x00, 0x83, 0x09, 0x87, 0x08, 0x83, 
0x01, 0x82, 0x07, 0x82, 0x03, 0x82, 0x06, 0x81, 
0x05, 0x81, 0x06, 0x81, 0x05, 0x82, 0x05, 0x81, 
0x06, 0x82, 0x04, 0x81, 0x07, 0x82, 0x03, 0x81, 
0x08, 0x82, 0x02, 0x81, 0x09, 0x82, 0x76
};

PROGMEM const unsigned char chr_f32_4C[] = 
{
0x0E, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x8A, 0x01, 0x8A, 0x5A
};

PROGMEM const unsigned char chr_f32_4D[] = 
{
0x16, 0x82, 0x0A, 0x82, 0x03, 0x83, 0x08, 0x83, 
0x03, 0x83, 0x08, 0x83, 0x03, 0x83, 0x08, 0x83, 
0x03, 0x81, 0x00, 0x81, 0x06, 0x81, 0x00, 0x81, 
0x03, 0x81, 0x00, 0x81, 0x06, 0x81, 0x00, 0x81, 
0x03, 0x81, 0x00, 0x81, 0x06, 0x81, 0x00, 0x81, 
0x03, 0x81, 0x01, 0x81, 0x04, 0x81, 0x01, 0x81, 
0x03, 0x81, 0x01, 0x81, 0x04, 0x81, 0x01, 0x81, 
0x03, 0x81, 0x01, 0x81, 0x04, 0x81, 0x01, 0x81, 
0x03, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x03, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x03, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x00, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x00, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x84, 0x03, 0x81, 0x03, 0x81, 
0x04, 0x82, 0x04, 0x81, 0x03, 0x81, 0x04, 0x82, 
0x04, 0x81, 0x7F, 0x14
};

PROGMEM const unsigned char chr_f32_4E[] = 
{
0x13, 0x82, 0x08, 0x81, 0x03, 0x82, 0x08, 0x81, 
0x03, 0x83, 0x07, 0x81, 0x03, 0x84, 0x06, 0x81, 
0x03, 0x81, 0x00, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x00, 0x82, 0x05, 0x81, 0x03, 0x81, 0x01, 0x82, 
0x04, 0x81, 0x03, 0x81, 0x02, 0x81, 0x04, 0x81, 
0x03, 0x81, 0x02, 0x82, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x82, 0x02, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x02, 0x81, 0x03, 0x81, 0x04, 0x82, 0x01, 0x81, 
0x03, 0x81, 0x05, 0x82, 0x00, 0x81, 0x03, 0x81, 
0x06, 0x81, 0x00, 0x81, 0x03, 0x81, 0x06, 0x84, 
0x03, 0x81, 0x07, 0x83, 0x03, 0x81, 0x08, 0x82, 
0x03, 0x81, 0x08, 0x82, 0x7F
};

PROGMEM const unsigned char chr_f32_4F[] = 
{
0x18, 0x86, 0x09, 0x8A, 0x06, 0x83, 0x04, 0x83, 
0x04, 0x82, 0x08, 0x82, 0x03, 0x81, 0x0A, 0x81, 
0x02, 0x82, 0x0A, 0x82, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x82, 0x0A, 0x82, 
0x02, 0x81, 0x0A, 0x81, 0x03, 0x82, 0x08, 0x82, 
0x04, 0x83, 0x04, 0x83, 0x06, 0x8A, 0x09, 0x86, 
0x7F, 0xA
};

PROGMEM const unsigned char chr_f32_50[] = 
{
0x11, 0x8A, 0x04, 0x8B, 0x03, 0x81, 0x07, 0x82, 
0x02, 0x81, 0x08, 0x81, 0x02, 0x81, 0x08, 0x81, 
0x02, 0x81, 0x08, 0x81, 0x02, 0x81, 0x08, 0x81, 
0x02, 0x81, 0x07, 0x82, 0x02, 0x8B, 0x03, 0x8A, 
0x04, 0x81, 0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 
0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 
0x7B
};

PROGMEM const unsigned char chr_f32_51[] = 
{
0x18, 0x86, 0x09, 0x8A, 0x06, 0x83, 0x04, 0x83, 
0x04, 0x82, 0x08, 0x82, 0x03, 0x81, 0x0A, 0x81, 
0x02, 0x82, 0x0A, 0x82, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x81, 0x0C, 0x81, 
0x01, 0x81, 0x0C, 0x81, 0x01, 0x82, 0x06, 0x80, 
0x02, 0x82, 0x02, 0x81, 0x05, 0x82, 0x01, 0x81, 
0x03, 0x82, 0x05, 0x85, 0x04, 0x83, 0x04, 0x83, 
0x06, 0x8C, 0x07, 0x86, 0x01, 0x82, 0x10, 0x80, 
0x73
};

PROGMEM const unsigned char chr_f32_52[] = 
{
0x12, 0x8B, 0x04, 0x8C, 0x03, 0x81, 0x08, 0x82, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x08, 0x81, 0x03, 0x8C, 0x03, 0x8B, 
0x04, 0x81, 0x07, 0x82, 0x03, 0x81, 0x08, 0x81, 
0x03, 0x81, 0x08, 0x82, 0x02, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x09, 0x81, 
0x02, 0x81, 0x09, 0x81, 0x02, 0x81, 0x09, 0x81, 
0x77
};

PROGMEM const unsigned char chr_f32_53[] = 
{
0x14, 0x85, 0x07, 0x89, 0x04, 0x82, 0x05, 0x82, 
0x03, 0x81, 0x07, 0x81, 0x03, 0x81, 0x07, 0x81, 
0x03, 0x81, 0x07, 0x82, 0x02, 0x82, 0x0D, 0x84, 
0x0B, 0x86, 0x0B, 0x85, 0x0C, 0x83, 0x0D, 0x82, 
0x01, 0x82, 0x08, 0x81, 0x02, 0x81, 0x08, 0x81, 
0x02, 0x81, 0x08, 0x81, 0x02, 0x83, 0x05, 0x82, 
0x03, 0x8A, 0x06, 0x86, 0x73
};

PROGMEM const unsigned char chr_f32_54[] = 
{
0x0D, 0x9B, 0x05, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 0x0B, 0x81, 
0x0B, 0x81, 0x67
};

PROGMEM const unsigned char chr_f32_55[] = 
{
0x13, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x81, 0x09, 0x81, 0x03, 0x81, 0x09, 0x81, 
0x03, 0x82, 0x07, 0x82, 0x04, 0x82, 0x05, 0x82, 
0x06, 0x89, 0x08, 0x87, 0x7F, 0x2
};

PROGMEM const unsigned char chr_f32_56[] = 
{
0x0E, 0x82, 0x08, 0x82, 0x00, 0x81, 0x08, 0x81, 
0x01, 0x81, 0x08, 0x81, 0x01, 0x81, 0x08, 0x81, 
0x02, 0x81, 0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x05, 0x81, 0x04, 0x81, 0x05, 0x81, 0x04, 0x81, 
0x06, 0x81, 0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x08, 0x81, 0x00, 0x81, 
0x09, 0x81, 0x00, 0x81, 0x09, 0x84, 0x0A, 0x82, 
0x0B, 0x82, 0x6E
};

PROGMEM const unsigned char chr_f32_57[] = 
{
0x16, 0x82, 0x06, 0x82, 0x06, 0x82, 0x00, 0x81, 
0x06, 0x82, 0x06, 0x81, 0x01, 0x81, 0x06, 0x82, 
0x06, 0x81, 0x01, 0x82, 0x04, 0x81, 0x00, 0x81, 
0x04, 0x82, 0x02, 0x81, 0x04, 0x81, 0x00, 0x81, 
0x04, 0x81, 0x03, 0x81, 0x04, 0x81, 0x00, 0x81, 
0x04, 0x81, 0x03, 0x81, 0x04, 0x81, 0x00, 0x81, 
0x04, 0x81, 0x03, 0x82, 0x02, 0x81, 0x02, 0x81, 
0x02, 0x82, 0x04, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x02, 0x81, 0x05, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x02, 0x81, 0x05, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x02, 0x81, 0x05, 0x82, 0x00, 0x81, 0x04, 0x81, 
0x00, 0x82, 0x06, 0x81, 0x00, 0x81, 0x04, 0x81, 
0x00, 0x81, 0x07, 0x81, 0x00, 0x81, 0x04, 0x81, 
0x00, 0x81, 0x07, 0x81, 0x00, 0x81, 0x04, 0x81, 
0x00, 0x81, 0x07, 0x83, 0x06, 0x83, 0x08, 0x82, 
0x06, 0x82, 0x09, 0x82, 0x06, 0x82, 0x7F, 0x25
};

PROGMEM const unsigned char chr_f32_58[] = 
{
0x0F, 0x81, 0x08, 0x81, 0x01, 0x82, 0x06, 0x82, 
0x02, 0x81, 0x06, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x05, 0x82, 0x02, 0x82, 0x06, 0x81, 0x02, 0x81, 
0x08, 0x81, 0x00, 0x81, 0x09, 0x84, 0x0A, 0x82, 
0x0B, 0x82, 0x0A, 0x81, 0x00, 0x81, 0x08, 0x82, 
0x00, 0x82, 0x07, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x04, 0x81, 0x04, 0x82, 0x04, 0x82, 0x03, 0x81, 
0x06, 0x81, 0x02, 0x82, 0x06, 0x82, 0x00, 0x82, 
0x08, 0x82, 0x68
};

PROGMEM const unsigned char chr_f32_59[] = 
{
0x0F, 0x82, 0x09, 0x82, 0x00, 0x82, 0x07, 0x82, 
0x02, 0x81, 0x07, 0x81, 0x03, 0x82, 0x05, 0x82, 
0x04, 0x81, 0x05, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x07, 0x82, 0x01, 0x82, 0x08, 0x81, 0x01, 0x81, 
0x0A, 0x83, 0x0B, 0x83, 0x0C, 0x81, 0x0D, 0x81, 
0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 0x0D, 0x81, 
0x0D, 0x81, 0x0D, 0x81, 0x76
};

PROGMEM const unsigned char chr_f32_5A[] = 
{
0x11, 0x8C, 0x02, 0x8C, 0x0C, 0x82, 0x0B, 0x82, 
0x0B, 0x82, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x82, 
0x0B, 0x82, 0x0B, 0x82, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x82, 0x0B, 0x82, 0x0B, 0x82, 0x0B, 0x82, 
0x0C, 0x8D, 0x01, 0x8D, 0x70
};

PROGMEM const unsigned char chr_f32_5B[] = 
{
0x0B, 0x84, 0x03, 0x84, 0x03, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x84, 0x03, 0x84, 0x12
};

PROGMEM const unsigned char chr_f32_5C[] = 
{
0x01, 0x80, 0x0B, 0x80, 0x0B, 0x81, 0x0B, 0x80, 
0x0B, 0x80, 0x0B, 0x81, 0x0B, 0x80, 0x0B, 0x80, 
0x0B, 0x81, 0x0B, 0x80, 0x0B, 0x80, 0x0B, 0x81, 
0x0B, 0x80, 0x0B, 0x80, 0x0B, 0x81, 0x0B, 0x80, 
0x0B, 0x80, 0x0B, 0x81, 0x0B, 0x80, 0x0B, 0x80, 
0x0B, 0x81, 0x0B, 0x80, 0x0B, 0x80, 0x0B, 0x81, 
0x0B, 0x80, 0xE
};

PROGMEM const unsigned char chr_f32_5D[] = 
{
0x09, 0x84, 0x03, 0x84, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 0x06, 0x81, 
0x06, 0x81, 0x03, 0x84, 0x03, 0x84, 0x14
};

PROGMEM const unsigned char chr_f32_5E[] = 
{
0x10, 0x82, 0x07, 0x81, 0x00, 0x81, 0x05, 0x81, 
0x02, 0x81, 0x03, 0x81, 0x04, 0x81, 0x01, 0x81, 
0x06, 0x81, 0x7F, 0x6F
};

PROGMEM const unsigned char chr_f32_5F[] = 
{
0x7F, 0x7F, 0x1D, 0x8C, 0x26
};

PROGMEM const unsigned char chr_f32_60[] = 
{
0x0B, 0x83, 0x03, 0x85, 0x01, 0x82, 0x01, 0x82, 
0x00, 0x81, 0x03, 0x81, 0x00, 0x81, 0x03, 0x81, 
0x00, 0x82, 0x01, 0x82, 0x01, 0x85, 0x03, 0x83, 
0x7F, 0x1A
};

PROGMEM const unsigned char chr_f32_61[] = 
{
0x57, 0x85, 0x05, 0x88, 0x04, 0x81, 0x04, 0x82, 
0x03, 0x81, 0x05, 0x81, 0x0B, 0x81, 0x05, 0x87, 
0x03, 0x89, 0x02, 0x82, 0x05, 0x81, 0x02, 0x81, 
0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 0x02, 0x82, 
0x03, 0x83, 0x03, 0x86, 0x00, 0x83, 0x02, 0x84, 
0x02, 0x82, 0x61
};

PROGMEM const unsigned char chr_f32_62[] = 
{
0x10, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x01, 0x83, 0x06, 0x81, 
0x00, 0x86, 0x04, 0x84, 0x01, 0x83, 0x03, 0x82, 
0x05, 0x81, 0x03, 0x81, 0x06, 0x82, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x06, 0x82, 0x02, 0x82, 
0x05, 0x81, 0x03, 0x84, 0x01, 0x83, 0x03, 0x81, 
0x00, 0x86, 0x04, 0x81, 0x01, 0x83, 0x6D
};

PROGMEM const unsigned char chr_f32_63[] = 
{
0x52, 0x83, 0x06, 0x87, 0x03, 0x83, 0x01, 0x82, 
0x03, 0x81, 0x05, 0x81, 0x01, 0x82, 0x09, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x82, 0x05, 0x81, 
0x02, 0x81, 0x05, 0x81, 0x02, 0x83, 0x01, 0x82, 
0x04, 0x87, 0x06, 0x83, 0x5E
};

PROGMEM const unsigned char chr_f32_64[] = 
{
0x19, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x06, 0x83, 0x01, 0x81, 0x04, 0x86, 
0x00, 0x81, 0x03, 0x83, 0x01, 0x84, 0x03, 0x81, 
0x05, 0x82, 0x02, 0x82, 0x06, 0x81, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x82, 0x06, 0x81, 0x03, 0x81, 
0x05, 0x82, 0x03, 0x83, 0x01, 0x84, 0x04, 0x86, 
0x00, 0x81, 0x06, 0x83, 0x01, 0x81, 0x6A
};

PROGMEM const unsigned char chr_f32_65[] = 
{
0x58, 0x83, 0x07, 0x87, 0x04, 0x83, 0x01, 0x83, 
0x03, 0x81, 0x05, 0x81, 0x02, 0x82, 0x06, 0x81, 
0x01, 0x8B, 0x01, 0x8B, 0x01, 0x81, 0x0B, 0x82, 
0x0B, 0x81, 0x05, 0x82, 0x02, 0x83, 0x01, 0x83, 
0x04, 0x87, 0x07, 0x83, 0x66
};

PROGMEM const unsigned char chr_f32_66[] = 
{
0x0A, 0x83, 0x02, 0x84, 0x02, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x03, 0x86, 0x00, 0x86, 0x02, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x3B
};

PROGMEM const unsigned char chr_f32_67[] = 
{
0x5E, 0x83, 0x01, 0x81, 0x04, 0x86, 0x00, 0x81, 
0x03, 0x83, 0x01, 0x84, 0x03, 0x81, 0x05, 0x82, 
0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x82, 0x06, 0x81, 0x03, 0x81, 0x05, 0x82, 
0x03, 0x83, 0x01, 0x84, 0x04, 0x86, 0x00, 0x81, 
0x06, 0x83, 0x01, 0x81, 0x0C, 0x81, 0x02, 0x81, 
0x07, 0x81, 0x02, 0x81, 0x06, 0x82, 0x03, 0x82, 
0x03, 0x82, 0x04, 0x89, 0x06, 0x85, 0x13
};

PROGMEM const unsigned char chr_f32_68[] = 
{
0x10, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x01, 0x84, 0x05, 0x81, 
0x00, 0x86, 0x04, 0x83, 0x03, 0x82, 0x03, 0x82, 
0x05, 0x81, 0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 
0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 0x6A
};

PROGMEM const unsigned char chr_f32_69[] = 
{
0x07, 0x81, 0x03, 0x81, 0x03, 0x81, 0x0F, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x2B
};

PROGMEM const unsigned char chr_f32_6A[] = 
{
0x07, 0x81, 0x03, 0x81, 0x03, 0x81, 0x0F, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x01, 0x83, 0x01, 0x82, 0x8
};

PROGMEM const unsigned char chr_f32_6B[] = 
{
0x19, 0x81, 0x09, 0x81, 0x09, 0x81, 0x09, 0x81, 
0x09, 0x81, 0x04, 0x82, 0x01, 0x81, 0x03, 0x82, 
0x02, 0x81, 0x02, 0x82, 0x03, 0x81, 0x01, 0x82, 
0x04, 0x81, 0x00, 0x82, 0x05, 0x85, 0x05, 0x86, 
0x04, 0x82, 0x01, 0x81, 0x04, 0x81, 0x02, 0x82, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x82, 
0x02, 0x81, 0x04, 0x81, 0x02, 0x81, 0x04, 0x82, 
0x53
};

PROGMEM const unsigned char chr_f32_6C[] = 
{
0x07, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 0x03, 0x81, 
0x03, 0x81, 0x03, 0x81, 0x2B
};

PROGMEM const unsigned char chr_f32_6D[] = 
{
0x7F, 0x05, 0x81, 0x01, 0x83, 0x03, 0x83, 0x05, 
0x81, 0x00, 0x85, 0x00, 0x86, 0x04, 0x83, 0x02, 
0x84, 0x02, 0x82, 0x03, 0x82, 0x04, 0x82, 0x04, 
0x81, 0x03, 0x81, 0x05, 0x81, 0x05, 0x81, 0x03, 
0x81, 0x05, 0x81, 0x05, 0x81, 0x03, 0x81, 0x05, 
0x81, 0x05, 0x81, 0x03, 0x81, 0x05, 0x81, 0x05, 
0x81, 0x03, 0x81, 0x05, 0x81, 0x05, 0x81, 0x03, 
0x81, 0x05, 0x81, 0x05, 0x81, 0x03, 0x81, 0x05, 
0x81, 0x05, 0x81, 0x03, 0x81, 0x05, 0x81, 0x05, 
0x81, 0x03, 0x81, 0x05, 0x81, 0x05, 0x81, 0x7F, 
0x1B
};

PROGMEM const unsigned char chr_f32_6E[] = 
{
0x5B, 0x81, 0x01, 0x84, 0x05, 0x81, 0x00, 0x86, 
0x04, 0x83, 0x03, 0x82, 0x03, 0x82, 0x05, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x03, 0x81, 0x06, 0x81, 
0x03, 0x81, 0x06, 0x81, 0x6A
};

PROGMEM const unsigned char chr_f32_6F[] = 
{
0x5E, 0x84, 0x07, 0x88, 0x04, 0x83, 0x02, 0x83, 
0x03, 0x81, 0x06, 0x81, 0x02, 0x82, 0x06, 0x82, 
0x01, 0x81, 0x08, 0x81, 0x01, 0x81, 0x08, 0x81, 
0x01, 0x81, 0x08, 0x81, 0x01, 0x82, 0x06, 0x82, 
0x02, 0x81, 0x06, 0x81, 0x03, 0x83, 0x02, 0x83, 
0x04, 0x88, 0x07, 0x84, 0x6D
};

PROGMEM const unsigned char chr_f32_70[] = 
{
0x5B, 0x81, 0x01, 0x83, 0x06, 0x81, 0x00, 0x86, 
0x04, 0x84, 0x01, 0x83, 0x03, 0x82, 0x05, 0x81, 
0x03, 0x81, 0x06, 0x82, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x81, 0x06, 0x82, 0x02, 0x82, 0x05, 0x81, 
0x03, 0x84, 0x01, 0x83, 0x03, 0x81, 0x00, 0x86, 
0x04, 0x81, 0x01, 0x83, 0x06, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x28
};

PROGMEM const unsigned char chr_f32_71[] = 
{
0x5E, 0x83, 0x01, 0x81, 0x04, 0x86, 0x00, 0x81, 
0x03, 0x83, 0x01, 0x84, 0x03, 0x81, 0x05, 0x82, 
0x02, 0x82, 0x06, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x81, 0x07, 0x81, 0x02, 0x81, 0x07, 0x81, 
0x02, 0x82, 0x06, 0x81, 0x03, 0x81, 0x05, 0x82, 
0x03, 0x83, 0x01, 0x84, 0x04, 0x86, 0x00, 0x81, 
0x06, 0x83, 0x01, 0x81, 0x0C, 0x81, 0x0C, 0x81, 
0x0C, 0x81, 0x0C, 0x81, 0x0C, 0x81, 0x1F
};

PROGMEM const unsigned char chr_f32_72[] = 
{
0x31, 0x81, 0x00, 0x82, 0x01, 0x85, 0x01, 0x82, 
0x04, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 0x05, 0x81, 
0x05, 0x81, 0x05, 0x81, 0x3B
};

PROGMEM const unsigned char chr_f32_73[] = 
{
0x4A, 0x84, 0x05, 0x87, 0x02, 0x82, 0x03, 0x81, 
0x02, 0x81, 0x05, 0x81, 0x01, 0x81, 0x0A, 0x84, 
0x07, 0x86, 0x07, 0x84, 0x09, 0x81, 0x01, 0x81, 
0x05, 0x81, 0x01, 0x82, 0x03, 0x82, 0x02, 0x87, 
0x04, 0x85, 0x56
};

PROGMEM const unsigned char chr_f32_74[] = 
{
0x0F, 0x81, 0x04, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x02, 0x85, 0x00, 0x85, 0x02, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x04, 0x81, 0x04, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x04, 0x81, 0x04, 0x81, 0x04, 0x83, 
0x03, 0x82, 0x31
};

PROGMEM const unsigned char chr_f32_75[] = 
{
0x55, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x02, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x02, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x02, 0x81, 0x06, 0x81, 0x02, 0x81, 0x06, 0x81, 
0x02, 0x81, 0x06, 0x81, 0x02, 0x81, 0x05, 0x82, 
0x02, 0x82, 0x03, 0x83, 0x03, 0x86, 0x00, 0x81, 
0x04, 0x84, 0x01, 0x81, 0x62
};

PROGMEM const unsigned char chr_f32_76[] = 
{
0x47, 0x81, 0x06, 0x81, 0x00, 0x81, 0x06, 0x81, 
0x01, 0x81, 0x04, 0x81, 0x02, 0x81, 0x04, 0x81, 
0x02, 0x81, 0x04, 0x81, 0x03, 0x81, 0x02, 0x81, 
0x04, 0x81, 0x02, 0x81, 0x04, 0x81, 0x02, 0x81, 
0x05, 0x81, 0x00, 0x81, 0x06, 0x81, 0x00, 0x81, 
0x06, 0x84, 0x07, 0x82, 0x08, 0x82, 0x58
};

PROGMEM const unsigned char chr_f32_77[] = 
{
0x6B, 0x81, 0x04, 0x82, 0x04, 0x81, 0x00, 0x81, 
0x04, 0x82, 0x04, 0x81, 0x00, 0x81, 0x04, 0x80, 
0x00, 0x80, 0x04, 0x81, 0x01, 0x81, 0x02, 0x81, 
0x00, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x00, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x81, 
0x00, 0x81, 0x02, 0x81, 0x02, 0x81, 0x02, 0x80, 
0x02, 0x80, 0x02, 0x81, 0x03, 0x81, 0x00, 0x81, 
0x02, 0x81, 0x00, 0x81, 0x04, 0x81, 0x00, 0x81, 
0x02, 0x81, 0x00, 0x81, 0x04, 0x81, 0x00, 0x81, 
0x02, 0x81, 0x00, 0x81, 0x05, 0x82, 0x04, 0x82, 
0x06, 0x82, 0x04, 0x82, 0x06, 0x82, 0x04, 0x82, 
0x7F, 0x1
};

PROGMEM const unsigned char chr_f32_78[] = 
{
0x4E, 0x81, 0x06, 0x81, 0x01, 0x82, 0x04, 0x82, 
0x02, 0x81, 0x04, 0x81, 0x04, 0x81, 0x02, 0x81, 
0x05, 0x82, 0x00, 0x82, 0x06, 0x84, 0x08, 0x82, 
0x08, 0x84, 0x06, 0x82, 0x00, 0x82, 0x05, 0x81, 
0x02, 0x81, 0x04, 0x81, 0x04, 0x81, 0x02, 0x82, 
0x04, 0x82, 0x01, 0x81, 0x06, 0x81, 0x5B
};

PROGMEM const unsigned char chr_f32_79[] = 
{
0x4D, 0x82, 0x06, 0x82, 0x00, 0x81, 0x06, 0x81, 
0x01, 0x81, 0x06, 0x81, 0x02, 0x81, 0x04, 0x81, 
0x03, 0x81, 0x04, 0x81, 0x03, 0x81, 0x04, 0x81, 
0x04, 0x81, 0x02, 0x81, 0x05, 0x81, 0x02, 0x81, 
0x05, 0x81, 0x02, 0x81, 0x06, 0x81, 0x00, 0x81, 
0x07, 0x81, 0x00, 0x81, 0x07, 0x81, 0x00, 0x81, 
0x08, 0x82, 0x09, 0x82, 0x09, 0x82, 0x09, 0x81, 
0x09, 0x82, 0x06, 0x84, 0x07, 0x83, 0x14
};

PROGMEM const unsigned char chr_f32_7A[] = 
{
0x48, 0x89, 0x01, 0x89, 0x08, 0x82, 0x07, 0x82, 
0x07, 0x82, 0x07, 0x82, 0x08, 0x81, 0x08, 0x82, 
0x07, 0x82, 0x07, 0x82, 0x07, 0x82, 0x08, 0x89, 
0x01, 0x89, 0x54
};

PROGMEM const unsigned char chr_f32_7B[] = 
{
0x06, 0x83, 0x07, 0x82, 0x09, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x09, 0x81, 
0x08, 0x82, 0x0B, 0x81, 0x0B, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x82, 
0x0A, 0x83, 0xE
};

PROGMEM const unsigned char chr_f32_7C[] = 
{
0x04, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x12
};

PROGMEM const unsigned char chr_f32_7D[] = 
{
0x01, 0x83, 0x0A, 0x82, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0B, 0x81, 
0x0B, 0x82, 0x08, 0x81, 0x09, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 
0x0A, 0x81, 0x0A, 0x81, 0x0A, 0x81, 0x09, 0x82, 
0x07, 0x83, 0x13
};

PROGMEM const unsigned char chr_f32_7E[] = 
{
0x11, 0x83, 0x09, 0x81, 0x01, 0x81, 0x07, 0x81, 
0x03, 0x82, 0x03, 0x80, 0x00, 0x80, 0x06, 0x81, 
0x01, 0x81, 0x00, 0x80, 0x07, 0x83, 0x7F, 0x7F, 
0x2C
};

PROGMEM const unsigned char chr_f32_7F[] = 
{
//0x19                                              // original

//0X46, 0X84, 0X0C, 0X82, 0X04, 0X82, 0X07, 0X81,   // first trial of wifi
//0X0A, 0X81, 0X03, 0X81, 0X0E, 0X81, 0X1E, 0X82,
//0X0E, 0X82, 0X02, 0X82, 0X09, 0X81, 0X08, 0X81,
//0X06, 0X80, 0X0C, 0X80, 0X20, 0X82, 0X0F, 0X81,
//0X02, 0X81, 0X0C, 0X80, 0X06, 0X80, 0X0A, 0X80,
//0X08, 0X80, 0X38, 0X80, 0X12, 0X82, 0X10, 0X84,
//0X10, 0X82, 0X12, 0X80, 0X33




0X45, 0X86, 0X0B, 0X81, 0X06, 0X81, 0X07, 0X81, 
0X0A, 0X81, 0X04, 0X80, 0X0E, 0X80, 0X02, 0X80,
0X10, 0X80, 0X09, 0X82, 0X0E, 0X82, 0X02, 0X82,
0X09, 0X81, 0X08, 0X81, 0X06, 0X80, 0X0C, 0X80,
0X20, 0X82, 0X0F, 0X81, 0X02, 0X81, 0X0C, 0X80,
0X06, 0X80, 0X0A, 0X80, 0X08, 0X80, 0X38, 0X80,
0X12, 0X82, 0X10, 0X84, 0X10, 0X82, 0X12, 0X80,
0X33

};





PROGMEM const unsigned char* const chrtbl_f32[96] =       // character pointer table
{
        chr_f32_20, chr_f32_21, chr_f32_22, chr_f32_23, chr_f32_24, chr_f32_25, chr_f32_26, chr_f32_27, 
        chr_f32_28, chr_f32_29, chr_f32_2A, chr_f32_2B, chr_f32_2C, chr_f32_2D, chr_f32_2E, chr_f32_2F, 
        chr_f32_30, chr_f32_31, chr_f32_32, chr_f32_33, chr_f32_34, chr_f32_35, chr_f32_36, chr_f32_37, 
        chr_f32_38, chr_f32_39, chr_f32_3A, chr_f32_3B, chr_f32_3C, chr_f32_3D, chr_f32_3E, chr_f32_3F, 
        chr_f32_40, chr_f32_41, chr_f32_42, chr_f32_43, chr_f32_44, chr_f32_45, chr_f32_46, chr_f32_47, 
        chr_f32_48, chr_f32_49, chr_f32_4A, chr_f32_4B, chr_f32_4C, chr_f32_4D, chr_f32_4E, chr_f32_4F, 
        chr_f32_50, chr_f32_51, chr_f32_52, chr_f32_53, chr_f32_54, chr_f32_55, chr_f32_56, chr_f32_57, 
        chr_f32_58, chr_f32_59, chr_f32_5A, chr_f32_5B, chr_f32_5C, chr_f32_5D, chr_f32_5E, chr_f32_5F, 
        chr_f32_60, chr_f32_61, chr_f32_62, chr_f32_63, chr_f32_64, chr_f32_65, chr_f32_66, chr_f32_67, 
        chr_f32_68, chr_f32_69, chr_f32_6A, chr_f32_6B, chr_f32_6C, chr_f32_6D, chr_f32_6E, chr_f32_6F, 
        chr_f32_70, chr_f32_71, chr_f32_72, chr_f32_73, chr_f32_74, chr_f32_75, chr_f32_76, chr_f32_77, 
        chr_f32_78, chr_f32_79, chr_f32_7A, chr_f32_7B, chr_f32_7C, chr_f32_7D, chr_f32_7E, chr_f32_7F
};
