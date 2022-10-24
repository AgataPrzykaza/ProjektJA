#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include<windows.h>
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


void BMPread(string s)
{
        ifstream bmp(s, ios::binary);
        BITMAPFILEHEADER bmpFile;
        BITMAPINFOHEADER bmpInfo;

        if (bmp.is_open())
        {
           

           bmp.read((char*)&bmpFile, 14);
           bmp.read((char*)&bmpInfo, 40);

           

            int szerokosc = bmpInfo.biWidth;
            int wysokosc = bmpInfo.biHeight;

            
            int dopelnienie = 4 - (3*szerokosc) % 4;
            cout << "szer=" << szerokosc << endl;
            cout << "dop=" << dopelnienie << endl;

            int iloscDanych = bmpFile.bfSize - 14 - 40;


           
           vector<char> a(iloscDanych);
           auto dataSize = ((szerokosc* 3 + 3) & (~3)) * wysokosc;

           a.resize(dataSize);
            

           bmp.read(a.data(), a.size());
            

           
           bmp.close();

           char temp = 0;

          /* for (auto i = iloscDanych - 4; i >= 0; i -= 3)
           {
               temp = a[i];
               a[i] = a[i + 2];
               a[i + 2] = temp;

               std::cout << "R: " << int(a[i] & 0xff) << " G: " << int(a[i + 1] & 0xff) << " B: " << int(a[i + 2] & 0xff) << std::endl;
           }*/

            
            ofstream nowy("klon.bmp");

           
            nowy.write((char*)&bmpFile, 14);
            nowy.write((char*)&bmpInfo, 40);

            nowy.write(a.data(), iloscDanych);
            nowy.close();


        }
    

  
}
void przetworzBMP(ifstream& s)
{
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;

    if (s.is_open())
    {
        //pobranie naglowka zajmuje 14 bajtow i 40 bajtow

        s.read((char*)&bmpFile, 14);
        s.read((char*)&bmpInfo, 40);

        //wydobycie informacji o rozmiarze pliku

        int szerokosc = bmpInfo.biWidth;
        int wysokosc = bmpInfo.biHeight;

        //dlugosc kazdego wiersza musi byc podzelna przez 4 - wiersz dopelniany jest zerami :-)
        int dopelnienie = 4 - (3 * szerokosc) % 4;
        cout << "szer=" << szerokosc << endl;
        cout << "dop=" << dopelnienie << endl;

        int iloscDanych = bmpFile.bfSize - 14 - 40;


        //rezerwuje miejsce na zawartosc mapy bitow
        unsigned char* dane = new unsigned char[iloscDanych];

        //wczytanie danych

        s.read((char*)dane, iloscDanych);
        s.close();

        //utworzenie nowego pliku ze zmienionym kolorem
        ofstream nowy("klon.bmp");

        //kopiowani naglowków
        nowy.write((char*)&bmpFile, 14);
        nowy.write((char*)&bmpInfo, 40);

        //transformacja bitow

        unsigned char* wsk = dane;

        for (int i = 0; i < wysokosc; i++)
        {

            for (int j = 0; j < szerokosc; j++)
            {


                *wsk = (byte)255;
                *(wsk + 1) = (byte)0;
                *(wsk + 2) = (byte)100;
                wsk += 3;

            }
            wsk += dopelnienie;

        }

        nowy.write((char*)dane, iloscDanych);
        nowy.close();

        delete[] dane;


    }
}

//unsigned char* readBMP(char* filename)
//{
//    int i;
//    FILE* f;
//    fopen_s(&f,filename, "rb");
//    unsigned char info[54];
//    //fread_s(info, sizeof(unsigned char), 54, f); // read the 54-byte header
//    fread_s(info, 54 * sizeof(unsigned char), sizeof(unsigned char), 54, f);
//    // extract image height and width from header
//    int width, height;
//    memcpy(&width, info + 18, sizeof(int));
//    memcpy(&height, info + 22, sizeof(int));
//
//    int heightSign = 1;
//    if (height < 0) {
//        heightSign = -1;
//    }
//
//    int size = 3 * width * abs(height);
//    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
//    fread_s(data,sizeof(unsigned char)*size, sizeof(unsigned char), size, f); // read the rest of the data at once
//    fclose(f);
//
//    FILE* g;
//    fopen_s(&g,"klon.bmp", "wb");
//    fwrite(info, sizeof(unsigned char), 54, g);
//    fwrite(data, sizeof(unsigned char), size, g);
//    //if (heightSign == 1) {
//    //    for (i = 0; i < size; i += 3)
//    //    {
//    //        //code to flip the image data here....
//    //    }
//    //}
//
//    fclose(g);
//    return data;
//}

int main()
{  
    CallAsm();
    CallCpp();
    BMPread("C:/notatki-pulpit/Pulpit/JA 5/SobelFilter/parrots.bmp");
   //ifstream bmp("C:/notatki-pulpit/Pulpit/JA 5/SobelFilter/parrots.bmp",ios::binary);
    //przetworzBMP(bmp);
    

    /*cout << endl;
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
    }*/
    
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
  
}