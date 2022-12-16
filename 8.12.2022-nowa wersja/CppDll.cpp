// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include<vector>
#include "CppDll.h"
#include <iostream>


unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w, int ileBit, int position)
{
	 int noRows =( h - (ileBit * (h / ileBit))) > position ? h / ileBit + 1 : h / ileBit;
	int rest = h % ileBit;
	if (rest != 0)
	{
		rest--;
		noRows++;
	}
	// Oblicz rozmiar paddingu
	int padding = (4 - (w * 3) % 4) % 4;
	int index = 0;
	// Przeiteruj przez wszystkie wiersze i kolumny
	for (int y = position; y < position+noRows; y++) {
		if (y == h - 1)
			break;

		for (int x = 1; x < w - 1; x++) {
			// Pobierz indeks dla danego piksela
			index = y * w*3 + x * 3 +padding*y;

			

			 //Przerób dane piksela zgodnie z potrzebami
			b[index] = a[index + 3*w] * -2 + a[index - 3*w] * 2 +
					a[index - 1 + 3*w] * -1 + a[index - 1 - 3*w] * 1 +
					a[index + 1 + 3*w] * -1 + a[index + 1 - 3*w] * 1;
			

		/*	b[index] = a[index + 1] * -2 + a[index - 1] * 2 +
			a[index + 1 - 3*w] * -1 + a[index - 1 - 3*w] * 1 +
			a[index + 1 + 3*w] * -1 + a[index - 1 + 3*w] * 1;*/
			
			b[index + 1] = b[index];
			b[index + 2] = b[index];
		}

		// Przeskocz padding na koñcu wiersza
		//index += padding;
	}
	return b;
}

//unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w, int ileBit, int position)
//{
//	const int noRows = h - (ileBit * (h / ileBit)) > position ? h / ileBit + 1 : h / ileBit;
//
//	// Oblicz rozmiar paddingu
//	int padding = (4 - (w * 3) % 4) % 4;
//	int index = 0;
//	// Przeiteruj przez wszystkie wiersze i kolumny
//	for (int y = position; y < ileBit; y++) {
//		if (y == h - 1)
//			break;
//
//		for (int x = 1; x < w - 1; x++) {
//			// Pobierz indeks dla danego piksela
//			index = y * w * 3 + x * 3 + padding * y;
//
//
//
//			//Przerób dane piksela zgodnie z potrzebami
//			b[index] = a[index + 3 * w] * -2 + a[index - 3 * w] * 2 +
//				a[index - 1 + 3 * w] * -1 + a[index - 1 - 3 * w] * 1 +
//				a[index + 1 + 3 * w] * -1 + a[index + 1 - 3 * w] * 1;
//
//
//			/*	b[index] = a[index + 1] * -2 + a[index - 1] * 2 +
//				a[index + 1 - 3*w] * -1 + a[index - 1 - 3*w] * 1 +
//				a[index + 1 + 3*w] * -1 + a[index - 1 + 3*w] * 1;*/
//
//			b[index + 1] = b[index];
//			b[index + 2] = b[index];
//		}
//
//		// Przeskocz padding na koñcu wiersza
//		//index += padding;
//	}
//	return b;
//}


//// Oblicz rozmiar paddingu
//int padding = (4 - (w * 3) % 4) % 4;
//int index = 0;
//// Przeiteruj przez wszystkie wiersze i kolumny
//for (int y = 1; y < h - 1; y++) {
//
//	for (int x = 1; x < w - 1; x++) {
//		// Pobierz indeks dla danego piksela
//		index = y * w * 3 + x * 3 + padding * y;
//
//
//
//		//Przerób dane piksela zgodnie z potrzebami
//		b[index] = a[index + 3 * w] * -2 + a[index - 3 * w] * 2 +
//			a[index - 1 + 3 * w] * -1 + a[index - 1 - 3 * w] * 1 +
//			a[index + 1 + 3 * w] * -1 + a[index + 1 - 3 * w] * 1;
//
//
//		/*	b[index] = a[index + 1] * -2 + a[index - 1] * 2 +
//			a[index + 1 - 3*w] * -1 + a[index - 1 - 3*w] * 1 +
//			a[index + 1 + 3*w] * -1 + a[index - 1 + 3*w] * 1;*/
//
//		b[index + 1] = b[index];
//		b[index + 2] = b[index];
//	}
//
//	// Przeskocz padding na koñcu wiersza
//	//index += padding;
//}
//return b;
