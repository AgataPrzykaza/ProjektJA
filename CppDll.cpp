// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include<vector>
#include "CppDll.h"
#include <iostream>


unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w,int ileBit,int position)
{
    
	int index = position*3;

	for (int i = position; i < position + ileBit; i++)
	{
		int row = (i - i % w) / w;	//sprawdzenie row
		
		
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
		//GY - + w czyli szerokosc wiersza by miec dostep do wyzej lub nizej
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