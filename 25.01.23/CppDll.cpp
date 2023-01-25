// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include<vector>
#include "CppDll.h"
#include <iostream>

void kik(unsigned char* a, unsigned char* b, int h, int w, int ileBit, int position)
{
	
	// Oblicz rozmiar paddingu
	int padding = (4 - (w * 3) % 4) % 4;
	int index = 0;
	int iloscWiersza = w * 3;
	// Przeiteruj przez wszystkie wiersze i kolumny
	for (int y = position; y < position+ileBit; y++) {
		if (y == h - 1)
			break;

		for (int x = 1; x < w - 1; x++) {
			// Pobierz indeks dla danego piksela
			index = y * iloscWiersza + x * 3 +padding*y;

			for (int i = 0; i < 3; i++)
			{
				b[index + i] = a[index + 3 * w + i] * (-2) + a[index - 3 * w + i] * 2 +
					a[index - 3 + 3 * w + i] * (-1) + a[index - 3 - 3 * w + i] * 1 +
					a[index + 3 + 3 * w + i] * (-1) + a[index + 3 - 3 * w + i] * 1;
				
			}


		}

		
	}
	
}

//int tmp = a[index + 3 * w + i] * (-2) + a[index - 3 * w + i] * 2 +
//a[index - 3 + 3 * w + i] * (-1) + a[index - 3 - 3 * w + i] * 1 +
//a[index + 3 + 3 * w + i] * (-1) + a[index + 3 - 3 * w + i] * 1;
//if (tmp < 0)
//{
//	b[index + i] = 0;
//}
//else if (tmp > 255)
//{
//	b[index + i] = 255;
//}
//else
//{
//	b[index + i] = tmp;
//}
