#include <iostream>
#include <thread>
#include <vector>
#include<windows.h>
#include<fstream>
#include<stdio.h>
#include <functional>

#define BMPFsize 14
#define BMPInfoHeaderSize 40


typedef unsigned char*(*f_funci)(unsigned char*, unsigned char*, int, int,int,int);

typedef unsigned char*(__stdcall* MyProc1)(unsigned char*,unsigned char*,int,int,int,int);


using namespace std;


unsigned char* modified;

int height;
int width;

unsigned char* nic = new unsigned char[100];



//-------------------------------------------

BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;
unsigned char* header;
unsigned char* tab;
unsigned char* grayscale;
int SizeOfdata;
//------------------------------------------


void setGrayScale(int h, int w)
{

    // Oblicz rozmiar paddingu
    int padding = (4 - (w * 3) % 4) % 4;
    int index = 0;
    // Przeiteruj przez wszystkie wiersze i kolumny
    for (int y = 0; y < h; y++) {

        for (int x = 0; x < w; x++) {
            // Pobierz indeks dla danego piksela
            index = y * w * 3 + x * 3;

            // Przerób dane piksela zgodnie z potrzebami
            tab[index] = 0.3 * tab[index] +
                0.59 * tab[index + 1] +
                0.11 * tab[index + 2];
            tab[index + 1] = tab[index];
            tab[index + 2] = tab[index];
        }

        // Przeskocz padding na koñcu wiersza
        index += padding;
    }

    for (int i = 0; i < SizeOfdata; i++)
    {
        modified[i] = 0;
    }
}


int Watki(string s,unsigned char*a,unsigned char* b,int numThreads,int h,int w)
{
    std::string asm1 = "JADll.dll";
    std::string cpp = "CppDll.dll";
    std::wstring stemp = s == "CppDll" ? std::wstring(cpp.begin(), cpp.end()) : std::wstring(asm1.begin(), asm1.end());
 
    HINSTANCE hGetProcIDDLL = LoadLibrary(stemp.c_str());
    HINSTANCE hGetProcIDDLL1 = LoadLibrary(stemp.c_str());
    f_funci funci;
    MyProc1 funci1;
    std::function<unsigned char* (unsigned char*, unsigned char*, int, int,int,int)> fun;




    if (s == "CppDll")
    {
        if (!hGetProcIDDLL1) {
            std::cout << "could not load the dynamic library" << std::endl;
            return EXIT_FAILURE;
        }

        funci= (f_funci)GetProcAddress(hGetProcIDDLL1, "kik");
        if (!funci) {
            std::cout << "could not locate the function" << std::endl;
            return EXIT_FAILURE;
        }

       fun = funci;
    }
    else
    {
        if (!hGetProcIDDLL) {
            std::cout << "could not load the dynamic library" << std::endl;
            return EXIT_FAILURE;
        }
         funci1= (MyProc1)GetProcAddress(hGetProcIDDLL, "MyProc1");
        if (!funci1) {
            std::cout << "could not locate the function" << std::endl;
            return EXIT_FAILURE;
        }

        fun = funci1;
    }

    /////-------divide data to threads
    //int* length = new int[numThreads];
  
    //int rest = bmpInfo.biHeight % numThreads;

    //length[0] = (bmpInfo.biHeight) / numThreads*3;
    //for (int i = 0; i < numThreads; i++)
    //{
    //    length[i++] =length[0]+3*(bmpInfo.biHeight) / numThreads;
    //}
    //for (int i = 0; i < rest; i++)
    //{
    //    length[i]++;
    //}
    ////length[numThreads - 1] += rest;

    //int ileBit = length[0];
    //int arrayStartOffset = 1;

 /*   ///-------divide data to threads
    int* length = new int[numThreads];
    int arraySize = h * w;
    int rest = arraySize % numThreads;

    for (int i = 0; i < numThreads; i++)
    {
        length[i] = (arraySize - rest) / numThreads;
    }
    for (int i = 0; i < rest; i++)
    {
        length[i]++;
    }


    int arrayStartOffset = 0;*/

    ////-----------------
    //int* threadN = new int[numThreads];
    //int numRows = bmpInfo.biHeight / numThreads;
   
    //int rest = bmpInfo.biHeight % numThreads;

    //for (int i = 0; i < numThreads; i++)
    //{
    //    threadN[i] = (bmpInfo.biHeight-rest) / numThreads;
    //}
    //for (int i = 0; i < rest; i++)
    //{
    //    length[i]++;
    //}


    // int StartRow = 0;
    //---------------------



    auto start = std::chrono::steady_clock::now();

    int numOfThreads= numThreads;
    std::vector<std::thread> t;
    int arrayStartOffset=1;
   
    //std::vector<int> liczbaRow(numOfThreads);
    //int rest = h % numOfThreads;
    //for (int i : liczbaRow)
    //{
    //    i = h / numThreads;
    //    if (rest != 0) {
    //        i += 1;
    //        rest--;
    //    }
    //}
    
    int rest = h % numOfThreads;
    

    for (int i = 0; i < numOfThreads; i++)
    {
       /* t.push_back(std::thread(fun,a,b,h,w,length[i], arrayStartOffset));
        arrayStartOffset = +length[i];*/
       int noRows = h - (numOfThreads * (h / numOfThreads)) > arrayStartOffset ? h / numOfThreads + 1 : h / numOfThreads;
        
        if (rest!=0)
        {
            rest--;
            noRows++;
        }
        t.push_back(std::thread(fun, a, b, h, w, numOfThreads, arrayStartOffset));
        arrayStartOffset += noRows;

       /* t.push_back(std::thread(fun, a, b, h, w, liczbaRow[i], arrayStartOffset));
        arrayStartOffset += liczbaRow[i];*/

    }

    for (int i = 0; i < numOfThreads; i++)
    {
        
            t[i].join();


    }
    auto end = std::chrono::steady_clock::now();

    
    std::chrono::duration<double> elapsed_seconds = end - start;

    t.clear();

    std::cout << "time: "<<elapsed_seconds.count()<<endl;

    
    return EXIT_SUCCESS;
}



