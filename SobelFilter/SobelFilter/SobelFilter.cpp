#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include "BitMap.h"

typedef int(__stdcall* f_funci)();

typedef int(_stdcall* MyProc1)(int, int);

int CallCpp()
{
    std::string s = "CppDll.dll";
    std::wstring stemp = std::wstring(s.begin(), s.end());
    HINSTANCE hGetProcIDDLL = LoadLibrary(stemp.c_str());

    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }

    f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "kik");
    if (!funci) {
        std::cout << "could not locate the function" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "funci() returned " << funci() << std::endl;

    int arrayStartOffset = 0;
    auto start = std::chrono::steady_clock::now();

    int numOfThreads=10;
    std::vector<std::thread> t;

    for (int i = 0; i < numOfThreads; i++)
    {
        t.push_back(std::thread(funci));
    }

    for (int i = 0; i < numOfThreads; i++)
    {
        if (t[i].joinable())
        {
            t[i].join();

        }
    }
    auto end = std::chrono::steady_clock::now();

    
    std::chrono::duration<double> elapsed_seconds = end - start;

    t.clear();

    std::cout << "time: "<<elapsed_seconds.count();
  
    return EXIT_SUCCESS;
}

int CallAsm()
{
    std::string s1 = "JADll.dll";
    std::wstring stemp1 = std::wstring(s1.begin(), s1.end());
    HINSTANCE hGetProcIDDLL1 = LoadLibrary(stemp1.c_str());

    if (!hGetProcIDDLL1) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }
    MyProc1 funci1 = (MyProc1)GetProcAddress(hGetProcIDDLL1, "MyProc1");
    if (!funci1) {
        std::cout << "could not locate the function" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "funci1() returned " << funci1(3, 4) << std::endl;

    int arrayStartOffset = 0;
    auto start = std::chrono::steady_clock::now();

    int numOfThreads = 10;
    std::vector<std::thread> t;

    for (int i = 0; i < numOfThreads; i++)
    {
        t.push_back(std::thread(funci1,3,4));
    }

    for (int i = 0; i < numOfThreads; i++)
    {
        if (t[i].joinable())
        {
            t[i].join();

        }
    }
    auto end = std::chrono::steady_clock::now();


    std::chrono::duration<double> elapsed_seconds = end - start;

    t.clear();

    std::cout << "time: " << elapsed_seconds.count()<<std::endl;
    return EXIT_SUCCESS;
}

int main()
{  
    CallAsm();
    CallCpp();
    

    cout << endl;
    char* fileName = (char*)"C:/notatki-pulpit/Pulpit/JA 5/SobelFilter/kotek.bmp";
std:cout << fileName;
    ifstream pFile(fileName, ios::in | ios::binary);

    BitMapFileHeader header [[gnu::unused]];

    header.bfType = read_u16(pFile);
    header.bfSize = read_u32(pFile);
    header.bfReserved1 = read_u16(pFile);
    header.bfReserved2 = read_u16(pFile);
    header.bfOffBits = read_u32(pFile);

    BitMapInfoHeader bmiHeader;

    bmiHeader.biSize = read_u32(pFile);
    bmiHeader.biWidth = read_s32(pFile);
    bmiHeader.biHeight = read_s32(pFile);
    bmiHeader.biPlanes = read_u16(pFile);
    bmiHeader.biBitCount = read_u16(pFile);
    bmiHeader.biCompression = read_u32(pFile);
    bmiHeader.biSizeImage = read_u32(pFile);
    bmiHeader.biXPelsPerMeter = read_s32(pFile);
    bmiHeader.biYPelsPerMeter = read_s32(pFile);
    bmiHeader.biClrUsed = read_u32(pFile);
    bmiHeader.biClrImportant = read_u32(pFile);

    std::cout << " h size: " << bmiHeader.biSize << " x:" << bmiHeader.biWidth << " y:" << bmiHeader.biHeight << " planes:" << bmiHeader.biPlanes << " bity:" << bmiHeader.biBitCount << " kompresja:" << bmiHeader.biCompression << " sizeimage:" << bmiHeader.biSizeImage << " header.bfType:" << endl << header.bfType << " header.bfSize:" << header.bfSize;

    std::vector<char> img(header.bfOffBits - 54);
    pFile.read(img.data(), img.size());

    auto dataSize = ((bmiHeader.biWidth * 3 + 3) & (~3)) * bmiHeader.biHeight;
    img.resize(dataSize);
    pFile.read(img.data(), img.size());

    char temp = 0;

    for (auto i = dataSize - 4; i >= 0; i -= 3)
    {
        temp = img[i];
        img[i] = img[i + 2];
        img[i + 2] = temp;

        std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << std::endl;
    }
    
   /* std::string s = "CppDll.dll";
    std::string s1 = "JADll.dll";
    std::wstring stemp = std::wstring(s.begin(), s.end());
    std::wstring stemp1 = std::wstring(s1.begin(), s1.end());
    HINSTANCE hGetProcIDDLL = LoadLibrary(stemp.c_str());
    HINSTANCE hGetProcIDDLL1 = LoadLibrary(stemp1.c_str());

    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }
    if (!hGetProcIDDLL1) {
        std::cout << "could not load the dynamic library" << std::endl;
        return EXIT_FAILURE;
    }

    MyProc1 funci1 = (MyProc1)GetProcAddress(hGetProcIDDLL1, "MyProc1");
    if (!funci1) {
        std::cout << "could not locate the function" << std::endl;
        return EXIT_FAILURE;
    }

    f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "kik");
    if (!funci) {
        std::cout << "could not locate the function" << std::endl;
        return EXIT_FAILURE;
    }

   std::cout << "funci() returned " << funci() << std::endl;
    std::cout << "funci1() returned " << funci1(3,4) << std::endl;
    return EXIT_SUCCESS; */
    //// Initialize a Fibonacci relation sequence.
    //fibonacci_init(1, 1);
    //// Write out the sequence values until overflow.
    //do {
    //    std::cout << fibonacci_index() << ": "
    //        << fibonacci_current() << std::endl;
    //} while (fibonacci_next());
    //// Report count of values written before overflow.
    //std::cout << fibonacci_index() + 1 <<
    //    " Fibonacci sequence values fit in an " <<
    //    "unsigned 64-bit integer." << std::endl;
}