// REaDWRiteBITMAP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include<fstream>
#include<windows.h>
#include <cstring>
#include<vector>
using namespace std;

BITMAPFILEHEADER bmpFile;
BITMAPINFOHEADER bmpInfo;
unsigned char* header;
unsigned char* tab;

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

    int szerokosc= bmpInfo.biWidth;
    int wysokosc = bmpInfo.biHeight;
   // const int paddingAmount = bmpInfo.biWidth % 4;
    int paddingAmount = (4 - (szerokosc * 3) % 4) %4;
    cout << paddingAmount;

     tab=new unsigned char[iloscDanych ];
    
    
    
    for (int y = 0; y < wysokosc; y++)
    {
        for (int x = 0; x < szerokosc-paddingAmount; x++)
        {
            unsigned char tmp[3];
            
            f.read(reinterpret_cast<char*>(tmp), 3);
            tab[3* y * szerokosc +  3*x] = tmp[2];
            tab[3* y * szerokosc +  3*x+1] =tmp[1];
            tab[ 3*y * szerokosc + 3* x+2] = tmp[0];
        }
       
        f.ignore(4*paddingAmount);
    }

    
    unsigned char temp;
    for (auto i =0; i <iloscDanych; i += 3)
        {
       

            //std::cout << "R: " << int(tab[i] ) << " G: " << int(tab[i + 1] ) << " B: " << int(tab[i + 2]) << std::endl;
        }

    f.close();

  

}

void DesprateRead(string s)
{
    ifstream f;
    f.open(s, ios::binary);
    if (!f.is_open()) {
        std::cerr << "\nThe file cannot be opened.\n";
        return;
    }

    f.read((char*)&bmpFile, 14);
    f.read((char*)&bmpInfo, 40);

    tab = new unsigned char[bmpFile.bfSize - 54];
    f.read((char*)tab, bmpFile.bfSize - 54);

    ofstream nowy("kupa.bmp");
    nowy.write((char*)&bmpFile, 14);
    nowy.write((char*)&bmpInfo, 40);
    nowy.write((char*)tab, bmpFile.bfSize - 54);
    nowy.close();
    f.close();
}
void readv2(std::string file, int noThreads)
{
    
        //FILE* f;
        //fopen_s(&f, file.c_str(), "r");
        std::ifstream f;
        f.open(file.c_str(), std::ios::in | std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "\nThe file cannot be opened.\n";
            return;
        }

        header = new unsigned char[54];
        f.read(reinterpret_cast<char*>(header), 54);
        //fread(reinterpret_cast<char*>(header), sizeof(unsigned char), BMP_File_Header, f);
        //currentHeader = new BmpHeader(header);
        f.clear();
        f.seekg(0);
        f.read((char*)&bmpFile, 14);
        f.read((char*)&bmpInfo, 40);

        const int paddingSize = (4 - (bmpInfo.biWidth * 3) % 4) % 4;
        //dt.resize(bmih.biWidth * bmih.biHeight);
        tab = new unsigned char[bmpFile.bfSize - 54];
        //fread(data, sizeof(float), bmfh.bfSize - BMP_File_Header, f);
        for (int y = 0; y < bmpInfo.biHeight; ++y) {
            for (int x = 0; x < bmpInfo.biWidth; ++x) {
                unsigned char colour[3] = { 0,0,0 };
                f.read(reinterpret_cast<char*>(colour), 3);
                /*dt[y * bmih.biWidth + x].red = static_cast<float>(colour[2]) / 255.0f;
                dt[y * bmih.biWidth + x].green = static_cast<float>(colour[1]) / 255.0f;
                dt[y * bmih.biWidth + x].blue = static_cast<float>(colour[0]) / 255.0f;*/
                tab[ y * bmpInfo.biWidth +  x] = static_cast<unsigned char>(colour[2]);
                tab[ y * bmpInfo.biWidth +x + 1] = static_cast<unsigned char>(colour[1]);
                tab[ y * bmpInfo.biWidth +  x + 2] = static_cast<unsigned char>(colour[0]);
            }
            f.ignore(paddingSize);
        }
    
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
    //BITMAPFILEHEADER bmfht = bmfh;
    //BITMAPINFOHEADER bmiht = bmih;
    const int fileSize = 54 + 3 * bmpInfo.biWidth * bmpInfo.biHeight + 3 * bmpInfo.biHeight;
    //dt.resize(bmpInfo.biWidth * bmpInfo.biHeight);
    //bmfht.bfSize = BMP_File_Header + 3 * bmih.biWidth * bmih.biHeight + 3 * bmih.biHeight;

    /*header[2] = fileSize;
    header[3] = fileSize >> 8;
    header[4] = fileSize >> 16;
    header[5] = fileSize >> 24;*/
 

    //setColour(Colour(0, 0, 55), 0, 100);
    file.write(reinterpret_cast<char*>(header), 54);
    for (int y = 0; y < bmpInfo.biHeight; ++y) {
        for (int x = 0; x < bmpInfo.biWidth; ++x) {
            unsigned char r = static_cast<unsigned char> (tab[3* y * bmpInfo.biWidth + 3*x]);
            unsigned char g = static_cast<unsigned char> (tab[3*y * bmpInfo.biWidth + 3*x+1]);
            unsigned char b = static_cast<unsigned char> (tab[3*y * bmpInfo.biWidth + 3*x+2]);
            unsigned char c[] = { b, g, r };
            file.write(reinterpret_cast<char*>(c), 3);
        }
        if(paddingSize!=0)
        file.write(reinterpret_cast<char*>(padding), paddingSize);
    }
    file.close();
}


