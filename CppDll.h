#pragma once
#include<vector>

#ifdef MATHLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif


extern "C" MATHLIBRARY_API unsigned char* kik(unsigned char* a, unsigned char* b, int h, int w,int th);