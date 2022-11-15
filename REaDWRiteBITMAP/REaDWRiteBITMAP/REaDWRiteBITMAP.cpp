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

    int paddingAmount = (4 - (szerokosc * 3) % 4) % 4;
    cout << paddingAmount;

     tab=new unsigned char[iloscDanych- bmpInfo.biWidth*paddingAmount];
    
    
    
    for (int y = 0; y < wysokosc; y++)
    {
        for (int x = 0; x < szerokosc-paddingAmount; x++)
        {
            unsigned char tmp[3];
            
            f.read(reinterpret_cast<char*>(tmp), 3);
            tab[3 * y * szerokosc + 3 * x] = tmp[2];
            tab[3 * y * szerokosc + 3 * x+1] =tmp[1];
            tab[3 * y * szerokosc + 3 * x+2] = tmp[0];
        }
       
        f.ignore(paddingAmount);
    }

    
    unsigned char temp;
    for (auto i =0; i <iloscDanych; i += 3)
        {
       

            //std::cout << "R: " << int(tab[i] ) << " G: " << int(tab[i + 1] ) << " B: " << int(tab[i + 2]) << std::endl;
        }

    f.close();

  

}
void BMPWrite(unsigned char* arr)
{
    ofstream nowy("klon.bmp");
    //nowy.open("klon.bmp", std::ios::out | std::ios::binary);

    const int paddingAmount = (4 - (bmpInfo.biWidth * 3) % 4) % 4;
    cout << paddingAmount;
    unsigned char padding[3] = { 0, 0, 0 };

    const int fileSize = 54 + 3 * bmpInfo.biWidth * bmpInfo.biHeight + 3 * bmpInfo.biHeight;

    

    nowy.write((char*)&bmpFile, 14);
    nowy.write((char*)&bmpInfo, 40);

  /*  header[2] = fileSize;
    header[3] = fileSize >> 8;
    header[4] = fileSize >> 16;
    header[5] = fileSize >> 24;
    nowy.write(reinterpret_cast<char*>(header), 54);*/

    for (int y = 0; y < bmpInfo.biHeight; y++)
    {
        for (int x = 0; x < (bmpInfo.biWidth- bmpInfo.biHeight*paddingAmount); ++x) {
          
            unsigned char tmp[3];
            tmp[0] = tab[3 * y * bmpInfo.biHeight + 3 * x+2];
            tmp[1] = tab[3 * y * bmpInfo.biHeight + 3 * x+1];
            tmp[2] = tab[3 * y * bmpInfo.biHeight + 3 * x];

            nowy.write(reinterpret_cast<char*>(tmp), 3);
        }

        nowy.write(reinterpret_cast<char*>(padding), paddingAmount);
    }

   
    nowy.close();

}
int main()
{
    std::cout << "Hello World!\n";
    BMPread("C:/notatki-pulpit/Pulpit/REaDWRiteBITMAP/REaDWRiteBITMAP/pingwin.bmp");
    BMPWrite(tab);
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