//void BMPget(string s)
//{
//    ifstream f;
//    f.open(s, std::ios::in | std::ios::binary);
//    if (!f.is_open())
//    {
//        cout << "File could not open" << endl;
//        return;
//    }
//
//    header = new unsigned char[sizeof(bmpFile)+sizeof(bmpInfo)];
//    f.read(reinterpret_cast<char*>(header), sizeof(bmpFile) + sizeof(bmpInfo));
//    f.clear();
//    f.seekg(0);
//    f.read((char*)&bmpFile, sizeof(bmpFile) );
//    f.read((char*)&bmpInfo,  sizeof(bmpInfo));
//
//    SizeOfdata = bmpFile.bfSize - sizeof(bmpFile) - sizeof(bmpInfo);
//    
//    tab = new unsigned char[SizeOfdata];
//    f.read(reinterpret_cast<char*>(tab), SizeOfdata);
//    f.close();
//    grayscale = new unsigned char[SizeOfdata];
//    modified= new unsigned char[SizeOfdata];
//
//}


void BMPget(string s)
{
    ifstream f;
    f.open(s, std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        cout << "File could not open" << endl;
        return;
    }

    header = new unsigned char[54];
    f.read(reinterpret_cast<char*>(header), 54);
    f.clear();
    f.seekg(0);
    f.read((char*)&bmpFile, 14);
    f.read((char*)&bmpInfo, 40);

    SizeOfdata = bmpFile.bfSize - 54;

    tab = new unsigned char[SizeOfdata];
    f.read(reinterpret_cast<char*>(tab), SizeOfdata);
    f.close();
    grayscale = new unsigned char[SizeOfdata];
    modified = new unsigned char[SizeOfdata];

}



