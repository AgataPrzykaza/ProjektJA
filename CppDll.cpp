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

unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w,int ileBit,int position)
{
    
	int index = position*3;

	for (int i = position; i < position + ileBit; i++)
	{
		int row = (i - i % w) / w;	//safe row calculation

		int GX,GY;
		if (i % w == 0 || i % w == w - 1 || row == 0)
		{
			index += 3;
			continue;
		}
		
		else if (row == h - 1)
		{
			index += 3;
			break;
		}
		//GY
	b[index]= a[i + w] * -2 + a[i - w] * 2 +
		a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
		a[i + 1 + w] * -1 + a[i + 1 - w] * 1;
	b[index+1]= a[i + w] * -2 + a[i - w] * 2 +
		a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
		a[i + 1 + w] * -1 + a[i + 1 - w] * 1;

	b[index + 2]= a[i + w] * -2 + a[i - w] * 2 +
		a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
		a[i + 1 + w] * -1 + a[i + 1 - w] * 1;

	//gX
		//b[index] = a[i + 1] * -2 + a[i - 1] * 2 +
		//a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
		//a[i + 1 + w] * -1 + a[i - 1 + w] * 1;

		//b[index+1] = a[i + 1] * -2 + a[i - 1] * 2 +
		//	a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
		//	a[i + 1 + w] * -1 + a[i - 1 + w] * 1;

		//b[index+2] = a[i + 1] * -2 + a[i - 1] * 2 +
		//	a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
		//	a[i + 1 + w] * -1 + a[i - 1 + w] * 1;

		/*a[i + w] * -2 + a[i - w] * 2 +
			a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
			a[i + 1 + w] * -1 + a[i + 1 - w] * 1;*/
		index += 3;
		 
	}

	
    return b;
}