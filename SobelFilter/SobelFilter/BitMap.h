#pragma once
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;



typedef struct
{
    unsigned int    bfType;
    unsigned long   bfSize;
    unsigned int    bfReserved1;
    unsigned int    bfReserved2;
    unsigned long   bfOffBits;
} BitMapFileHeader;

typedef struct
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
} BitMapInfoHeader;


typedef struct
{
    unsigned char  rgbBlue;
    unsigned char   rgbGreen;
    unsigned char   rgbRed;
    unsigned char   rgbReserved;
} RGBColor;


unsigned short read_u16(ifstream& pFile);

unsigned int read_u32(ifstream& pFile);

int read_s32(ifstream& pFile);