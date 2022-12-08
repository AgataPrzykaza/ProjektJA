// MathLibrary.cpp : Defines the exported functions for the DLL.

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include<vector>
#include "CppDll.h"
#include <iostream>


unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w, int ileBit, int position)
{

	// Oblicz rozmiar paddingu
	int padding = (4 - (w * 3) % 4) % 4;
	int index = 0;
	// Przeiteruj przez wszystkie wiersze i kolumny
	for (int y = 1; y < h-1; y++) {
		
		for (int x = 1; x < w-1; x++) {
			// Pobierz indeks dla danego piksela
			index = y * w*3 + x * 3;

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
		index += padding;
	}
	return b;
}

//unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w,int ileBit,int position)
//{
//    
//	int index = position*3;
//
//	for (int i = position; i < position + ileBit; i++)
//	{
//		int row = (i - i % w) / w;	//sprawdzenie row
//		
//		
//		if (i % w == 0 || i % w == w - 1 || row == 0)
//		{
//			index += 3;
//			continue;
//		}
//		
//		else if (row == h - 1)
//		{
//			index += 3;
//			break;
//		}
//		//GY - + w czyli szerokosc wiersza by miec dostep do wyzej lub nizej
//	b[index]= a[i + w] * -2 + a[i - w] * 2 +
//		a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
//		a[i + 1 + w] * -1 + a[i + 1 - w] * 1;
//	b[index + 1] = b[index];
//
//	b[index + 2] = b[index];
//	
//	//gX
//		//b[index] = a[i + 1] * -2 + a[i - 1] * 2 +
//		//a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
//		//a[i + 1 + w] * -1 + a[i - 1 + w] * 1;
//
//		//b[index+1] = a[i + 1] * -2 + a[i - 1] * 2 +
//		//	a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
//		//	a[i + 1 + w] * -1 + a[i - 1 + w] * 1;
//
//		//b[index+2] = a[i + 1] * -2 + a[i - 1] * 2 +
//		//	a[i + 1 - w] * -1 + a[i - 1 - w] * 1 +
//		//	a[i + 1 + w] * -1 + a[i - 1 + w] * 1;
//
//		/*a[i + w] * -2 + a[i - w] * 2 +
//			a[i - 1 + w] * -1 + a[i - 1 - w] * 1 +
//			a[i + 1 + w] * -1 + a[i + 1 - w] * 1;*/
//		index += 3;
//		 
//	}
//
//	
//    return b;
//}