#include <iostream>
#include <thread>
#include <vector>
#include<windows.h>
#include<fstream>
#include<stdio.h>
#include <functional>




typedef void(*f_funci)(unsigned char*, unsigned char*, int, int,int,int);

typedef void(*MyProc1)(unsigned char*,unsigned char*,int,int,int,int);


using namespace std;


unsigned char* modified;

int height;
int width;

unsigned char* nic = new unsigned char[100];

int bmpfsize = 14;
int bmpinfosize = 40;


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
    std::function<void (unsigned char*, unsigned char*, int, int,int,int)> fun;




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





    auto start = std::chrono::steady_clock::now();

    int numOfThreads= numThreads;
    std::vector<std::thread> t;
    int arrayStartOffset=1;
   
 
    
    int rest = h % numOfThreads;
    

    for (int i = 0; i < numOfThreads; i++)
    {
       
       int noRows = h - (numOfThreads * (h / numOfThreads)) > arrayStartOffset ? h / numOfThreads + 1 : h / numOfThreads;
      
        if (rest!=0)
        {
            rest--;
            noRows++;
        }
        t.push_back(std::thread(fun, a, b, h, w, noRows, arrayStartOffset));
        arrayStartOffset += noRows;

       

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






void BMPget(string s)
{
    ifstream f;
    f.open(s, std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        cout << "File could not open" << endl;
        return;
    }

    header = new unsigned char[bmpfsize+bmpinfosize];
    f.read(reinterpret_cast<char*>(header), bmpfsize + bmpinfosize);
    f.clear();
    f.seekg(0);
    f.read((char*)&bmpFile, bmpfsize);
    f.read((char*)&bmpInfo, bmpinfosize);

    SizeOfdata = bmpFile.bfSize - bmpfsize-bmpinfosize;

    tab = new unsigned char[SizeOfdata];
    f.read(reinterpret_cast<char*>(tab), SizeOfdata);
    f.close();
    grayscale = new unsigned char[SizeOfdata];
    modified = new unsigned char[SizeOfdata];

}





void BMPsave(unsigned char* table ,const char* destinationFile)
{
    std::ofstream file;
    file.open(destinationFile, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Pliku nie udalo sie zapisac.";
        return;
    }

    file.write(reinterpret_cast<char*>(header), bmpfsize+bmpinfosize); 
    file.write(reinterpret_cast<char*>(table), SizeOfdata);
    file.close();

}





int main()
{
    
    BMPget("C:/notatki-pulpit/Pulpit/test-JA/FilterSobelv2/JAApp/stol.bmp");

    // setGrayScale(bmpInfo.biHeight, bmpInfo.biWidth);
 
  


    int ileWatkow = 20;

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





}