void BMPWrite(unsigned char* arr)
{
    ofstream nowy("klon.bmp");
    //nowy.open("klon.bmp", std::ios::out | std::ios::binary);
    const int paddingAmount = bmpInfo.biWidth % 4;
   // const int paddingAmount = (4 - (bmpInfo.biWidth*3 ) % 4);
    cout << paddingAmount<<endl;
    unsigned char padding[3] = { 0, 0, 0 };

    const int fileSize = 54 + 3 * bmpInfo.biWidth * bmpInfo.biHeight + 3 * bmpInfo.biHeight;
    cout << fileSize;
    

    nowy.write((char*)&bmpFile, 14);
    nowy.write((char*)&bmpInfo, 40);

 

    for (int y = 0; y < bmpInfo.biHeight; y++)
    {
        for (int x = 0; x < bmpInfo.biWidth- bmpInfo.biHeight *paddingAmount;x++) {
          
            unsigned char tmp[3];
            tmp[0] = tab[3* y * bmpInfo.biHeight + 3* x+2];
            tmp[1] = tab[ 3*y * bmpInfo.biHeight +  3*x+1];
            tmp[2] = tab[ 3*y * bmpInfo.biHeight +  3*x];

            nowy.write(reinterpret_cast<char*>(tmp), 3);
        }
        
        nowy.write(reinterpret_cast<char*>(padding), paddingAmount);
        
    }

   
    nowy.close();

}
int main()
{
    std::cout << "Hello World!\n";
    BMPread("C:/notatki-pulpit/Pulpit/REaDWRiteBITMAP/REaDWRiteBITMAP/parrots.bmp"); //to dziala razem z saveImage!!!!!!!!!!!!!!!!
     //readv2("C:/notatki-pulpit/Pulpit/REaDWRiteBITMAP/REaDWRiteBITMAP/pingwin.bmp", 2);
    // BMPWrite(tab);

 
   saveImage(tab, "pupa.bmp");

   // DesprateRead("C:/notatki-pulpit/Pulpit/REaDWRiteBITMAP/REaDWRiteBITMAP/pingwin.bmp");
}