void BMPread(string s)
{
    ifstream f;
    f.open(s, std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        cout << "File could not open" << endl;
        return;
    }
    

    header = new unsigned char[54];
    f.read(reinterpret_cast<char*>(header),54);  
    f.clear();
    f.seekg(0);
    f.read((char*)&bmpFile, 14);
    f.read((char*)&bmpInfo, 40);

    int iloscDanych = bmpFile.bfSize - 54; //ofset zamenic

    int szerokosc = bmpInfo.biWidth;
    int wysokosc = bmpInfo.biHeight;
    
    height = wysokosc;
    width = szerokosc;

    int paddingAmount = (4 - (szerokosc * 3) % 4) % 4;
    cout << paddingAmount;

    tab = new unsigned char[iloscDanych];
    modified = new unsigned char[iloscDanych];


    for (int y = 0; y < wysokosc; y++)
    {
        for (int x = 0; x < szerokosc - paddingAmount; x++)
        {
            unsigned char tmp[3];

            f.read(reinterpret_cast<char*>(tmp), 3);
            tab[3 * y * szerokosc + 3 * x] = tmp[2];
            tab[3 * y * szerokosc + 3 * x + 1] = tmp[1];
            tab[3 * y * szerokosc + 3 * x + 2] = tmp[0];
        }

        f.ignore(4 * paddingAmount);
    }


    f.close();

    grayscale= new unsigned char[bmpInfo.biHeight * bmpInfo.biWidth];


}

void BMPsave(unsigned char* table ,const char* destinationFile)
{
    std::ofstream file;
    file.open(destinationFile, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Pliku nie udalo sie zapisac.";
        return;
    }

    file.write(reinterpret_cast<char*>(header), 54); //54
    file.write(reinterpret_cast<char*>(table), SizeOfdata);
    file.close();

}
void saveImage(unsigned char* modifiedData, const char* destinationFile) {
    std::ofstream file;
    file.open(destinationFile, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Pliku nie udalo sie zapisac.";
        return;
    }
    unsigned char padding[3] = { 0, 0, 0 };
    const int paddingSize = (4 - (bmpInfo.biWidth * 3) % 4) % 4;
   
    const int fileSize = 54 + 3 * bmpInfo.biWidth * bmpInfo.biHeight + 3 * bmpInfo.biHeight;
    

    
    file.write(reinterpret_cast<char*>(header), 54);
    for (int y = 0; y < bmpInfo.biHeight; ++y) {
        for (int x = 0; x < bmpInfo.biWidth; ++x) {
            unsigned char r = static_cast<unsigned char> (modifiedData[3 * y * bmpInfo.biWidth + 3 * x]);
            unsigned char g = static_cast<unsigned char> (modifiedData[3 * y * bmpInfo.biWidth + 3 * x + 1]);
            unsigned char b = static_cast<unsigned char> (modifiedData[3 * y * bmpInfo.biWidth + 3 * x + 2]);
            unsigned char c[] = { b, g, r };
            file.write(reinterpret_cast<char*>(c), 3);
        }
        if (paddingSize != 0)
            file.write(reinterpret_cast<char*>(padding), paddingSize);
    }
    file.close();
}








int main()
{
    
    BMPget("C:/notatki-pulpit/Pulpit/test-JA/FilterSobelv2/JAApp/pingwin.bmp");

    setGrayScale(bmpInfo.biHeight,bmpInfo.biWidth);
  // Watki("CppDll", tab, modified, 12, bmpInfo.biHeight,bmpInfo.biWidth);
  


    int ileWatkow = 1;

    int opcja = 0;

    if (opcja == 0)
    {
        Watki("JADll", tab, modified, ileWatkow, bmpInfo.biHeight, bmpInfo.biWidth);
    }
    else if (opcja == 1)
    {
        Watki("CppDll", tab, modified, ileWatkow, bmpInfo.biHeight, bmpInfo.biWidth);
    }
  BMPsave(modified, "nowyy.bmp");




   // BMPread("C:/notatki-pulpit/Pulpit/test-JA/FilterSobelv2/JAApp/pingwin.bmp");

    
   
   /* setGrayScale(tab, height, width);
    
    int ileWatkow = 5;
    
    int opcja = 1;

    if (opcja == 0)
    {
        Watki("JADll", grayscale, modified, ileWatkow, height, width);
    }
    else if (opcja == 1)
    {
        Watki("CppDll", grayscale, modified, ileWatkow, height, width);
    }*/
    
    


    

  //  saveImage(modified, "nowyyy.bmp");

    

   
     

   

   

     

}