#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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

double czasWykonania;

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

    // std::cout << "time: " << elapsed_seconds.count() << endl;
    czasWykonania = elapsed_seconds.count();
    
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



sf::Text AsmBtn;                             //przycisk asm
sf::Text CppBtn;                             //przycisk cpp
sf::Text Restart;                             //przycisk restart
sf::Text error;
sf::Text Title;
sf::Text text;
sf::RectangleShape poleText(sf::Vector2f(500, 70));                      //pole wpisywania
sf::Text Sciezka;
sf::Texture obrazek;
sf::Sprite sprite;
bool BlokOpcji = false;
sf::Font font;
sf::Text czas;

void ZaladujCzcionke()
{
    font.loadFromFile("arial.ttf");
    text.setFont(font);
}
void ZaladujObrazek()                   //dorobic argumenty
{
    obrazek.loadFromFile("n.bmp");
    
    sprite.setTexture(obrazek, true);
    sprite.setPosition(100, 300);
    sprite.setScale(sf::Vector2f(0.7f, 0.7f));
}

void setUpWindow() {

    
 
    ZaladujCzcionke();

    AsmBtn = text;
    AsmBtn.setCharacterSize(50); 
    AsmBtn.setString("Asembler");
    AsmBtn.setFillColor(sf::Color::White);
    AsmBtn.setPosition(1500, 600);

    error = text;
    error.setCharacterSize(50);
    error.setString("Brak error");
    error.setFillColor(sf::Color::White);
    error.setPosition(1000, 900);

    CppBtn = text;
    CppBtn.setString("Cpp");
    CppBtn.setFillColor(sf::Color::White);
    CppBtn.setCharacterSize(50);
    CppBtn.setPosition(1500, 700);

    Title = text;
    Title.setString("Filtr Sobela");
    Title.setFillColor(sf::Color::White);
    Title.setCharacterSize(100);
    Title.setPosition(1500/2-Title.getLocalBounds().width/2, 100);

    Restart = text;
    Restart.setString("Restart");
    Restart.setFillColor(sf::Color::White);
    Restart.setCharacterSize(50);
    Restart.setPosition(1500, 900);

    Sciezka = text;
    Sciezka.setString("Sciezka");
    Sciezka.setFillColor(sf::Color::Black);
    Sciezka.setCharacterSize(50);
    Sciezka.setPosition(1200, 500);

    czas = text;
    czas.setCharacterSize(50);
    czas.setString("Czas wykonania: ");
    czas.setFillColor(sf::Color::White);
    czas.setPosition(1200, 300);
    

    poleText.setFillColor(sf::Color::White);
    poleText.setPosition(1200, 500);

}
int main()
{
    
    BMPget("C:/notatki-pulpit/Pulpit/pls work/FilterSobelv2/JAApp/stol.bmp");

    // setGrayScale(bmpInfo.biHeight, bmpInfo.biWidth);
 
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Filtr Sobela");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    setUpWindow();
  
    double timex = 0;
  
        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 1, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout <<"1" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 2, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "2" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 4, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "4" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 6, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "6" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 8, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "8" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 12, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "12" << ":" << timex / 100 << endl;

        timex = 0;
        for (int i = 0; i < 100; i++)
        {
            Watki("JADll", tab, modified, 24, bmpInfo.biHeight, bmpInfo.biWidth);
            timex += czasWykonania;
        }
        cout << "24" << ":" << timex / 100 << endl;
   

    // int ileWatkow = 6;

    //int opcja = 0;

    //if (opcja == 0)
    //{
    //    Watki("JADll", tab, modified, ileWatkow, bmpInfo.biHeight, bmpInfo.biWidth);
    //}
    //else if (opcja == 1)
    //{
    //    Watki("CppDll", tab, modified, ileWatkow, bmpInfo.biHeight, bmpInfo.biWidth);
    //}
  //BMPsave(modified, "nowyy.bmp");
   

  boolean displayImage = false;
  boolean poleSciezka = false;
  string watki = "";
  while (window.isOpen())
    { 
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (AsmBtn.getGlobalBounds().contains(mousePosF))
                {
                    if (stoi(watki) == 1 || stoi(watki) == 2 || stoi(watki) == 4 || stoi(watki) == 6 || stoi(watki) == 8 || stoi(watki) == 12 || stoi(watki) == 16 || stoi(watki) == 24 || stoi(watki) == 32)
                    {
                        cout << "na asm" << endl;
                        Watki("JADll", tab, modified, stoi(watki), bmpInfo.biHeight, bmpInfo.biWidth);
                        BMPsave(modified, "n.bmp");
                        ZaladujObrazek();
                        string s = "Czas wykonania: " + to_string(czasWykonania) + "s ";
                        czas.setString(s);
                        displayImage = true;
                        error.setString("Brak error");
                    }
                    else
                    {
                        error.setString("Bledna ilosc watkow");
                    }
                }
                if (CppBtn.getGlobalBounds().contains(mousePosF))
                {
                    if (stoi(watki) == 1 || stoi(watki) == 2 || stoi(watki) == 4 || stoi(watki) == 6 || stoi(watki) == 8 || stoi(watki) == 12 || stoi(watki) == 16 || stoi(watki) == 24 || stoi(watki) == 32)
                    {
                    cout << "na cpp" << endl;
                    Watki("CppDll", tab, modified, stoi(watki), bmpInfo.biHeight, bmpInfo.biWidth);
                    BMPsave(modified, "n.bmp");
                    ZaladujObrazek();
                    string s = "Czas wykonania: "+ to_string(czasWykonania) + "s ";
                    czas.setString(s);
                    displayImage = true;
                    error.setString("Brak error");
                    }
                    else
                    {
                        error.setString("Bledna ilosc watkow");
                    }
                }

                if (Restart.getGlobalBounds().contains(mousePosF))
                {
                    cout << "na restart" << endl;
                    displayImage = false;
                }

                if (poleText.getGlobalBounds().contains(mousePosF))
                {
                    poleSciezka = true;
                }
            }

            if (poleSciezka && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8 && watki.length() > 0) // usuwanie tekstu za pomoc¹ backspace
                {
                    watki.pop_back();
                    Sciezka.setString(watki);
                }
                else if (event.text.unicode != 8 && watki.length() < 19) // wpisywanie tekstu max 19znaków
                {
                    watki.push_back(static_cast<char>(event.text.unicode));
                    Sciezka.setString(watki);
                }

            }
        }

        window.clear(sf::Color(230, 165, 190));
        window.draw(Title);
        window.draw(AsmBtn);
        window.draw(CppBtn);
        window.draw(Restart);
        window.draw(poleText);
        window.draw(Sciezka);
        window.draw(error);

        if (displayImage == true)
        {
            window.draw(sprite);
        }
        
        window.draw(czas);
        window.display();
    }



}