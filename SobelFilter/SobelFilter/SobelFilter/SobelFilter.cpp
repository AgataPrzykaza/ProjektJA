#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>


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
    return EXIT_SUCCESS;
}

int main()
{  
    CallAsm();
    CallCpp();

    

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