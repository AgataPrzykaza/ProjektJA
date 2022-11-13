// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include<vector>
#include "CppDll.h"
#include <iostream>
//int kik(std::vector<char> a)
//{
//   /* auto temp = a[0];
//       a[0] = a[2];
//         a[2] = temp;*/
//  
//    a[2] = (BYTE)100;
//
//    return a[2];
////}

unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w,int th)
{
    /*    auto temp = a[0];
           a[0] = a[2];
             a[2] = temp;*/

    b[0] = a[2];
    b[1] = a[1];
    b[2] = a[0];
    


    return b;
}