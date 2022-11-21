#include <iostream>
#include <thread>
#include <vector>
#include<windows.h>
#include<fstream>
#include<stdio.h>
#include <functional>


//typedef int(* f_funci)(std::vector<char>);
//typedef int(*f_funci)(unsigned char*, unsigned char*, int, int);
typedef unsigned char*(*f_funci)(unsigned char*, unsigned char*, int, int,int,int);
//typedef int(__stdcall* MyProc1)(unsigned char*, unsigned char*,int,int,_int32);
typedef unsigned char*(__stdcall* MyProc1)(unsigned char*,unsigned char*,int,int,int,int);


using namespace std;
std::vector<char> V;
int lol = 12;

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
//------------------------------------------


void setGrayScale(unsigned char* t, int h, int w)
{
    
   /* int index = 0;
    int step = 3 * width;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width * 3; col += 3) {
            gray[index] = 0.3 * t[row * step + col] +
                0.59 * t[row * step + col + 1] +
                0.11 * t[row * step + col + 2];

            gray[index +1] = 0.3 * t[row * step + col] +
                0.59 * t[row * step + col + 1] +
                0.11 * t[row * step + col + 2];

            gray[index+2] = 0.3 * t[row * step + col] +
                0.59 * t[row * step + col + 1] +
                0.11 * t[row * step + col + 2];
            
            index+=3;
        }
    }*/

    int index = 0;
    int step = 3 * width;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width * 3; col += 3) {
            grayscale[index] = 0.3 * t[row * step + col] +
                0.59 * t[row * step + col + 1] +
                0.11 * t[row * step + col + 2];

                index ++;
        }
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

    ///-------divide data to threads
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


    int arrayStartOffset = 0;

    //-----------------




    auto start = std::chrono::steady_clock::now();

    int numOfThreads= numThreads;
    std::vector<std::thread> t;

    int ileBIt = 3*h*w;
    
  
    unsigned char* alaaaa = new unsigned char[10];

    for (int i = 0; i < numOfThreads; i++)
    {
        t.push_back(std::thread(fun,a,b,h,w,length[i], arrayStartOffset));
        arrayStartOffset += length[i];
       

    }

    for (int i = 0; i < numOfThreads; i++)
    {
        
            t[i].join();


    }
    auto end = std::chrono::steady_clock::now();

    
    std::chrono::duration<double> elapsed_seconds = end - start;

    t.clear();

    std::cout << "time: "<<elapsed_seconds.count()<<endl;

    cout << (int)b[0] << " " << (int)b[1] << " " << (int)b[2] << endl;

    return EXIT_SUCCESS;
}

int CallCpp(unsigned char* a, unsigned char* b, int h, int w)
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
    int sa = 2;
    std::cout << "funci() returned " << funci(a, b, h, w,4,12) << std::endl;

    //int arrayStartOffset = 0;
    //auto start = std::chrono::steady_clock::now();

    //int numOfThreads=10;
    //std::vector<std::thread> t;

    //for (int i = 0; i < numOfThreads; i++)
    //{
    //    t.push_back(std::thread(funci,a));
    //}

    //for (int i = 0; i < numOfThreads; i++)
    //{
    //    if (t[i].joinable())
    //    {
    //        t[i].join();

    //    }
    //}
    //auto end = std::chrono::steady_clock::now();

    //
    //std::chrono::duration<double> elapsed_seconds = end - start;

    //t.clear();

    //std::cout << "time: "<<elapsed_seconds.count();

    return EXIT_SUCCESS;
}



int CallAsm(unsigned char* a, unsigned char* b, int h, int w)
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

    

    
    char ch=24;

   // std::cout << "funci1() returned " << funci1(lol) << std::endl;
    
    
  
    unsigned char* ola = funci1(a,b,h,w,4,12);
   
    cout << int(ola[2]);

  /*  unsigned char* NOwaSUPerTAblica = ( unsigned char*)funci1(a, b, h, w, lol);

    std::cout<<NOwaSUPerTAblica[0];*/

    int arrayStartOffset = 0;
    auto start = std::chrono::steady_clock::now();

    int numOfThreads = 10;
    std::vector<std::thread> t;

  /*  for (int i = 0; i < numOfThreads; i++)
    {
        t.push_back(std::thread(funci1, 3, 4,5));
    }

    for (int i = 0; i < numOfThreads; i++)
    {

        t[i].join();


    }
    auto end = std::chrono::steady_clock::now();


    std::chrono::duration<double> elapsed_seconds = end - start;

    t.clear();*/

    //std::cout << "time: " << elapsed_seconds.count() << std::endl;
    return EXIT_SUCCESS;
}


