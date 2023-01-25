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
sf::RectangleShape poleObraz(sf::Vector2f(500, 70));                      //pole wpisywania
sf::Text ileWatkow;
sf::Text nazwa;
bool BlokOpcji = false;
sf::Font font;
sf::Text czas;

void ZaladujCzcionke()
{
    font.loadFromFile("arial.ttf");
    text.setFont(font);
}


void setUpWindow() {

    
 
    ZaladujCzcionke();

    AsmBtn = text;
    AsmBtn.setCharacterSize(50); 
    AsmBtn.setString("Asembler");
    AsmBtn.setFillColor(sf::Color::White);
    AsmBtn.setPosition(200, 600);

    error = text;
    error.setCharacterSize(50);
    error.setString("Brak error");
    error.setFillColor(sf::Color::White);
    error.setPosition(1000, 900);

    CppBtn = text;
    CppBtn.setString("Cpp");
    CppBtn.setFillColor(sf::Color::White);
    CppBtn.setCharacterSize(50);
    CppBtn.setPosition(200, 700);

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

    ileWatkow = text;
    ileWatkow.setString("ileWatkow");
    ileWatkow.setFillColor(sf::Color::Black);
    ileWatkow.setCharacterSize(50);
    ileWatkow.setPosition(1200, 400);


    nazwa = text;
    nazwa.setString("Nazwa BMP");
    nazwa.setFillColor(sf::Color::Black);
    nazwa.setCharacterSize(50);
    nazwa.setPosition(600, 400);

    czas = text;
    czas.setCharacterSize(50);
    czas.setString("Czas wykonania: ");
    czas.setFillColor(sf::Color::White);
    czas.setPosition(1000, 300);
    

    poleText.setFillColor(sf::Color::White);
    poleText.setPosition(1200, 400);


    poleObraz.setFillColor(sf::Color::White);
    poleObraz.setPosition(600, 400);

}
int main()
{
    
   


 
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Filtr Sobela");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    setUpWindow();
  
   



  boolean poleSciezka = false;
  boolean poleNazwa = false;
  string watki = "";
  string nazwaBMP = "";
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
                    if ((stoi(watki) == 1 || stoi(watki) == 2 || stoi(watki) == 4 || stoi(watki) == 6 || stoi(watki) == 8 || stoi(watki) == 12 || stoi(watki) == 16 || stoi(watki) == 24 || stoi(watki) == 32)&&nazwaBMP!="")
                    {
                        cout << "na asm" << endl;
                        BMPget("C:/notatki-pulpit/Pulpit/desprate/FilterSobelv2/JAApp/"+nazwaBMP);
                        Watki("JADll", tab, modified, stoi(watki), bmpInfo.biHeight, bmpInfo.biWidth);
                        BMPsave(modified, "sad.bmp");
                       
                        string s = "Czas wykonania: " + to_string(czasWykonania) + "s ";
                        czas.setString(s);
                     
                        error.setString("Brak error");
                    }
                    else
                    {
                        error.setString("Bledna ilosc watkow");
                    }
                }
                if (CppBtn.getGlobalBounds().contains(mousePosF))
                {
                    if ((stoi(watki) == 1 || stoi(watki) == 2 || stoi(watki) == 4 || stoi(watki) == 6 || stoi(watki) == 8 || stoi(watki) == 12 || stoi(watki) == 16 || stoi(watki) == 24 || stoi(watki) == 32)&&nazwaBMP!="")
                    {
                    cout << "na cpp" << endl;
                    BMPget("C:/notatki-pulpit/Pulpit/desprate/FilterSobelv2/JAApp/"+nazwaBMP);
                    Watki("CppDll", tab, modified, stoi(watki), bmpInfo.biHeight, bmpInfo.biWidth);
                    BMPsave(modified, "sad.bmp");
                 
                    string s = "Czas wykonania: "+ to_string(czasWykonania) + "s ";
                    czas.setString(s);
                  
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
                    
                }

                if (poleText.getGlobalBounds().contains(mousePosF))
                {
                    poleSciezka = true;
                    poleNazwa = false;
                }

                if (poleObraz.getGlobalBounds().contains(mousePosF))
                {
                    poleNazwa = true;
                    poleSciezka = false;
                }
            }

            if (poleSciezka && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8 && watki.length() > 0) // usuwanie tekstu za pomoc¹ backspace
                {
                    watki.pop_back();
                    ileWatkow.setString(watki);
                }
                else if (event.text.unicode != 8 && watki.length() < 19) // wpisywanie tekstu max 19znaków
                {
                    watki.push_back(static_cast<char>(event.text.unicode));
                    ileWatkow.setString(watki);
                }

            }


            if (poleNazwa && event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == 8 && nazwaBMP.length() > 0) // usuwanie tekstu za pomoc¹ backspace
                {
                    nazwaBMP.pop_back();
                    nazwa.setString(nazwaBMP);
                }
                else if (event.text.unicode != 8 && nazwaBMP.length() < 19) // wpisywanie tekstu max 19znaków
                {
                    nazwaBMP.push_back(static_cast<char>(event.text.unicode));
                    nazwa.setString(nazwaBMP);
                }

            }
        }

        window.clear(sf::Color(230, 165, 190));
        window.draw(Title);
        window.draw(AsmBtn);
        window.draw(CppBtn);
        window.draw(Restart);
        window.draw(poleText);
        window.draw(poleObraz);
        window.draw(nazwa);
        window.draw(ileWatkow);
        window.draw(error);

     
        
        window.draw(czas);
        window.display();
    }



}