#include "BitMap.h"

unsigned short read_u16(ifstream& pFile) {
    unsigned char b0, b1;
    b0 = pFile.get();
    b1 = pFile.get();

    return ((b1 << 8) | b0);
}

unsigned int read_u32(ifstream& pFile) {
    unsigned char b0, b1, b2, b3;
    b0 = pFile.get();
    b1 = pFile.get();
    b2 = pFile.get();
    b3 = pFile.get();

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

int read_s32(ifstream& pFile) {
    unsigned char b0, b1, b2, b3;
    b0 = pFile.get();
    b1 = pFile.get();
    b2 = pFile.get();
    b3 = pFile.get();
    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);

}