//void BMPread(string s)
//{
//    ifstream bmp(s, ios::binary);
//    BITMAPFILEHEADER bmpFile;
//    BITMAPINFOHEADER bmpInfo;
//
//    if (bmp.is_open())
//    {
//
//
//        bmp.read((char*)&bmpFile, 14);
//        bmp.read((char*)&bmpInfo, 40);
//
//
//
//        int szerokosc = bmpInfo.biWidth;
//        int wysokosc = bmpInfo.biHeight;
//        height = wysokosc;
//        width = szerokosc;
//
//        int dopelnienie = 4 - (3 * szerokosc) % 4;
//        cout << "szer=" << szerokosc << endl;
//        cout << "dop=" << dopelnienie << endl;
//
//        int iloscDanych = bmpFile.bfSize - 14 - 40;
//
//
//
//        vector<char> a(iloscDanych);
//        auto dataSize = ((4 - (szerokosc * 3) % 4) % 4) * wysokosc;
//
//        a.resize(dataSize);
//
//
//        bmp.read(a.data(), a.size());
//        unsigned char* buffer = new unsigned char[a.size()];
//
//        tab = new unsigned char[a.size()];
//        modified = new unsigned char[a.size()];
//
//        std::copy(a.begin(), a.end(), buffer);
//        std::copy(a.begin(), a.end(), tab);
//
//        V = a;
//
//
//
//        //unsigned char* l;
//        //l = new unsigned char[a.size()];
//        //// std::cout << "R: " << int(buffer[0] & 0xff) << " G: " << int(buffer[1] & 0xff) << " B: " << int(buffer[2] & 0xff) << std::endl;
//        //std::cout << "R: " << int(buffer[0]) << " G: " << int(buffer[1]) << " B: " << int(buffer[2]) << std::endl;
//        //CallCpp(buffer, l, wysokosc, szerokosc);
//        //std::cout << "R: " << int(l[0]) << " G: " << int(l[1]) << " B: " << int(l[2]) << std::endl;
//        ////std::cout << "R: " << int(buffer[0] & 0xff) << " G: " << int(buffer[ 1] & 0xff) << " B: " << int(buffer[2] & 0xff) << std::endl;
//        //bmp.close();
//
//        char temp = 0;
//
//        /*for (auto i = iloscDanych - 4; i >= 0; i -= 3)
//        {
//            temp = a[i];
//            a[i] = a[i + 2];
//            a[i + 2] = temp;
//
//            std::cout << "R: " << int(a[i] & 0xff) << " G: " << int(a[i + 1] & 0xff) << " B: " << int(a[i + 2] & 0xff) << std::endl;
//        }*/
//
//        
//           ofstream nowy("klon.bmp");
//
//           V.clear();
//           std::copy(buffer, buffer + a.size(), std::back_inserter(V));
//           nowy.write((char*)&bmpFile, 14);
//           nowy.write((char*)&bmpInfo, 40);
//
//           nowy.write(a.data(), iloscDanych);
//           nowy.close();
//
//
//    }
//
//
//
//}


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
    f.read(reinterpret_cast<char*>(header), 54);
    f.clear();
    f.seekg(0);
    f.read((char*)&bmpFile, 14);
    f.read((char*)&bmpInfo, 40);

    int iloscDanych = bmpFile.bfSize - 14 - 40;

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


    unsigned char temp;
    for (auto i = 0; i < iloscDanych; i += 3)
    {


        //std::cout << "R: " << int(tab[i] ) << " G: " << int(tab[i + 1] ) << " B: " << int(tab[i + 2]) << std::endl;
    }

    f.close();

    grayscale= new unsigned char[bmpInfo.biHeight * bmpInfo.biWidth];


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


//void przetworzBMP(ifstream& s)
//{
//    BITMAPFILEHEADER bmpFile;
//    BITMAPINFOHEADER bmpInfo;
//
//    if (s.is_open())
//    {
//        //pobranie naglowka zajmuje 14 bajtow i 40 bajtow
//
//        s.read((char*)&bmpFile, 14);
//        s.read((char*)&bmpInfo, 40);
//
//        //wydobycie informacji o rozmiarze pliku
//
//        int szerokosc = bmpInfo.biWidth;
//        int wysokosc = bmpInfo.biHeight;
//
//        //dlugosc kazdego wiersza musi byc podzelna przez 4 - wiersz dopelniany jest zerami :-)
//        int dopelnienie = 4 - (3 * szerokosc) % 4;
//        cout << "szer=" << szerokosc << endl;
//        cout << "dop=" << dopelnienie << endl;
//
//        int iloscDanych = bmpFile.bfSize - 14 - 40;
//
//
//        //rezerwuje miejsce na zawartosc mapy bitow
//        unsigned char* dane = new unsigned char[iloscDanych];
//
//        //wczytanie danych
//
//        s.read((char*)dane, iloscDanych);
//        s.close();
//
//        //utworzenie nowego pliku ze zmienionym kolorem
//        ofstream nowy("klon.bmp");
//
//        //kopiowani naglowków
//        nowy.write((char*)&bmpFile, 14);
//        nowy.write((char*)&bmpInfo, 40);
//
//        //transformacja bitow
//
//        unsigned char* wsk = dane;
//
//        for (int i = 0; i < wysokosc; i++)
//        {
//
//            for (int j = 0; j < szerokosc; j++)
//            {
//
//
//                *wsk = (byte)255;
//                *(wsk + 1) = (byte)0;
//                *(wsk + 2) = (byte)100;
//                wsk += 3;
//
//            }
//            wsk += dopelnienie;
//
//        }
//
//        nowy.write((char*)dane, iloscDanych);
//        nowy.close();
//
//        delete[] dane;
//
//
//    }
//}

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
    

    BMPread("C:/notatki-pulpit/Pulpit/FilterSobelv1/JAApp/pingwin.bmp");

    unsigned char* ala = new unsigned char[90];
   
    setGrayScale(tab, height, width);

    //Watki("CppDll",grayscale, modified, 2, height, width);
    Watki("JADll", grayscale, modified, 2, height, width);
    //Watki("JADll", tab, modified, 4, height, width);
    //Watki("CppDll", tab, modified, 4, height, width);


    

    saveImage(modified, "now.bmp");

    

   // Watki("JADll", nic, modified, 4, height, width);

     

   

   

     

}